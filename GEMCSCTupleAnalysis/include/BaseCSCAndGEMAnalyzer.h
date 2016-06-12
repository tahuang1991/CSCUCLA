#ifndef CSCUCLA_CSCGEMTUPLEANALYZER_BASECSCANDGEMANALYZER_H
#define CSCUCLA_CSCGEMTUPLEANALYZER_BASECSCANDGEMANALYZER_H
#include "BaseTupleAnalyzer.h"
#include "CSCInfo.h"
#include "GEMInfo.h"
#include <TFile.h>
#include <TString.h>
#include <TTree.h>
#include <assert.h>
#include <iostream>

namespace CSCGEMTuples {
class AnalyzeCSC : public BaseTupleAnalyzer{
public:
  AnalyzeCSC(std::string fileName, std::string treeName);
  virtual void analyze(int reportFrequency = 1000000);
  virtual void runAEvent() {}

  EventInfo eventInfo;
  RecHitInfo recHitInfo;
  StripInfo  stripInfo;
  CompInfo  compInfo;
  WireInfo  wireInfo;
  LCTInfo  lctInfo;
  SegmentInfo  segmentInfo;
  CLCTInfo  clctInfo;
};



class AnalyzeGEM : public BaseTupleAnalyzer{
public:
  AnalyzeGEM(std::string fileName, std::string treeName,const GEMConfigInfo* gemInfo);
  virtual void runAEvent();
  Event * event;
  GEMInfo gemInfo;
  GEMGeo  gemGeo;
};


class AnalyzeBoth {
public:

  AnalyzeBoth(std::string cscFile, std::string gemFile, const GEMConfigInfo* gemInfo = new GEMConfigInfo);
  virtual ~AnalyzeBoth(){}

  virtual void runAEvent() {};
  void analyze(int reportFrequency = 1000000);

  void projSement(SegmentInfo * segInfo, int segIDX, double projZ, double& projx, double& projy);

  AnalyzeCSC csc;
  AnalyzeGEM gem;

};
}
#endif
