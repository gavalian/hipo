#pragma once

#include <bank.h>
#include <dictionary.h>
#include <event.h>
#include <reader.h>
#include <iterator>
#include <stdexcept>
#include <string>

namespace hipo {
    /**
         * Iterable file of HIPO events. Provides begin()/end() for range-based loops.
         */
    class hipoeventfile {
    public:
        /** Iterator type for HipoEventFile (input iterator). */
        class iterator; // defined below
        using const_iterator = iterator;

    private:
        hipo::reader reader;
        dictionary dict;

    public:
        /** Open a HIPO file and read its dictionary. */
        explicit hipoeventfile(const std::string &filename) {
            reader.open(filename.c_str());
            reader.readDictionary(dict);
        }

        /** Iterator pointing to the first event in the file. */
        iterator begin();

        /** End iterator (past-the-last event). */
        iterator end();
    };

    /**
         * Lightweight wrapper for a HIPO event, providing access to banks by name or tag.
         * Holds a pointer to a hipo::event and the hipo::dictionary for schema lookup.
         */
    class hipoevent {
        friend class hipoeventfile;
        friend class hipoeventfileiterator;

    private:
        event *event_ptr;
        dictionary *dict_ptr;

        // Private constructor used by iterator to wrap current event
        hipoevent(event *ev, dictionary *dict)
            : event_ptr(ev), dict_ptr(dict) {
        }

    public:
        hipoevent() : event_ptr(nullptr), dict_ptr(nullptr) {
        }

        /** Get a bank by schema name (e.g., "RUN::config") from the current event. */
        bank get_bank(const std::string &bankName) const {
            if (!event_ptr || !dict_ptr) throw std::runtime_error("Invalid HipoEvent (no event data)");
            if (!dict_ptr->hasSchema(bankName.c_str())) throw std::runtime_error("Schema not found for bank: " + bankName);

            bank bank(dict_ptr->getSchema(bankName.c_str()));
            event_ptr->read(bank);
            return bank;
        }

        /** Get a bank by numeric group and item tag from the current event. */
        bank getBank(const int group, const int item) const {
            if (!event_ptr) throw std::runtime_error("Invalid HipoEvent (no event data)");

            if (dict_ptr) {
                for (const std::string &name: dict_ptr->getSchemaList()) {
                    schema &schema = dict_ptr->getSchema(name.c_str());
                    if (schema.getGroup() == group && schema.getItem() == item) {
                        bank bank(schema);
                        event_ptr->read(bank);
                        return bank;
                    }
                }
            }
            throw std::runtime_error("Schema for group=" + std::to_string(group) + ", item=" + std::to_string(item) + " not found");
        }
    };


    /**
         * Input iterator over HIPO events. Each iterator dereferences to a HipoEvent,
         * which gives access to the current event's banks.
         */
    class hipoeventfile::iterator {
        friend class hipoeventfile;

    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = hipoevent;
        using reference = hipoevent &;
        using pointer = hipoevent *;

    private:
        hipo::reader *reader_ptr; // pointer to parent file's reader
        dictionary *dict_ptr; // pointer to parent file's dictionary
        event current_event; // storage for the current event's raw data
        hipoevent current_wrap; // wrapper for current_event + dictionary
        bool at_end;

        // Private constructor used by HipoEventFile
        iterator(hipo::reader *rdr, dictionary *dict);

    public:
        /** Default constructor (end iterator). */
        iterator() : reader_ptr(nullptr), dict_ptr(nullptr), at_end(true) {
        }

        iterator &operator++();

        reference operator*() { return current_wrap; }

        pointer operator->() { return &current_wrap; }

        bool operator!=(const iterator &other) const;

        bool operator==(const iterator &other) const { return !(*this != other); }
    };
} // namespace hipo
