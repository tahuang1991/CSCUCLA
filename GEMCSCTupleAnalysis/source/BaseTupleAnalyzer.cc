#include "../include/BaseTupleAnalyzer.h"


namespace CSCGEMTuples {
BaseTupleAnalyzer::BaseTupleAnalyzer(std::string  fileName, std::string  treeName) : eventNumber(0){
  std::clog << "Loading file: "<< fileName <<" and tree: " << treeName <<std::endl;
  file = TFile::Open(fileName.c_str(),"READ");
  assert(file);
  tree = (TTree*)(file->Get(treeName.c_str()) );
  assert(tree);
  std::clog << tree->GetEntries() << " entries to process" << std::endl;
}

BaseTupleAnalyzer::~BaseTupleAnalyzer(){
  file->Close();
  delete file;
}

void BaseTupleAnalyzer::analyze(int reportFrequency) {
  while(nextEvent(reportFrequency)){runAEvent();eventNumber++;}
}

//Load the next event from the tree....return false if there are no more events in the tree
bool BaseTupleAnalyzer::nextEvent(int reportFrequency)
{
  if(eventNumber >= tree->GetEntries()) return false;
  tree->GetEntry(eventNumber);

  if(eventNumber%reportFrequency == 0)
    std::clog << "Processing event " << eventNumber << std::endl;

  return true;
}
}
