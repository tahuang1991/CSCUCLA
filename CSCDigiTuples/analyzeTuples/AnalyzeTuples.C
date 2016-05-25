
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
    clctInfo.load(this);

    bookHistos();
  }

  bool withinBox(double x, double y, double height, double lowW, double highW){
    double y1 = -height/2;
    double y2 = height/2;
    double x1 = -lowW/2;
    double x2 = lowW/2;
    double x3 = -highW/2;
    double x4 = highW/2;
    if(y < y1) return false;
    if(y > y2) return false;

//    cout << (x1-x3)/(y1-y2) <<" "<< (x1*y2-x3*y1)<<endl;

    if( x < y*(x1-x3)/(y1-y2) - (x1*y2-x3*y1)/(y1-y2)  ) return false;
    if( x > y*(x2-x4)/(y1-y2) - (x2*y2-x4*y1)/(y1-y2)) return false;
    return true;
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

    plotter.book2D("SegmentMap_withinBox"    ,"SegmentMap_withinBox    ;x[cm];y[cm]",60,-30,30,90,-90,90);
    plotter.book2D("ProjSegmentMap_withinBox"    ,"ProjSegmentMap_withinBox    ;x[cm];y[cm]",60,-30,30,90,-90,90);

    plotter.book1D("SegmentsIn","SegmentsIn;SegmentsIn",2,-0.5,1.5);
    plotter.book1D("ProjSegmentsIn","ProjSegmentsIn;ProjSegmentsIn",2,-0.5,1.5);

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

    if(segmentInfo.segment_id->size() == 1)
    for(unsigned int iH = 0; iH < segmentInfo.segment_id->size(); ++iH){


      plotter.get2D("SegmentMap")->Fill(segmentInfo.segment_pos_x->at(iH),segmentInfo.segment_pos_y->at(iH));
        plotter.get2D("ProjSegmentMap")->Fill(segmentInfo.segment_pos_x->at(iH) +53*segmentInfo.segment_dxdz->at(iH),segmentInfo.segment_pos_y->at(iH)+53*segmentInfo.segment_dydz->at(iH));

        if(withinBox(segmentInfo.segment_pos_x->at(iH),segmentInfo.segment_pos_y->at(iH),100,16,32)){
            plotter.get2D("SegmentMap_withinBox")->Fill(segmentInfo.segment_pos_x->at(iH),segmentInfo.segment_pos_y->at(iH));
            plotter.get1D("SegmentsIn")->Fill(1);
        } else{
          plotter.get1D("SegmentsIn")->Fill(0);
        }
        if(withinBox(segmentInfo.segment_pos_x->at(iH) +53*segmentInfo.segment_dxdz->at(iH),segmentInfo.segment_pos_y->at(iH)+53*segmentInfo.segment_dydz->at(iH),100,16,32)){
            plotter.get2D("ProjSegmentMap_withinBox")->Fill(segmentInfo.segment_pos_x->at(iH) +53*segmentInfo.segment_dxdz->at(iH),segmentInfo.segment_pos_y->at(iH)+53*segmentInfo.segment_dydz->at(iH));
            plotter.get1D("ProjSegmentsIn")->Fill(1);
        } else {
          plotter.get1D("ProjSegmentsIn")->Fill(0);
        }

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
  CLCTInfo  clctInfo;
};

#endif

void AnalyzeTuples(std::string fileName,std::string treeName = "CSCDigiTree",std::string outFileName = "plots.root"){
  Analyzer a(fileName,treeName);
  a.analyze();
  a.write(outFileName);
}
