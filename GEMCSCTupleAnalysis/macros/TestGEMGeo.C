
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "../include/BaseCSCAndGEMAnalyzer.h"
#include "include/GEMPlottingInfo.h"
#include "../include/GEMGeometry.h"
#include "../include/Segment.h"

#include<iostream>

using namespace std;
using namespace CSCGEMTuples;

class Analyze : public AnalyzeBoth {
public:
  Analyze(std::string cscFileName, std::string gemFileName,const GEMConfigInfo* info) : AnalyzeBoth(cscFileName,gemFileName,info)
  {
    plotter.book2D("old_proj_SegmentMap"            ,";x[cm];y[cm]"     ,90,-90,90,90,-90,90);
    plotter.book2D("new_proj_SegmentMap"            ,";x[cm];y[cm]"     ,90,-90,90,90,-90,90);
    plotter.book2D("move_proj_SegmentMap"            ,";x[cm];y[cm]"     ,90,-90,90,90,-90,90);
    plotter.book2D("rot_proj_SegmentMap"            ,";x[cm];y[cm]"     ,90,-90,90,90,-90,90);
    plotter.book2D("proj_SegmentMap"            ,";x[cm];y[cm]"     ,90,-90,90,90,-90,90);

    plotter.book1D("new_proj_x"            ,";x [cm]"     ,100,-200,200);
    plotter.book1D("new_proj_y"            ,";y [cm]"     ,100,-200,200);
    plotter.book1D("new_proj_xError"            ,";error [cm]"     ,500,0,10);
    plotter.book1D("new_proj_yError"            ,";error [cm]"     ,500,0,10);
    plotter.book1D("rot_proj_xError"            ,";error [cm]"     ,500,0,10);
    plotter.book1D("rot_proj_yError"            ,";error [cm]"     ,500,0,10);

    plotter.book1D("clus_x"            ,";X [cm]"     ,100,-90,90);
    plotter.book1D("clus_xError"            ,";error [cm]"     ,500,-5,5);
    plotter.book1D("clus_yError"            ,";error [cm]"     ,500,-20,20);

    plotter.book1D("deltaX"            ,";#Delta X [cm]"     ,500,-90,90);
    plotter.book1D("deltaY"            ,";#Delta Y [cm]"     ,500,-90,90);
    plotter.book1D("deltaR"            ,";#Delta R [cm]"     ,500,-90,90);

    plotter.book1D("deltaXoSig"            ,";#Delta X / #sigma"     ,500,-10,10);
    plotter.book1D("deltaYoSig"            ,";#Delta Y / #sigma"     ,500,-10,10);
    plotter.book1D("deltaRoSig"            ,";#Delta R / #sigma"     ,500,-10,10);

    for(unsigned int iV =  0 ; iV < 24; ++iV){
      TString name = TString::Format("deltaXoSig_%u",iV);
      plotter.book1D(name            ,";#Delta X / #sigma"     ,1000,-20,20);
    }


    for(unsigned int iC = 1; iC <= 384;iC += 10){
      cout <<geo.getStripCenter(0,iC).x() <<" ";
    }
    cout << endl;
    for(unsigned int iC = 1; iC <= 384; iC+=10){
      cout <<geo.getClusterInfo(0,iC,5).error_y() <<" ";
    }
  }
  virtual  ~Analyze() {};

  void write(TString outFileName){ plotter.write(outFileName);}

  virtual void runAEvent() {
    if(!pureSample(csc)) return;
    if(!gem.gemInfo.vFats.size()) return;

    double sX = csc.segmentInfo.segment_pos_x->at(0);
    double sY = csc.segmentInfo.segment_pos_y->at(0);
    double spX,spY,projxe,projye;
    csc.projSement(0,gem.gemGeo.ZDIST,spX,spY,projxe,projye);
    plotter.get2D("old_proj_SegmentMap")->Fill(spX,spY);

    Segment seg;
    seg.setCoords(csc.segmentInfo.segment_pos_x->at(0),csc.segmentInfo.segment_pos_y->at(0),csc.segmentInfo.segment_dxdz->at(0),csc.segmentInfo.segment_dydz->at(0));
    seg.setCov(csc.segmentInfo.segment_cov_x->at(0),csc.segmentInfo.segment_cov_x_y->at(0),csc.segmentInfo.segment_cov_dxdz_x->at(0),csc.segmentInfo.segment_cov_dydz_x->at(0),
        csc.segmentInfo.segment_cov_y->at(0),csc.segmentInfo.segment_cov_dxdz_y->at(0),csc.segmentInfo.segment_cov_dydz_y->at(0),
        csc.segmentInfo.segment_cov_dxdz->at(0),csc.segmentInfo.segment_cov_dxdz_dydz->at(0),csc.segmentInfo.segment_cov_dydz->at(0));
    auto p = seg.project(0,0,gem.gemGeo.ZDIST,0);
    plotter.get1D("new_proj_x")->Fill(p.x());
    plotter.get1D("new_proj_y")->Fill(p.y());
    plotter.get1D("new_proj_xError")->Fill(p.error_x());
    plotter.get1D("new_proj_yError")->Fill(p.error_y());
    plotter.get2D("new_proj_SegmentMap")->Fill(p.x(),p.y());
    p = seg.project(10,10,gem.gemGeo.ZDIST,0);


    plotter.get2D("move_proj_SegmentMap")->Fill(p.x(),p.y());
    p = seg.project(0,0,gem.gemGeo.ZDIST,3.14/4.0);
    plotter.get2D("rot_proj_SegmentMap")->Fill(p.x(),p.y());
    plotter.get1D("rot_proj_xError")->Fill(p.error_x());
    plotter.get1D("rot_proj_yError")->Fill(p.error_y());



    if(gem.gemInfo.clusters.size() != 1) return;

    auto translateRow = [] (int row) -> int {
      row -= 3;
      row *= -1;
      row += 4;
      return row;
    };

    auto translateStrip = [] (int strip) -> int {
      strip -= 192;
      strip *= -1;
      strip += 192;
      return strip;
    };


    int transRow = translateRow(gem.gemInfo.clusters[0].nRow);
    int transStrip = translateStrip(gem.gemInfo.clusters[0].getFirstStrip());
//    if(gem.gemInfo.vFats[0].idx != 0 && gem.gemInfo.vFats[0].idx != 8 && gem.gemInfo.vFats[0].idx != 16) return;
//    if(gem.gemInfo.clusters[0].nStrips != 2) return;
    auto gemClus = geo.getClusterInfo(transRow,transStrip,gem.gemInfo.clusters[0].getNStrips());

//    cout <<transRow <<" "<< gemClus.x() <<" "<<" "<<transStrip <<" "<< gem.gemInfo.clusters[0].getNStrips()<<" " << gemClus.y()<<  endl;

    plotter.get1D("clus_x")->Fill(gemClus.x());
    plotter.get1D("clus_xError")->Fill(gemClus.error_x());
    plotter.get1D("clus_yError")->Fill(gemClus.error_y());
//    auto cscSeg = seg.project(1.6058,0.995368,51.6499,0.0534664);
    auto cscSeg = seg.project(-0.8773,1.60713,33.7098,-0.0238179);
    plotter.get2D("proj_SegmentMap")->Fill(cscSeg.x(),cscSeg.y());

    gemClus -= cscSeg;
//    cout << cscSeg.x()<<" "<< gemClus.x() << endl;

    plotter.get1D("deltaX")->Fill(gemClus.x());
    plotter.get1D("deltaY")->Fill(gemClus.y());
    plotter.get1D("deltaR")->Fill(TMath::Sqrt(gemClus.y()*gemClus.y() + gemClus.x()*gemClus.y()));

    plotter.get1D("deltaXoSig")->Fill(gemClus.x()/gemClus.error_x());
    plotter.get1D("deltaYoSig")->Fill(gemClus.y()/gemClus.error_y());


      TString name = TString::Format("deltaXoSig_%u",gem.gemInfo.vFats[0].idx);
      plotter.get1D(name)->Fill(gemClus.x()/gemClus.error_x());


//
//    plotter.book1D("deltaX"            ,";#Delta X [cm]"     ,500,-10,10);
//    plotter.book1D("deltaY"            ,";#Delta Y [cm]"     ,500,-10,10);
//    plotter.book1D("deltaR"            ,";#Delta R [cm]"     ,500,-10,10);
//
//    plotter.book1D("deltaXoSig"            ,";#Delta X / #sigma"     ,500,-5,5);
//    plotter.book1D("deltaYoSig"            ,";#Delta Y / #sigma"     ,500,-5,5);
//    plotter.book1D("deltaRoSig"            ,";#Delta R / #sigma"     ,500,-5,5);





  }

  HistGetter plotter;
  GEMGeometry geo;
};

#endif

void TestGEMGeo(std::string cscfileName="csc_forsync.root",std::string gemfilename = "gem_forsync.root",std::string outFileName = "plotCSCAndGEM_out.root"){
  GEMConfigInfo info;
  info.geoName               = "gemGeo.txt";
  info.vFATChanMapName       = "slot_table_904_june09.csv";
//  info.vFATChanMapName       = "slot_table.csv";
  Analyze a(cscfileName,gemfilename,&info);
  a.analyze();
  a.write(outFileName);
//  a.draw();
}
