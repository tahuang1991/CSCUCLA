#include "../include/BaseCSCAndGEMAnalyzer.h"
#include "../../gem-light-dqm/gemtreewriter/include/Event.h"

namespace CSCGEMTuples {
AnalyzeCSC::AnalyzeCSC(std::string fileName, std::string treeName) : BaseTupleAnalyzer(fileName,treeName){
    eventInfo.load(this);
    recHitInfo.load(this);
    stripInfo.load(this);
    compInfo.load(this);
    wireInfo.load(this);
    lctInfo.load(this);
    segmentInfo.load(this);
    clctInfo.load(this);
  }

void AnalyzeCSC::analyze(int reportFrequency) {
  while(nextEvent(reportFrequency)){runAEvent();eventNumber++;}
}

AnalyzeGEM::AnalyzeGEM(std::string fileName, std::string treeName,const GEMConfigInfo * gemInfo)
  : BaseTupleAnalyzer(fileName,treeName),
    gemInfo(*gemInfo),
    gemGeo(gemInfo->dataDir+gemInfo->geoName){
  event = new Event();
  setBranchAddress("GEMEvents",&event,true);
}
void AnalyzeGEM::runAEvent() {
  gemInfo.build(event);
}

AnalyzeBoth::AnalyzeBoth(std::string cscFile, std::string gemFile,const GEMConfigInfo* gemInfo) :
  csc(cscFile,"CSCDigiTree"),
  gem(gemFile,"GEMtree", gemInfo)
{}

void AnalyzeBoth::projSement(SegmentInfo * segInfo, int segIDX, double projZ, double& projx, double& projy){
  double sX = segInfo->segment_pos_x->at(segIDX);
  double sY = segInfo->segment_pos_y->at(segIDX);
  projx = sX +projZ*segInfo->segment_dxdz->at(segIDX);
  projy = sY +projZ*segInfo->segment_dydz->at(segIDX);
}

void AnalyzeBoth::analyze(int reportFrequency) {
  gem.eventNumber +=1;
  while(csc.nextEvent(reportFrequency) && gem.nextEvent(reportFrequency)){
    csc.runAEvent();
    gem.runAEvent();
    runAEvent();
    csc.eventNumber++;
    gem.eventNumber++;
  }
}

}

