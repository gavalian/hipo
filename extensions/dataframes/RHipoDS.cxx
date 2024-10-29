//
// Created by Maurik Holtrop on 7/15/22.
//
/**
 * \file RHipoDS.cxx
 * \brief Implementation of the RHipoDS class.
 * \author Maurik Holtrop
 * \date 2021-07-15
 * \version 1.0
 * \mainpage RHipoDS - A ROOT RDataFrame data source for HIPO files.
 * \section intro_sec Introduction
 * This is a single class package that provides a ROOT RDataFrame data source for HIPO files.
 * All the information is in the RHipoDS() class.
 *
 * \section RHipoDS
 The RHipoDS class implements a Hipo file reader for
 [RDataFrame](https://root.cern/doc/master/classROOT_1_1RDataFrame.html). This is accomplished
 by deriving from the [RDataSource](https://root.cern/doc/master/classROOT_1_1RDF_1_1RDataSource.html)

A RDataFrame that reads from a Hipo file can be constructed using the factory method MakeHipoDataFrame.
### Example
```
 auto df = MakeHipoDataFrame("/path/file*.hipo");
```
MakeHipoDataFrame() and RHipoDS() both accept two parameters:
1. file_pattern: Path to the Hipo file(s). The file specification can be a "glob", i.e. "path/rec_*.hipo"
2. nevt_inspect: (Optional) The number of events to inspect in the file to determine the useful part of the scheme.
   - IF nevt_inspect > 0, then this number of events is inspected to see how many rows each event contains for each
     variable in the schema. If there are zero rows, the variable is not added to the RDataFrame. If there is only
     ever one row, the variable is entered as a scaler, otherwise it is entered as vector.
   - IF nevt_inspect = 0, then do not inspect the file. All variables in the schema are copied to the RDataFrame, so
     many will be empty. Also, all variables will be treated as vectors, even if the file only contains one number,
     so variables like event number (RUN::Config.event) will still be a vector.
   - IF nevt_inspect < 0, then there is no initialization of the RDataFrame variables. You will then need to specify
     in your code which variable you want to read from the file using AddHipoBank(name, nrows).

Alternatively, you can first create an RHipoDS() with a unique_ptr and then move the unique_ptr to
the RDataFrame.

### Example
```
   string files = "path/rec_*.hipo";
   auto hipods = std::make_unique<RHipoDS>(files, 1000);
   // At this point, some specific operations can be done on the hipods directly. Such as:
   // hipods->AddHipoBank("REC::Particle", 2);                // 2 indicates these will be vectors.
   // This is not needed if you inspect the hipo file.
   auto n_events = hipods->GetEntries();                   // Get the number of events in the files.
   auto df = ROOT::RDF:RDataFrame(std::move(hipods));      // Move the pointer to the RDataFrame.
   auto h1 = df.Histo1D("REC::Particle.pid");              // Create a histogram of the particle ID.
   h1->Draw();                                             // Draw the histogram.
```
 */

/** \class RHipoDS
    \ingroup DataFrames
    \brief RDataFrame data source class for reading Hipo files.
 **/

#include "RHipoDS.hxx"

////////////////////////////////////////////////////////////////////////
/// \brief Constructor to create a Hipo RDataSource for RDataFrame.
/// \param[in] fileName Path of the Hipo file, or glob pattern for the files.
/// \param[in] nevt_inspect Number of events to inspect to determine the schema. See Init() for details.
/// \param[in] debug Debug level, 0 is no debug [default], 1 is some debug, 2 is a lot of debug.
RHipoDS::RHipoDS(std::string_view file_pattern, int nevt_inspect, int debug): fDebug(debug) {
   AddFiles(file_pattern);
   Init(nevt_inspect);
}

////////////////////////////////////////////////////////////////////////
/// \brief Constructor to create a Hipo RDataSource for RDataFrame.
/// \param[in] vector<std::string> files, a vector of file names.
/// \param[in] nevt_inspect Number of events to inspect to determine the schema. See Init() for details.
/// \param[in] debug Debug level, 0 is no debug [default], 1 is some debug, 2 is a lot of debug.
RHipoDS::RHipoDS(std::vector<std::string> &files, int nevt_inspect, int debug): fDebug(debug) {

   for(auto &file_name: files) {
      AddFiles(file_name);
   }
   Init(nevt_inspect);
}

////////////////////////////////////////////////////////////////////////
/// \brief Utility function to translate the HIPO Schema names to C++ compatible names.
/// \param[in] name The HIPO Schema name.
/// Example: REC::Particle.px -> REC_Particle_px
std::string RHipoDS::GetTranslatedColumnName(std::string name) const{
   auto pos = name.find("::");
   if (pos != -1) name.replace(pos, 2, "_");
   pos = name.find(".");
   if (pos != -1) name.replace(pos, 1, "_");
   return name;
}

////////////////////////////////////////////////////////////////////////
/// \brief Initialize the class for reading from the HIPO files.
/// \param[in] nevt_inspect, inspect that many events in the file to see what banks the file actually contains.
/// If nevt_inspect < 0, no banks will be added at all, and you have to do so by hand.
/// If nevt_inspect = 0, all the banks are added as vectors, whether they are in the file or not (so you get lots of empties).
/// If nevt_inspect > 0 then that many events are inspected.
void RHipoDS::Init(int nevt_inspect) {

   if(fHipoFiles.empty()) return;
   fHipoReader.setTags(0);
   fHipoReader.open(fHipoFiles[0].c_str());
   fHipoCurrentMaxEvent = fHipoReader.getEntries();
   fHipoCurrentMaxRecord = fHipoReader.getNRecords();

   if(fDebug>0) std::cout << "Opened Hipo file " << fHipoFiles[0] << " with " << fHipoCurrentMaxEvent << " events. \n";

   fHipoReader.readDictionary(fHipoDict);

   // Step 2: Read the schema and construct a column map.
   fAllBankNames = fHipoDict.getSchemaList();

   hipo::event event;
   if(nevt_inspect < 0) return;
   int counter = 0;

   if( nevt_inspect == 0){
      for (int ib = 0; ib < fAllBankNames.size(); ++ib) {
         AddHipoBank(fAllBankNames[ib], 2); // We don't know anything about the banks, so they are all vectors???
      }
   }else {
      std::map<std::string, int> bank_column_depth;

      while (fHipoReader.next() == true && counter < nevt_inspect) { // Only inspect the first N events?
         fHipoReader.read(event);
         for (int ib = 0; ib < fAllBankNames.size(); ++ib) {
            auto sch = fHipoDict.getSchema(fAllBankNames[ib].c_str());
            hipo::bank this_bank(sch);
            event.getStructure(this_bank);
            int nrows = this_bank.getRows();

            auto bcd_itt = bank_column_depth.find(fAllBankNames[ib]);
            if( bcd_itt == bank_column_depth.end() ){
               bank_column_depth[fAllBankNames[ib]] = nrows;
            }else{
               if( nrows > bcd_itt->second )
                  bcd_itt->second = nrows;
            }

            if (nrows > 0) {
               // Check if we stored this bank already. If not, store it, and parse it.
               AddHipoBank(fAllBankNames[ib], nrows);
            }
         }
         counter++;
      }
      if(fDebug>0) {
         std::cout << "Hipo bank profile for " << fHipoFiles[0] << std::endl;
         for (auto &itt: bank_column_depth) {
            if (itt.second > 0) printf("%35s : %4d \n", itt.first.c_str(), itt.second);
         }
      }
   }
   fHipoReader.gotoEvent(0);  // Wind back to first event.
}

////////////////////////////////////////////////////////////////////////
/// \brief Add a file, or files, according to the file_glob pattern.
/// \param[in] file_glob A glob pattern for the files to add.
int  RHipoDS::AddFiles(std::string_view file_glob) {

   glob_t glob_result;
   memset(&glob_result, 0, sizeof(glob_result));

   int return_value = glob(file_glob.data(), GLOB_TILDE | GLOB_BRACE, NULL, &glob_result);
   if(return_value != 0) {
      globfree(&glob_result);
      if( return_value == GLOB_NOMATCH) return 0;
      std::stringstream ss;
      ss << "glob() failed with return_value " << return_value << std::endl;
      throw std::runtime_error(ss.str());
   }

   int n_files = glob_result.gl_pathc;
   for(size_t i = 0; i < n_files; ++i) {
      fHipoFiles.push_back(std::string(glob_result.gl_pathv[i]));
   }

   // cleanup
   globfree(&glob_result);
   return n_files;
}

////////////////////////////////////////////////////////////////////////
/// \brief Add a HIPO bank to the list of banks to read. This is called by Init() to add all the banks in the file.
/// \param[in] bank_name The name of the HIPO bank to add. EG: REC::Particle
/// \param[in] nrows The number of rows in the bank. If nrows > 1, then the bank is added as a vector.
/// \return 1 if the bank was added, 0 if nrows = 0.
int RHipoDS::AddHipoBank(std::string name, int nrows) {
   // Add a bank name (eg. REC::Track) to the set of banks to take into account.
   // If nrows == 0, no action is taken.
   // Set nrows = 1 if it is a scaler bank, nrows > 1 for a vector bank.
   // If nrows > 1 for a bank previously entered as scaler, then change it to a vector.
   //
   if(nrows == 0 ) return(0);
   auto bank_it = fBanksToIndex.find(name);
   if( bank_it == fBanksToIndex.end()) {
      int bank_index = fBanks.size();
      fBanksToIndex[name] = bank_index;

      auto sch = fHipoDict.getSchema(name.c_str());
      hipo::bank this_bank(sch);

      fBanks.emplace_back(this_bank);   // Put the bank in the bank store.

      for (int i = 0; i < sch.getEntries(); ++i) {
         std::string full_name = name + "." + sch.getEntryName(i);

         fAllColumnsPreTranslated.emplace_back(full_name);
         if(fColumnNameTranslation) full_name = GetTranslatedColumnName(full_name);

         fColumnNameToIndex[full_name] = fAllColumns.size();
         fAllColumns.emplace_back(full_name);
         fColumnBank.push_back(bank_index);
         fColumnItem.push_back(i);
         int i_type = sch.getEntryType(i);
         if(i_type < 0 || i_type >= fgCollTypeNumToString.size()){
            std::string err = "RHipoDS::RHipoDS() -- Error -- Type out of range for " + full_name;
            err += " type num = " + std::to_string(i_type);
            throw std::runtime_error(err);
         }
         fColumnType.push_back(i_type);

         if( nrows > 1) fColumnTypeIsVector.push_back(true);
         else fColumnTypeIsVector.push_back(false);

         if(fDebug> 1){
            printf("%30s  bank id: %3d item: %2d  %s \n", full_name.c_str(), bank_index, i,
                   fColumnTypeIsVector.back() ? "vector":"scaler" );
         }
      }
   } else if(nrows > 1){ // We have seen this bank before. If it was entered as a scalar, check if it should be a vector
      auto bank = fBanks[bank_it->second];
      auto sch = bank.getSchema();
      std::string full_name = name + "." + sch.getEntryName(0);  // If one is vector, all are vector.
      if(fColumnNameTranslation) full_name = GetTranslatedColumnName(full_name);

      int idx = fColumnNameToIndex.at(full_name);
      if( !fColumnTypeIsVector[idx]){
         if(fDebug>1) std::cout << "The column " << full_name << " is actually a vector!\n";
         fColumnTypeIsVector[idx] = true;
         for(int i=1; i< sch.getEntries(); ++i){  // So flip all the others too.
            full_name = name + "." + sch.getEntryName(i);
            if(fColumnNameTranslation) full_name = GetTranslatedColumnName(full_name);
            idx = fColumnNameToIndex.at(full_name);
            fColumnTypeIsVector[idx] = true;
         }
      }
   }
   return 1;
}

////////////////////////////////////////////////////////////////////////
/// \brief Identify the class.
/// \return A string identifying the class.
std::string RHipoDS::AsString()
{
   return "Hipo data source";
}

////////////////////////////////////////////////////////////////////////
/// \brief Get the number of events in the file.
/// \param[in] current_file_only If true, only count the events in the current file.
/// \return The number of events in the file.
unsigned long RHipoDS::GetEntries(bool current_file_only) {
   // Get the total number of entries in the data set.
   if(current_file_only) return (unsigned long)fHipoReader.getEntries();

   int save_hipo_file_index = -1;
   if(fHipoReader.is_open()) save_hipo_file_index = fHipoFiles_index;

   unsigned long total_events = 0;
   for(auto &file: fHipoFiles){
      fHipoReader.open(file.c_str());
      total_events += fHipoReader.getEntries();
   }
   if(save_hipo_file_index>=0) fHipoReader.open(fHipoFiles[save_hipo_file_index].c_str());

   return total_events;
}

////////////////////////////////////////////////////////////////////////
/// \brief Return the number of slots. Used internally for RDataFrame.
/// \return The number of slots.
void RHipoDS::SetNSlots(unsigned int nSlots) {
   fNSlots = nSlots;
}

////////////////////////////////////////////////////////////////////////
/// \brief Return a vector<string> of the column names that are known to this data source.
/// \return A vector<string> of the column names.
const std::vector<std::string> &RHipoDS::GetColumnNames() const{
// Return the names of ALL the active columns.
   return fAllColumns;
}

// clang-format off
////////////////////////////////////////////////////////////////////////
/// \brief Return ranges of entries to distribute to tasks.
/// This is used internally for the RDataFrame.
/// They are required to be contiguous intervals with no entries skipped. Supposing a dataset with nEntries, the
/// intervals must start at 0 and end at nEntries, e.g. [0-5],[5-10] for 10 entries.
/// This function will be invoked repeatedly by RDataFrame as it needs additional entries to process.
/// The same entry range should not be returned more than once.
/// Returning an empty collection of ranges signals to RDataFrame that the processing can stop.
/// \return Return a list of ranges that can be processed in parallel.
// clang-format on
std::vector<std::pair<ULong64_t, ULong64_t>> RHipoDS::GetEntryRanges(){
// Note:
// Because this is HIPO, this requires *reading* the next Record (or Block) from file.

   std::vector<std::pair<ULong64_t, ULong64_t>> entryRanges;
   if(fHipoCurrentRecord >= fHipoCurrentMaxRecord){
      if( ++fHipoFiles_index >= fHipoFiles.size()) return entryRanges;
      fHipoReader.open(fHipoFiles[fHipoFiles_index].c_str());
      fHipoCurrentMaxEvent = fHipoReader.getEntries();
      fHipoCurrentMaxRecord = fHipoReader.getNRecords();
      fHipoCurrentRecord=0;
   }

   fHipoReader.loadRecord(fHipoRecord, fHipoCurrentRecord);
   fHipoCurrentRecord++;

   fHipoCurrentMaxEvent = fHipoRecord.getEventCount();

   // Divide up the event in the record over the slots.
   unsigned int chunkSize = fHipoCurrentMaxEvent / fNSlots;
   unsigned int remainder =  ( fNSlots == 1? 0 : fHipoReader.getEntries() % fNSlots );

   unsigned int start = 0;
   unsigned int end = start;

   for(unsigned int i=0; i< fNSlots; ++i){
      start = end;
      end += chunkSize;
      entryRanges.emplace_back(start, end);
   }
   entryRanges.back().second += remainder;  // Last few events are added to the last slot.
   return entryRanges;
}

////////////////////////////////////////////////////////////////////////
/// \brief Check if column exists in the data source.
/// \param[in] colName The name of the column to check.
/// \return True if the column exists.
/// This is used internally for the RDataFrame
bool RHipoDS::HasColumn(std::string_view colName) const {
   // See if colName is in fAllColumns
   return fColumnNameToIndex.count(colName.data()) == 1;
};

////////////////////////////////////////////////////////////////////////
/// \brief Get the index number of the column.
/// \param[in] colName The name of the column.
/// \return The index number of the column.
/// This is used internally for the RDataFrame
int RHipoDS::GetColNum(std::string_view colName) const{
   // Get the C++ index for the named column.
   std::string name = colName.data();
   int iloc = fColumnNameToIndex.at(name);
   return iloc;
}

////////////////////////////////////////////////////////////////////////
/// \brief Get the data type of the column.
/// \param[in] colName The name of the column.
/// \return The type number of the column.
/// This is used internally for the RDataFrame
int RHipoDS::GetTypeNum(std::string_view colName) const{
   // Get type number of column name.
   int iloc = GetColNum(colName);
   return fColumnType[iloc];
}

////////////////////////////////////////////////////////////////////////
/// \brief Get the data type name of the column.
/// \param[in] column_index The index of the column.
/// \return The type name of the column as a string.
/// This is used internally for the RDataFrame
std::string RHipoDS::GetTypeName(int column_index) const {
   // Get the type name from the index.
   if( fColumnTypeIsVector[column_index] ){
      return "vector<" + fgCollTypeNumToString[fColumnType[column_index]] + ">";
   }else
      return fgCollTypeNumToString[fColumnType[column_index]];
}

////////////////////////////////////////////////////////////////////////
/// \brief Get the data type name of the column.
/// \param[in] colName The name of the column.
/// \return The type name of the column as a string.
/// This is used internally for the RDataFrame
std::string RHipoDS::GetTypeName(std::string_view colName) const {
   // Get the C++ type for the column.
   int iloc = GetColNum(colName);
   return GetTypeName(iloc);
}

////////////////////////////////////////////////////////////////////////
/// \brief Clear all the DATA *VECTORS*
/// \param[in] slot The slot number. See RDataFrame docs for meaning of slot.
/// This is used internally for the RDataFrame
void RHipoDS::ClearData(int slot){
   // We need to clear the vector data only
   // If slot < 0 then clear all slots. Notice that while multi-threading, that can wreak havoc.
   if(slot == -1){
      for(int i=0; i<fNSlots; ++i)
         ClearData(i);  // Recurse.
      return;
   }

//   for(auto &vecvec: fVecCharData){
//      vecvec[slot].clear();
//   }
   for(auto &vecvec: fVecShortData){
      vecvec[slot].clear();
   }
   for(auto &vecvec: fVecIntData){
      vecvec[slot].clear();
   }
   for(auto &vecvec: fVecLongData){
      vecvec[slot].clear();
   }
   for(auto &vecvec: fVecFloatData){
      vecvec[slot].clear();
   }
   for(auto &vecvec: fVecDoubleData){
      vecvec[slot].clear();
   }
}


////////////////////////////////////////////////////////////////////////
/// \brief Advance the "cursors" returned by GetColumnReaders to the selected entry for a particular slot.
/// \param[in] slot The data processing slot that needs to be considered
/// \param[in] entry The entry which needs to be pointed to by the reader pointers
/// Slots are adopted to accommodate parallel data processing.
/// Different workers will loop over different ranges and
/// will be labelled by different "slot" values.
/// Returns *true* if the entry has to be processed, *false* otherwise.
// clang-format on
bool RHipoDS::SetEntry(unsigned int slot, ULong64_t entry){
   // Set the Hipo reader to the correct entry, or record, in the file.

   ClearData(slot);

   if(entry > fHipoCurrentMaxEvent) return false;

   if(fDebug>1) std::cout << "SetEntry: slot: " << slot << " entry: " << entry
            << " current record: " << fHipoCurrentRecord << std::endl;

//   fHipoReader.gotoEvent(entry); // Issue: this may jump around a Block border, slowing the reading.
//   fHipoReader.read(fHipoEvent);

   if(fHipoReadOnlyPhysicsEvents){

   }

   std::vector<int> banks_seen;
   for(int active_index=0; active_index < fActiveColumns.size(); ++active_index){
      int col_index = fActiveColumns[active_index];
      int bank_index = fColumnBank[col_index];
// #define DO_BANK_SEARCH
#ifdef DO_BANK_SEARCH
      // 16.62698 16.81025 16.77813 17.15750
      if( !std::binary_search(banks_seen.begin(), banks_seen.end(), bank_index) ){
         // Bank has not already been processed.
         banks_seen.push_back(bank_index);
         std::sort(banks_seen.begin(), banks_seen.end());
//         fHipoEvent.getStructure(fBanks[bank_index]);     // Fill the bank with data.
         fHipoRecord.read(fBanks[bank_index], entry);
      }
#else
      // Reading the bank each time is faster? - seems so, but marginal difference.
      // 16.76788 16.71784 16.64359 16.63519
      fHipoRecord.read(fBanks[bank_index], entry);
#endif
      // So here, we can assume that the bank has the data we are interested in.
      int nrows = fBanks[bank_index].getRows();
      int data_index = fIndexToData[active_index];
      char cnum  = 0;
      short snum = 0;
      if( fColumnTypeIsVector[col_index] ){
         for(int irow=0; irow < nrows; ++irow) {
            switch (fColumnType.at(col_index)) {
               case 1: // vector<char>
                  cnum = fBanks[bank_index].getByte(fColumnItem[col_index], irow);
                  fVecShortData.at(data_index).at(slot).push_back( (short) cnum );
//                   short snum = fBanks[bank_index].getByte(fColumnItem[col_index], irow);
//                   fVecCharData.at(data_index).at(slot).push_back(cnum);
                  break;
               case 2: //
                  fVecShortData.at(data_index).at(slot).push_back( (short) fBanks[bank_index].getShort(fColumnItem[col_index], irow));
                  break;
               case 3: // vector<int>
                  fVecIntData.at(data_index).at(slot).push_back(fBanks[bank_index].getInt(fColumnItem[col_index], irow));
                  break;
               case 4: // vector<float>
                  fVecFloatData.at(data_index).at(slot).push_back(fBanks[bank_index].getFloat(fColumnItem[col_index], irow));
                  break;
               case 5: // vector<double>
                  fVecDoubleData.at(data_index).at(slot).push_back(fBanks[bank_index].getDouble(fColumnItem[col_index], irow));
                  break;
               case 6:
               case 8:
                  fVecLongData.at(data_index).at(slot).push_back(fBanks[bank_index].getLong(fColumnItem[col_index], irow));
                  break;
               default:
                  std::cout << "We got a column type issue. ColIndex: " << col_index << " ColType:"
                            << fColumnType.at(col_index);
            }
         }
      }else{
         switch (fColumnType.at(col_index)) {
            case 1: // char
            case 2: // short
               if(nrows>0){
                  fShortData.at(data_index).at(slot) = fBanks[bank_index].getShort(fColumnItem[col_index], 0);
               } else
                  fShortData.at(data_index).at(slot) = 0;
               break;
            case 3: // int
               if(nrows>0){
                  fIntData.at(data_index).at(slot) = fBanks[bank_index].getInt(fColumnItem[col_index], 0);
               } else
                  fIntData.at(data_index).at(slot) = 0;
               break;
            case 4: // float
               if(nrows>0){
                  fFloatData.at(data_index).at(slot) = fBanks[bank_index].getFloat(fColumnItem[col_index], 0);
               } else
                  fFloatData.at(data_index).at(slot) = 0.;
               break;
            case 5: // double
               if(nrows>0){
                  fDoubleData.at(data_index).at(slot) = fBanks[bank_index].getDouble(fColumnItem[col_index], 0);
               } else
                  fDoubleData.at(data_index).at(slot) = 0.;
               break;
            case 6:
            case 8:
               if(nrows>0){
                  fLongData.at(data_index).at(slot) = fBanks[bank_index].getLong(fColumnItem[col_index], 0);
               } else
                  fLongData.at(data_index).at(slot) = 0.;
               break;
            default:
               std::cout << "We got a column type issue. \n";
               std::cout << "Column: " << fAllColumns[col_index] << "ColIndex: " << col_index << " ColType:"
                         << fColumnType.at(col_index);
         }
      }
   }

   return true;
}

////////////////////////////////////////////////////////////////////////
/// \brief From RDataSource.cxx:  type-erased vector of pointers to pointers to column values - one per slot
/// \param[in] col_name The name of the column to be read.
/// \param[in] ti  type info for the column.
/// Inferred documentation:
/// GetColumnReadersImpl will be called for each variable in the user code with the column name of that variable.
/// It must return a vector<void *> with one entry for each slot.
/// Each entry in the vector<void *> contains a pointer, which points to another pointer, which then
/// points to the value, (or to the vector of values)
/// The ti parameter can be used to check if there is consistency. This is not used because it is not needed
/// and such checking will only slow things down.
std::vector<void *> RHipoDS::GetColumnReadersImpl(std::string_view col_name, const std::type_info &ti){
   //
   // Verify that the column pointed to by col_name has the correct type, as in ti
   const auto colType = GetTypeNum(col_name);
   if(fDebug>1) std::cout << "GetColumnReadersImpl:: " << col_name << ": colType:" << colType << " type name:" << ti.name()  << std::endl;

   std::vector<void *> ret(fNSlots);

   std::string column_name = col_name.data();
   int col_index = fColumnNameToIndex.at(column_name);
   if(fDebug>1) std::cout << "Index " << col_index << " for column " << column_name.data() << std::endl;

   auto fActiveColumn_itt = std::find(fActiveColumns.begin(), fActiveColumns.end(), col_index);
   if(  fActiveColumn_itt == fActiveColumns.end()) {
   // Has this column already been setup for reading? If not, set it up.
   // Yup, this is one big data allocation clusterfuck.
      fActiveColumns.push_back(col_index);
      if( fColumnTypeIsVector.at(col_index)) {   // VECTORS
         switch (fColumnType.at(col_index)) {
            case 1: // vector<char> is up-cast to vector<short>
            case 2: // vector<short>
               fVecShortData.emplace_back( fNSlots, std::vector<short>(fNColumnDepth));
               fIndexToData.push_back((int)fVecShortData.size()-1);
               fColumnPointers.emplace_back(fNSlots);
               for(int i=0; i<fNSlots; ++i){
                  fColumnPointers.back()[i] = (void *)&fVecShortData.back()[i];
                  ret[i] = &fColumnPointers.back()[i];
               }
               break;
            case 3: // vector<int>
               fVecIntData.emplace_back( fNSlots, std::vector<int>(fNColumnDepth));
               fIndexToData.push_back((int)fVecIntData.size()-1);
               fColumnPointers.emplace_back(fNSlots);
               for(int i=0; i<fNSlots; ++i){
                  fColumnPointers.back()[i] = (void *)&fVecIntData.back()[i];
                  ret[i] = &fColumnPointers.back()[i];
               }
               break;
            case 4: // vector<float>
               fVecFloatData.emplace_back( fNSlots, std::vector<float>(fNColumnDepth));
               fIndexToData.push_back((int)fVecFloatData.size()-1);
               fColumnPointers.emplace_back(fNSlots);
               for(int i=0; i<fNSlots; ++i){
                  fColumnPointers.back()[i] = (void *)&fVecFloatData.back()[i];
                  ret[i] = &fColumnPointers.back()[i];
               }
               break;
            case 5: // vector<double>
               fVecDoubleData.emplace_back( fNSlots, std::vector<double>(fNColumnDepth));
               fIndexToData.push_back((int)fVecDoubleData.size()-1);
               fColumnPointers.emplace_back(fNSlots);
               for(int i=0; i<fNSlots; ++i){
                  fColumnPointers.back()[i] = (void *)&fVecDoubleData.back()[i];
                  ret[i] = &fColumnPointers.back()[i];
               }
               break;
            case 6:
            case 8:
               fVecLongData.emplace_back( fNSlots, std::vector<long>(fNColumnDepth));
               fIndexToData.push_back((int)fVecLongData.size()-1);
               fColumnPointers.emplace_back(fNSlots);
               for(int i=0; i<fNSlots; ++i){
                  fColumnPointers.back()[i] = (void *)&fVecLongData.back()[i];
                  ret[i] = &fColumnPointers.back()[i];
               }
               break;
            default:
               std::cout << "We got a column type issue. ColIndex: " << col_index << " ColType:"
                         << fColumnType.at(col_index);
         }
      }else{   // SCALARS = NOT VECTORS
            switch (fColumnType.at(col_index)) {
               case 1:  // char
               case 2:  // short
                  fShortData.emplace_back(fNSlots);
                  fIndexToData.push_back((int)fShortData.size()-1);
                  fColumnPointers.emplace_back(fNSlots);
                  for(int i=0; i<fNSlots; ++i){
                     fColumnPointers.back()[i] = (void *)&fShortData.back()[i];
                     ret[i] = &fColumnPointers.back()[i];
                  }
                  break;
               case 3:  // int
                  fIntData.emplace_back(fNSlots);
                  fIndexToData.push_back((int)fIntData.size()-1);
                  fColumnPointers.emplace_back(fNSlots);
                  for(int i=0; i<fNSlots; ++i){
                     fColumnPointers.back()[i] = (void *)&fIntData.back()[i];
                     ret[i] = &fColumnPointers.back()[i];
                  }
                  break;
               case 4:  // float
                  fFloatData.emplace_back(fNSlots);
                  fIndexToData.push_back((int)fFloatData.size()-1);
                  fColumnPointers.emplace_back(fNSlots);
                  for(int i=0; i<fNSlots; ++i){
                     fColumnPointers.back()[i] = (void *)&fFloatData.back()[i];
                     ret[i] = &fColumnPointers.back()[i];
                  }
                  break;
               case 5: // double
                  fDoubleData.emplace_back(fNSlots);
                  fIndexToData.push_back((int)fDoubleData.size()-1);
                  fColumnPointers.emplace_back(fNSlots);
                  for(int i=0; i<fNSlots; ++i){
                     fColumnPointers.back()[i] = (void *)&fDoubleData.back()[i];
                     ret[i] = &fColumnPointers.back()[i];
                  }
                  break;
               case 6: // long
               case 8:
                  fLongData.emplace_back(fNSlots);
                  fIndexToData.push_back((int)fLongData.size()-1);
                  fColumnPointers.emplace_back(fNSlots);
                  for(int i=0; i<fNSlots; ++i){
                     fColumnPointers.back()[i] = (void *)&fLongData.back()[i];
                     ret[i] = &fColumnPointers.back()[i];
                  }
                  break;
               default:
                  std::cout << "We got a column type issue. ColIndex: " << col_index << " ColType:"
                            << fColumnType.at(col_index);
         }
      }
   }else{  // WOA, we told you about this variable already. Okay, older ROOT versions ask again, so here it is.
      int active_index = std::distance(fActiveColumns.begin(), fActiveColumn_itt);
      for(int i=0; i< fNSlots; ++i) {
         ret[i] = &fColumnPointers[active_index][i];
      }
   }

   return ret;
}

ClassImp(RHipoDS);

///////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Factory method to create a Hipo RDataFrame.
/// \param[in] fileName Path of the Hipo file.
/// \param[in] nevt_inspect Number of events to inspect to determine the schema.
/// This is a function to quickly create an RDataFrame from a Hipo file.
RDataFrame MakeHipoDataFrame(std::string_view fileName, int n_inspect){
   RDataFrame tdf(std::make_unique<RHipoDS>(fileName, n_inspect));
   return tdf;
}

