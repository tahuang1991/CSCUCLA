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

void AnalyzeCSC::projSement(int segIDX, double projZ, double& projx, double& projy,double& projxe, double& projye) const {

  const double s_x          = segmentInfo.segment_pos_x     ->at(segIDX);
  const double s_y          = segmentInfo.segment_pos_y     ->at(segIDX);
  const double s_dxdz       = segmentInfo.segment_dxdz      ->at(segIDX);
  const double s_dydz       = segmentInfo.segment_dydz      ->at(segIDX);
  const double s_cov_x      = segmentInfo.segment_cov_x     ->at(segIDX);
  const double s_cov_y      = segmentInfo.segment_cov_y     ->at(segIDX);
  const double s_cov_dxdz   = segmentInfo.segment_cov_dxdz  ->at(segIDX);
  const double s_cov_dydz   = segmentInfo.segment_cov_dydz  ->at(segIDX);
  const double s_cov_dxdz_x = segmentInfo.segment_cov_dxdz_x->at(segIDX);
  const double s_cov_dydz_y = segmentInfo.segment_cov_dydz_y->at(segIDX);

  projx = s_x + projZ*s_dxdz;
  projy = s_y + projZ*s_dydz;
  projxe = TMath::Sqrt( s_cov_x + projZ*projZ*s_cov_dxdz + 2*projZ*s_cov_dxdz_x);
  projye = TMath::Sqrt( s_cov_y + projZ*projZ*s_cov_dydz + 2*projZ*s_cov_dydz_y);
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

