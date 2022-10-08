//
// Created by Maurik Holtrop on 7/15/22.
//
/** \class ROOT::RDF::RCHipoDS
    \ingroup dataframe
    \brief RDataFrame data source class for reading Hipo files.

The RHipoDS class implements a Hipo file reader for RDataFrame.

A RDataFrame that reads from a Hipo file can be constructed using the factory method
ROOT::RDF::MakeHipoDataFrame, which accepts one parameter:
1. Path to the Hipo file.
**/

#include "RHipoDS.hxx"

////////////////////////////////////////////////////////////////////////
/// Constructor to create a Hipo RDataSource for RDataFrame.
/// \param[in] fileName Path of the Hipo file.
RHipoDS::RHipoDS(std::string_view file_pattern, int nevt_inspect, int debug): fDebug(debug) {
   AddFiles(file_pattern);
   Init(nevt_inspect);
}

RHipoDS::RHipoDS(std::vector<std::string> &files, int nevt_inspect, int debug): fDebug(debug) {

   for(auto &file_name: files) {
      AddFiles(file_name);
   }
   Init(nevt_inspect);
}

std::string RHipoDS::GetTranslatedColumnName(std::string name) const{
   auto pos = name.find("::");
   if (pos != -1) name.replace(pos, 2, "_");
   pos = name.find(".");
   if (pos != -1) name.replace(pos, 1, "_");
   return name;
}

/// Initialize the class for reading from the HIPO files.
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
   if(nevt_inspect < 0) nevt_inspect = 1<<30; // MAX INT
   int counter = 0;

   if( nevt_inspect == 0){
      for (int ib = 0; ib < fAllBankNames.size(); ++ib) {
         auto sch = fHipoDict.getSchema(fAllBankNames[ib].c_str());
         hipo::bank this_bank(sch);
         event.getStructure(this_bank);
         auto bank_it = fBanksToIndex.find(fAllBankNames[ib]);
         if( bank_it == fBanksToIndex.end()) {
            int bank_index = fBanks.size();
            fBanksToIndex[fAllBankNames[ib]] = bank_index;
            fBanks.emplace_back(this_bank);   // Put the bank in the bank store.

            for (int i = 0; i < sch.getEntries(); ++i) {

               std::string full_name = fAllBankNames[ib] + "." + sch.getEntryName(i);

               fAllColumnsPreTranslated.emplace_back(full_name);
               if(fColumnNameTranslation) full_name = GetTranslatedColumnName(full_name);

               fColumnNameToIndex[full_name] = fAllColumns.size();

               fAllColumns.emplace_back(full_name);
               fColumnBank.push_back(bank_index);
               fColumnItem.push_back(i);
               int i_type = sch.getEntryType(i);
               if (i_type < 0 || i_type >= fgCollTypeNumToString.size()) {
                  std::string err = "RHipoDS::RHipoDS() -- Error -- Type out of range for " + full_name;
                  err += " type num = " + std::to_string(i_type);
                  throw std::runtime_error(err);
               }
               fColumnType.push_back(i_type);
               fColumnTypeIsVector.push_back(true);

               if (fDebug > 1) {
                  printf("%30s  bank id: %3d item: %2d  %s \n", full_name.c_str(), bank_index, i,
                         fColumnTypeIsVector.back() ? "vector" : "scaler");
               }
            }
         }
      }
   }else {
      std::map<std::string, int> bank_column_depth;

      while (fHipoReader.next() == true && counter < nevt_inspect) { // Only inspect the first 100 events?
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
               auto bank_it = fBanksToIndex.find(fAllBankNames[ib]);
               if( bank_it == fBanksToIndex.end()) {
                  int bank_index = fBanks.size();
                  fBanksToIndex[fAllBankNames[ib]] = bank_index;
                  fBanks.emplace_back(this_bank);   // Put the bank in the bank store.

                  for (int i = 0; i < sch.getEntries(); ++i) {
                     std::string full_name = fAllBankNames[ib] + "." + sch.getEntryName(i);

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
                  std::string full_name = fAllBankNames[ib] + "." + sch.getEntryName(0);  // If one is vector, all are vector.
                  if(fColumnNameTranslation) full_name = GetTranslatedColumnName(full_name);

                  int idx = fColumnNameToIndex.at(full_name);
                  if( !fColumnTypeIsVector[idx]){
                     if(fDebug>1) std::cout << "The column " << full_name << " is actually a vector!\n";
                     fColumnTypeIsVector[idx] = true;
                     for(int i=1; i< sch.getEntries(); ++i){  // So flip all the others too.
                        full_name = fAllBankNames[ib] + "." + sch.getEntryName(i);
                        if(fColumnNameTranslation) full_name = GetTranslatedColumnName(full_name);
                        idx = fColumnNameToIndex.at(full_name);
                        fColumnTypeIsVector[idx] = true;
                     }
                  }
               }
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

/// Add a file, or files, according to the file_glob pattern.
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

std::string RHipoDS::AsString()
{
   return "Hipo data source";
}

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

void RHipoDS::SetNSlots(unsigned int nSlots) {
   fNSlots = nSlots;
}

const std::vector<std::string> &RHipoDS::GetColumnNames() const{
// Return the names of ALL the active columns.
   return fAllColumns;
}

// clang-format off
/// \brief Return ranges of entries to distribute to tasks.
/// They are required to be contiguous intervals with no entries skipped. Supposing a dataset with nEntries, the
/// intervals must start at 0 and end at nEntries, e.g. [0-5],[5-10] for 10 entries.
/// This function will be invoked repeatedly by RDataFrame as it needs additional entries to process.
/// The same entry range should not be returned more than once.
/// Returning an empty collection of ranges signals to RDataFrame that the processing can stop.
// clang-format on
std::vector<std::pair<ULong64_t, ULong64_t>> RHipoDS::GetEntryRanges(){
// Return a list of ranges that can be processed in parallel.
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

bool RHipoDS::HasColumn(std::string_view colName) const {
   // See if colName is in fAllColumns
   return fColumnNameToIndex.count(colName.data()) == 1;
};

int RHipoDS::GetColNum(std::string_view colName) const{
   // Get the C++ index for the named column.
   std::string name = colName.data();
   int iloc = fColumnNameToIndex.at(name);
   return iloc;
}

int RHipoDS::GetTypeNum(std::string_view colName) const{
   // Get type number of column name.
   int iloc = GetColNum(colName);
   return fColumnType[iloc];
}

std::string RHipoDS::GetTypeName(int column_index) const {
   // Get the type name from the index.
   if( fColumnTypeIsVector[column_index] ){
      return "vector<" + fgCollTypeNumToString[fColumnType[column_index]] + ">";
   }else
      return fgCollTypeNumToString[fColumnType[column_index]];
}

std::string RHipoDS::GetTypeName(std::string_view colName) const {
   // Get the C++ type for the column.
   int iloc = GetColNum(colName);
   return GetTypeName(iloc);
}

// clang-format off
/// Clear all the DATA *VECTORS*
void RHipoDS::ClearData(int slot){
   // We need to clear the vector data only
   // If slot < 0 then clear all slots. Notice that while multi-threading, that can wreak havoc.
   if(slot == -1){
      for(int i=0; i<fNSlots; ++i)
         ClearData(i);  // Recurse.
      return;
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

// clang-format off
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
      if( fColumnTypeIsVector[col_index] ){
         for(int irow=0; irow < nrows; ++irow) {
            switch (fColumnType.at(col_index)) {
               case 1: // vector<char>
               case 2: // vector<short>
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

/// From RDataSource.cxx:  type-erased vector of pointers to pointers to column values - one per slot
/// Inferred documentation:
/// GetColumnReadersImpl will be called for each variable in the user code with the column name of that variable.
/// It must return a vector<void *> with one entry for each slot.
/// Each entry in the vector<void *> contains a pointer, which points to another pointer, which then
/// points to the value, (or to the vector of values?)
///
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
            case 1: // vector<char>
            case 2: // vector<short>
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

RDataFrame MakeHipoDataFrame(std::string_view fileName, int n_inspect){
   RDataFrame tdf(std::make_unique<RHipoDS>(fileName, n_inspect));
   return tdf;
}

