
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "include/BaseTupleAnalyzer.h"
#include "include/CSCInfo.h"
#include "include/HistGetter.h"

using namespace std;

class Analyzer : public BaseTupleAnalyzer{
public:
  Analyzer(TString fileName, TString treeName) : BaseTupleAnalyzer(fileName,treeName){
    eventInfo.load(this);
    recHitInfo.load(this);
    stripInfo.load(this);
    compInfo.load(this);
    wireInfo.load(this);
    lctInfo.load(this);
    segmentInfo.load(this);

    bookHistos();
  }

  void bookHistos() {
    plotter.book2D("RecHitMap"    ,"RecHitMap    ;x[cm];y[cm]",60,-30,30,90,-90,90);




    plotter.book2D("StripMap","StripMap;strip # ;layer #",70,-0.5,69.5,10,-0.5,9.5);


    plotter.book2D("CompMap","CompMap;strip # ;layer #",85,-0.5,84.5,10,-0.5,9.5);
    plotter.book2D("WireMap","WireMap;wire group # ;layer #",50,-0.5,49.5,10,-0.5,9.5);

    plotter.book2D("LCTMap","LCTMap;half strip # ;wire group #",200,-0.5,199.5,200,-0.5,199.5);
    plotter.book1D("LCTPattern","LCTPattern;pattern #",12,-0.5,11.5);

    plotter.book2D("SegmentMap"    ,"SegmentMap    ;x[cm];y[cm]",60,-30,30,90,-90,90);
    plotter.book2D("ProjSegmentMap"    ,"SegmentMap    ;x[cm];y[cm]",60,-30,30,90,-90,90);

    plotter.book1D("SegmentDxDz","SegmentDxDz;#theta",400,-20,20);
    plotter.book1D("SegmentDyDz","SegmentDyDz;#phi",400,-20,20);


  }

  virtual void runAEvent() {
    for(unsigned int iH = 0; iH < recHitInfo.rh_pos_x->size(); ++iH){
      plotter.get2D("RecHitMap")->Fill((*recHitInfo.rh_pos_x)[iH],(*recHitInfo.rh_pos_y)[iH]);
    }
    for(unsigned int iH = 0; iH < stripInfo.strip_number->size(); ++iH){
      plotter.get2D("StripMap")->Fill(stripInfo.strip_number->at(iH),stripInfo.strip_lay->at(iH));
    }
    for(unsigned int iH = 0; iH < compInfo.comp_strip->size(); ++iH){
        plotter.get2D("CompMap")->Fill(compInfo.comp_strip->at(iH),compInfo.comp_lay->at(iH));
      }
    for(unsigned int iH = 0; iH < wireInfo.wire_grp->size(); ++iH){
        plotter.get2D("WireMap")->Fill(wireInfo.wire_grp->at(iH),wireInfo.wire_lay->at(iH));
      }
    for(unsigned int iH = 0; iH < lctInfo.lct_id->size(); ++iH){
        plotter.get2D("LCTMap")->Fill(lctInfo.lct_keyHalfStrip->at(iH),lctInfo.lct_keyWireGroup->at(iH));
        plotter.get1D("LCTPattern")->Fill(lctInfo.lct_pattern->at(iH));
      }

    unsigned int goodSeg = -1;


    for(unsigned int iH = 0; iH < segmentInfo.segment_id->size(); ++iH){


      plotter.get2D("SegmentMap")->Fill(segmentInfo.segment_pos_x->at(iH),segmentInfo.segment_pos_y->at(iH));



        plotter.get2D("ProjSegmentMap")->Fill(segmentInfo.segment_pos_x->at(iH) +53*segmentInfo.segment_dxdz->at(iH),segmentInfo.segment_pos_y->at(iH)+53*segmentInfo.segment_dydz->at(iH));
        plotter.get1D("SegmentDxDz")->Fill(segmentInfo.segment_dxdz->at(iH));
        plotter.get1D("SegmentDyDz")->Fill(segmentInfo.segment_dydz->at(iH));
      }
  }

  void write(TString fileName){ plotter.write(fileName);}

  HistGetter plotter;
  EventInfo eventInfo;
  RecHitInfo recHitInfo;
  StripInfo  stripInfo;
  CompInfo  compInfo;
  WireInfo  wireInfo;
  LCTInfo  lctInfo;
  SegmentInfo  segmentInfo;
};

#endif

void AnalyzeTuples(std::string fileName,std::string treeName = "CSCDigiTree",std::string outFileName = "plots.root"){
  Analyzer a(fileName,treeName);
  a.analyze();
  a.write(outFileName);
}
