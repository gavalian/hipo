//
// Created by Maurik Holtrop on 7/15/22.
//

#ifndef HIPODATAFRAME_RHIPODS_HXX
#define HIPODATAFRAME_RHIPODS_HXX

#include "TROOT.h"
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDataSource.hxx"

#include "reader.h"

#include <typeinfo>
#include <vector>
#include <string>

// As of now, I am not "namespacing" this for simplicity.
using namespace ROOT;
using namespace ROOT::RDF;

class RHipoDS : public RDataSource {
   //private:
public:
   using ColType_t = char;  // Either B, S, I, F, D or L.
//   const std::map<ColType_t , std::string> fgColTypeMap{
//      {'B', "char"},  // 0  -- In Java a "byte" is a "char"
//      {'S', "short"}, // 1
//      {'I', "int"},   // 2
//      {'F', "float"}, // 3
//      {'D', "double"},// 4
//      {'L', "long"}   // 5
//   };
   const std::vector<std::string> fgCollTypeNumToString{ // ORDER is important here. C++ so go +1
      "zero", "char", "short", "int", "float", "double", "long", "None1", "long"};
   std::vector<std::string> fHeaders;
   std::map<std::string, ColType_t> fColTypes;

   unsigned int fNSlots = 0U;
   unsigned int fNColumnDepth = 25;
   std::string  fHipoFile;
   hipo::reader fHipoReader;
   hipo::record fHipoRecord;
   int fHipoCurrentRecord=0;
   int fHipoCurrentMaxRecord=0;
   // int fHipoCurrentEvent=0;
   int fHipoCurrentMaxEvent = 0;

   hipo::dictionary fHipoDict;
   hipo::event  fHipoEvent;
   std::vector<std::string> fAllBankNames;  // ALL possible bank names existing in schema.

   std::map<std::string, int> fBanksToIndex; // Bank name -> bank_index for all banks in run.
   std::vector<hipo::bank>  fBanks;          // [bank_index] - Index to bank. Found banks.

   // Use map to rapidly find the column [col_index]
   std::vector<std::string> fAllColumns;   // [col_index] All possible columns
   std::map<std::string, int> fColumnNameToIndex; // Name -> [col_index]
   std::vector<int>  fColumnBank; // [col_index] Store the bank_index for this column.
   std::vector<int>  fColumnItem; // [col_index] Store the item number for this column.
   std::vector<int>  fColumnType; // [col_index] Store the type number for this column.
   std::vector<bool> fColumnTypeIsVector;  // [col_index] Is it a vector or a scalar?

   // For each active column:
   std::vector<int> fActiveColumns;  // [active_index] -> index; Indexes of the active columns to fAllColumns.
   // std::vector< std::vector<int> > fColumnToPointer; // [active_index][slot] - The pointer to the pointer, one per slot.
   std::vector< int > fIndexToData;     // [active_index] - index to the actual data stores.

   std::vector< std::vector<void *> > fColumnPointers; // [active_index][slot] - The anonymous store of the pointers to the data.

   // The data, one of each type, per slot. data_index = fIndexToData[active_index]
   std::vector< std::vector<int> > fIntData;      // [data_index][slot] - to integer.
   std::vector< std::vector<long> > fLongData;
   std::vector< std::vector<float> > fFloatData;
   std::vector< std::vector<double> > fDoubleData;

   std::vector< std::vector< std::vector<int> > > fVecIntData; // [data_index][slot] to vector.
   std::vector< std::vector< std::vector<long> > > fVecLongData;
   std::vector< std::vector< std::vector<float> > > fVecFloatData;
   std::vector< std::vector< std::vector<double> > > fVecDoubleData;

   hipo::bank RunConfig;
   int stupid_event_number;
   int *stupid_event_number_ptr;
   std::vector<float> stupid_pz;
   std::vector<float> *stupid_pz_ptr;


public:
   int fDebug;
// private:
public:
   std::vector<void *> GetColumnReadersImpl(std::string_view, const std::type_info &) override;

protected:
   std::string AsString() override;

public:
   explicit RHipoDS(std::string_view fileName, int nevt_inspect=100, int debug=0);
   ~RHipoDS() override= default;

   void Finalize() override;

   unsigned long GetEntries();
   const std::vector<std::string> &GetColumnNames() const override;
   std::vector<std::pair<ULong64_t, ULong64_t>> GetEntryRanges() override;
   int GetColNum(std::string_view colName) const;
   int GetTypeNum(std::string_view colName) const;
   std::string GetTypeName(std::string_view colName) const override;
   std::string GetTypeName(int column_index) const;
   bool HasColumn(std::string_view colName) const override;
   void ClearData(int slot=-1);
   bool SetEntry(unsigned int slot, ULong64_t entry) override;
   void SetNSlots(unsigned int nSlots) override;
   virtual std::string GetLabel() override { return "Hipo Datasource"; }

   // Not required utility methods.
   int getEntries(){ return fHipoReader.getEntries(); }



#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
ClassDef(RHipoDS, 0);
#pragma clang diagnostic pop

};

///////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Factory method to create a CSV RDataFrame.
/// \param[in] fileName Path of the Hipo file.
RDataFrame MakeHipoDataFrame(std::string_view fileName);

#endif //HIPODATAFRAME_RHIPODS_HXX
