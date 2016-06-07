#ifndef CSCUCLA_CSCDIGITUPLES_ANALYZETUPLES_BASECSCANDGEMANALYZER_H
#define CSCUCLA_CSCDIGITUPLES_ANALYZETUPLES_BASECSCANDGEMANALYZER_H
#include "BaseTupleAnalyzer.h"
#include "CSCInfo.h"
#include "GEMInfo.h"
#include "gemtreewriter/include/Event.h"
#include "../gemdaq-testing/gemreadout/include/gem/readout/GEMslotContents.h"
#include <TFile.h>
#include <TString.h>
#include <TTree.h>
#include <assert.h>
#include <iostream>


class AnalyzeCSC : public BaseTupleAnalyzer{
public:
  AnalyzeCSC(TString fileName, TString treeName) : BaseTupleAnalyzer(fileName,treeName){
    eventInfo.load(this);
    recHitInfo.load(this);
    stripInfo.load(this);
    compInfo.load(this);
    wireInfo.load(this);
    lctInfo.load(this);
    segmentInfo.load(this);
    clctInfo.load(this);
  }

  virtual void analyze(int reportFrequency = 1000000) {
    while(nextEvent(reportFrequency)){runAEvent();eventNumber++;}
  }
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
  AnalyzeGEM(TString fileName, TString treeName, TString gemGeoName) : BaseTupleAnalyzer(fileName,treeName){
  event = new Event();
  setBranchAddress("GEMEvents",&event,true);
  BX = -1;
  buildGEMGeo(gemGeoName,geo);
  }

  virtual void runAEvent() {
    std::vector<AMC13Event> v_amc13 = event->amc13s();
    std::vector<AMCdata> v_amc = v_amc13[0].amcs();
    BX = v_amc[0].BX();
    GEMInfo::fillGEMInfo(vFats,clusters,event);
  }

  GEMInfo::GEMGeo geo;
  Event * event;
  std::vector<GEMInfo::VFAT> vFats;
  std::vector<GEMInfo::GEMCluster> clusters;
  int BX;

};


class AnalyzeBoth {
public:

  AnalyzeBoth(TString cscFile, TString gemFile, TString gemGeoName = "gemGeo.txt") :
    csc(cscFile,"CSCDigiTree"),
    gem(gemFile,"GEMtree", gemGeoName)
  {}
  virtual ~AnalyzeBoth(){}

  virtual void runAEvent() {};

  void projSement(SegmentInfo * segInfo, int segIDX, double projZ, double& projx, double& projy){
    double sX = segInfo->segment_pos_x->at(segIDX);
    double sY = segInfo->segment_pos_y->at(segIDX);
    projx = sX +projZ*segInfo->segment_dxdz->at(segIDX);
    projy = sY +projZ*segInfo->segment_dydz->at(segIDX);
  }

  void analyze(int reportFrequency = 1000000) {
    gem.eventNumber +=1;
    while(csc.nextEvent(1000000) && gem.nextEvent(1000000)){
      csc.runAEvent();
      gem.runAEvent();
      runAEvent();
      csc.eventNumber++;
      gem.eventNumber++;
    }
  }

  AnalyzeCSC csc;
  AnalyzeGEM gem;

};
#endif
