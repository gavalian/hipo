#pragma once

#include <bank.h>
#include <dictionary.h>
#include <event.h>
#include <reader.h>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

namespace hipo {
class hipoeventfile {

    friend class hipoevent;

   public:
    /** Iterator type for HipoEventFile (input iterator). */
    class iterator;  // defined below
    using const_iterator = iterator;

   private:
    hipo::reader reader;
    dictionary dict;
    std::map<std::string, bank> bank_templates;              // Store bank templates by name
    std::map<std::pair<int, int>, bank> tag_bank_templates;  // Store bank templates by tag

   public:
    /** Open a HIPO file and read its dictionary. Throws if file cannot be opened. */
    explicit hipoeventfile(const std::string& filename) {
        reader.open(filename.c_str());
        reader.readDictionary(dict);

        // Pre-create bank templates for all known schemas
        for (const std::string& name : dict.getSchemaList()) {
            schema& sch = dict.getSchema(name.c_str());
            bank_templates.emplace(name, bank(sch));

            // Also store by group/item
            tag_bank_templates.emplace(
                std::make_pair(sch.getGroup(), sch.getItem()),
                bank(sch));
        }
    }

    /** Iterator pointing to the first event in the file. */
    iterator begin();

    /** End iterator (past-the-last event). */
    iterator end();

    /** Get a bank template by name */
    bank& get_bank_template(const std::string& name) {
        auto it = bank_templates.find(name);
        if (it == bank_templates.end()) {
            throw std::runtime_error("Schema not found for bank: " + name);
        }
        return it->second;
    }

    /** Get a bank template by tag */
    bank& get_bank_template(int group, int item) {
        auto tag = std::make_pair(group, item);
        auto it = tag_bank_templates.find(tag);
        if (it == tag_bank_templates.end()) {
            throw std::runtime_error("Schema for group=" + std::to_string(group) +
                                     ", item=" + std::to_string(item) + " not found");
        }
        return it->second;
    }
};

class hipoevent {
    friend class hipoeventfile;
    friend class hipoeventfileiterator;

   private:
    event* event_ptr;
    hipoeventfile* file_ptr;                                     // Reference to parent file for bank templates

    // Private constructor used by iterator to wrap current event
    hipoevent(event* ev, hipoeventfile* file)
        : event_ptr(ev), file_ptr(file) {
    }

   public:
    hipoevent() : event_ptr(nullptr), file_ptr(nullptr) {
    }

    /** Get a bank by schema name (e.g., "RUN::config") from the current event. */
    bank& get_bank(const std::string& bankName) const {
        if (!event_ptr || !file_ptr) throw std::runtime_error("Invalid HipoEvent (no event data)");

        // std::cout << "Getting bank: " << bankName << std::endl;
        // Check if the bank is already in the cache for this event
        auto it = file_ptr->bank_templates.find(bankName);
        if (it == file_ptr->bank_templates.end()) {
            std::cout << "Bank not found in cache: " << bankName << std::endl;
            // Not in cache, get template from file and read event data into it
            bank new_bank = file_ptr->get_bank_template(bankName);  // Copy the template
            event_ptr->read(new_bank);                              // Fill with current event's data
            auto result = file_ptr->bank_templates.emplace(bankName, std::move(new_bank));
            return result.first->second;
        }
        event_ptr->read(it->second);  // Fill with current event's data
        return it->second;
    }

    ///** Get a bank by numeric group and item tag from the current event. */
    //bank& getBank(const int group, const int item) const {
    //    if (!event_ptr || !file_ptr) throw std::runtime_error("Invalid HipoEvent (no event data)");
//
    //    std::pair<int, int> tag(group, item);
//
    //    // Check if the bank is already in the cache for this event
    //    auto it = tag_bank_cache.find(tag);
    //    if (it != tag_bank_cache.end()) {
    //        return it->second;
    //    }
//
    //    // Not in cache, get template from file and read event data into it
    //    bank new_bank = file_ptr->get_bank_template(group, item);  // Copy the template
    //    event_ptr->read(new_bank);                                 // Fill with current event's data
    //    auto result = tag_bank_cache.emplace(tag, std::move(new_bank));
    //    return result.first->second;
    //}
};

// Updated iterator implementation for hipoeventfile
class hipoeventfile::iterator {
    friend class hipoeventfile;

   public:
    using iterator_category = std::input_iterator_tag;
    using value_type = hipoevent;
    using reference = hipoevent&;
    using pointer = hipoevent*;

   private:
    hipo::reader* reader_ptr;  // pointer to parent file's reader
    hipoeventfile* file_ptr;   // pointer to parent file
    event current_event;       // storage for the current event's raw data
    hipoevent current_wrap;    // wrapper for current_event + file reference
    bool at_end;

    // Private constructor used by HipoEventFile
    iterator(hipo::reader* rdr, hipoeventfile* file)
        : reader_ptr(rdr), file_ptr(file), at_end(false) {
        // Advance to the first event
        if (!reader_ptr || !reader_ptr->next(current_event)) {
            // No event available (empty file or error)
            reader_ptr = nullptr;
            file_ptr = nullptr;
            at_end = true;
        } else {
            // Successfully read first event; initialize wrapper with file reference
            current_wrap = hipoevent(&current_event, file_ptr);
        }
    }

   public:
    /** Default constructor (end iterator). */
    iterator() : reader_ptr(nullptr), file_ptr(nullptr), at_end(true) {
    }

    iterator& operator++() {
        if (!reader_ptr || at_end) {
            // already at end, do nothing
            return *this;
        }
        // Attempt to read the next event
        if (!reader_ptr->next(current_event)) {
            // No more events -> mark as end
            reader_ptr = nullptr;
            file_ptr = nullptr;
            at_end = true;
        } else {
            // Loaded next event successfully, update the wrapper
            current_wrap = hipoevent(&current_event, file_ptr);
        }
        return *this;
    }

    reference operator*() { return current_wrap; }

    pointer operator->() { return &current_wrap; }

    bool operator!=(const iterator& other) const {
        // Two iterators are not equal if one is at end and the other is not.
        const bool thisEnd = at_end;
        const bool otherEnd = other.at_end;
        if (thisEnd && otherEnd) {
            return false;  // both at end (equal)
        }
        return !(thisEnd && otherEnd);  // not equal if one is end and the other is not
    }

    bool operator==(const iterator& other) const { return !(*this != other); }
};

// Updated hipoeventfile begin() and end() methods
inline hipoeventfile::iterator hipoeventfile::begin() {
    reader.rewind();
    return {&reader, this};  // Pass 'this' pointer instead of &dict
}

inline hipoeventfile::iterator hipoeventfile::end() {
    return {};
}
}  // namespace hipo
