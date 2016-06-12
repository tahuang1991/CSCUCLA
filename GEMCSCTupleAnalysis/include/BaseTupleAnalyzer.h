#ifndef CSCUCLA_CSCGEMTUPLEANALYZER_BASETUPLEANALYZER_H
#define CSCUCLA_CSCGEMTUPLEANALYZER_BASETUPLEANALYZER_H

#include <TFile.h>
#include <TString.h>
#include <TTree.h>
#include <assert.h>
#include <iostream>


namespace CSCGEMTuples{
class BaseTupleAnalyzer{
public:
  BaseTupleAnalyzer(std::string  fileName, std::string  treeName);
  virtual ~BaseTupleAnalyzer();
  virtual void analyze(int reportFrequency = 1000000);
  virtual void runAEvent() = 0;
  //Load the next event from the tree....return false if there are no more events in the tree
  bool nextEvent(int reportFrequency = 1000000);
  template<typename varType>
  void setBranchAddress(const std::string varName, varType **var, bool require = false, bool verbose = true) {
    TString tBranchName = varName.c_str();
    if(tree->GetBranch(tBranchName)){
      tree->SetBranchStatus(tBranchName,1);
      tree->SetBranchAddress(tBranchName,var);
    }
    else {
      if(require) throw std::invalid_argument(( TString("BaseTupleAnalyzer::setBranchAddress could not load variable: " ) + tBranchName).Data() );
      if(verbose)std::clog << " -" <<tBranchName;
    }
  }
  template<typename varType>
  void setBranchAddress(const std::string varName, varType *var, bool require = false, bool verbose = true) {
    TString tBranchName = varName.c_str();
    if(tree->GetBranch(tBranchName)){
      tree->SetBranchStatus(tBranchName,1);
      tree->SetBranchAddress(tBranchName,var);
    }
    else {
      if(require) throw std::invalid_argument(( TString("BaseTupleAnalyzer::setBranchAddress could not load variable: " ) + tBranchName).Data() );
      if(verbose)std::clog << " -" <<tBranchName;
    }
  }


  TFile * file;
  TTree * tree;
  int     eventNumber; //current event number
};
}

#endif
