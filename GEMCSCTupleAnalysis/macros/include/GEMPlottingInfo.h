#ifndef CSCUCLA_CSCDIGITUPLES_ANALYZETUPLES_GEMPLOTTINGINFO_H
#define CSCUCLA_CSCDIGITUPLES_ANALYZETUPLES_GEMPLOTTINGINFO_H

#include <TFile.h>
#include <TString.h>
#include <TTree.h>
#include <assert.h>
#include <iostream>

#include "../../include/BaseCSCAndGEMAnalyzer.h"
#include "HistGetter.h"

namespace CSCGEMTuples {
class PlotClusterInfo {
public:
  void bookHistos(HistGetter& plotter){

    for(unsigned int iV = 0; iV < GEMGeoInfo::NVFAT + 1; ++iV){
      TString name = TString::Format("%u",iV);
      if(iV == GEMGeoInfo::NVFAT) name = "all";
      plotter.book1D(TString::Format("numClusters_%s",name.Data())               ,";number of clusters"  ,20,-0.5,19.5);
      plotter.book1D(TString::Format("numClusters_noise_%s",name.Data())         ,";number of clusters"  ,20,-0.5,19.5);
      plotter.book1D(TString::Format("clusterSize_%s",name.Data())               ,";cluster size"        ,10,0.5,10.5);
      plotter.book1D(TString::Format("clusterSize_signal_%s",name.Data())         ,";cluster size"       ,10,0.5,10.5);
      plotter.book1D(TString::Format("clusterSize_noise_%s",name.Data())          ,";cluster size"       ,10,0.5,10.5);
    }

    plotter.book2D("numClusters_2D_hit",";Number of clusters"  ,GEMGeoInfo::NCOLS, -0.5, GEMGeoInfo::NCOLS -.5,GEMGeoInfo::NROWS, -0.5, GEMGeoInfo::NROWS -.5);
    plotter.book2D("numClusters_2D_all",";Number of clusters"  ,GEMGeoInfo::NCOLS, -0.5, GEMGeoInfo::NCOLS -.5,GEMGeoInfo::NROWS, -0.5, GEMGeoInfo::NROWS -.5);

//    plotter.book2D("numClusters_signal_hit",";Number of clusters"  ,GEMGeoInfo::NCOLS, -0.5, GEMGeoInfo::NCOLS -.5,GEMGeoInfo::NROWS, -0.5, GEMGeoInfo::NROWS -.5);
//    plotter.book2D("numClusters_signal_all",";Number of clusters"  ,GEMGeoInfo::NCOLS, -0.5, GEMGeoInfo::NCOLS -.5,GEMGeoInfo::NROWS, -0.5, GEMGeoInfo::NROWS -.5);

    plotter.book2D("numClusters_2D_noiseEvent_hit",";Number of clusters"  ,GEMGeoInfo::NCOLS, -0.5, GEMGeoInfo::NCOLS -.5,GEMGeoInfo::NROWS, -0.5, GEMGeoInfo::NROWS -.5);
    plotter.book2D("numClusters_2D_noise_hit",";Number of clusters"  ,GEMGeoInfo::NCOLS, -0.5, GEMGeoInfo::NCOLS -.5,GEMGeoInfo::NROWS, -0.5, GEMGeoInfo::NROWS -.5);
    plotter.book2D("numClusters_2D_noise_all",";Number of clusters"  ,GEMGeoInfo::NCOLS, -0.5, GEMGeoInfo::NCOLS -.5,GEMGeoInfo::NROWS, -0.5, GEMGeoInfo::NROWS -.5);
  }

  template<class Analyzer>
  void fillHistos(Analyzer * ana, HistGetter& plotter){
    AnalyzeCSC&  csc = ana->csc;
    AnalyzeGEM&  gem = ana->gem;

    if(csc.segmentInfo.segment_pos_x->size() == 0) return;
    bool pure = true;
    if(csc.segmentInfo.segment_pos_x->size() != 1) pure=false;
    if(csc.recHitInfo.rh_id->size() > csc.segmentInfo.segment_nHits->at(0) + 1 ) pure=false;

    //We need to look at every vFat....so let's get a map first
    std::vector<int> vFatIdx(GEMGeoInfo::NVFAT,-1);
    for(unsigned int iV = 0; iV < gem.gemInfo.vFats.size(); ++iV){vFatIdx[gem.gemInfo.vFats[iV].idx] = iV;}

    //Now fill number of clusters and size
    std::vector<int> nClus(GEMGeoInfo::NVFAT,0);
    std::vector<std::vector<int> > clusSize(GEMGeoInfo::NVFAT,std::vector<int>(0));
    getClustersByVFAT(gem.gemInfo.clusters,nClus,clusSize);

    //Now we find our "signal" vfats and
    std::vector<bool> isSignalVFat(GEMGeoInfo::NVFAT,false);
    std::vector<bool> isNoiseVFat(GEMGeoInfo::NVFAT,true);
    getSigNoise(ana,csc,gem,isSignalVFat,isNoiseVFat);



    plotter.get1D("numClusters_all")->Fill(gem.gemInfo.clusters.size());
    int nClusNoise = 0;
    for(unsigned int iC = 0; iC < gem.gemInfo.clusters.size(); ++iC){
      plotter.get1D("clusterSize_all")->Fill(gem.gemInfo.clusters[iC].getNStrips());
      if(isNoiseVFat[gem.gemInfo.clusters[iC].nRow] && pure) plotter.get1D("clusterSize_noise_all")->Fill(gem.gemInfo.clusters[iC].getNStrips());
      if(isSignalVFat[gem.gemInfo.clusters[iC].nRow] && pure) plotter.get1D("clusterSize_signal_all")->Fill(gem.gemInfo.clusters[iC].getNStrips());
    }

    for(unsigned int iV = 0; iV < GEMGeoInfo::NVFAT; ++iV){
      TString name = TString::Format("%u",iV);

      plotter.get1D(TString::Format("numClusters_%s",name.Data()) )->Fill(nClus[iV]);
      if(isNoiseVFat[iV] && pure)plotter.get1D(TString::Format("numClusters_noise_%s",name.Data()) )->Fill(nClus[iV]);

      for(unsigned int iC = 0; iC < clusSize[iV].size(); ++iC){
        plotter.get1D(TString::Format("clusterSize_%s",name.Data()) )->Fill(clusSize[iV][iC]);
        if(isSignalVFat[iV] && pure)plotter.get1D(TString::Format("clusterSize_signal_%s",name.Data()) )->Fill(clusSize[iV][iC]);
        if(isNoiseVFat[iV] && pure)plotter.get1D(TString::Format("clusterSize_noise_%s",name.Data()) )->Fill(clusSize[iV][iC]);
      }

      int idx=2;
      if(iV >= 8) idx = 1;
      if(iV >= 16) idx = 0;
      if(nClus[iV]) plotter.get2D("numClusters_2D_hit")->Fill(idx,GEMGeoInfo::getRow(iV),nClus[iV]);
      plotter.get2D("numClusters_2D_all")->Fill(idx,GEMGeoInfo::getRow(iV));

      if(isNoiseVFat[iV] && pure){
        if(nClus[iV]) plotter.get2D("numClusters_2D_noise_hit")->Fill(idx,GEMGeoInfo::getRow(iV),nClus[iV]);
        if(nClus[iV]) plotter.get2D("numClusters_2D_noiseEvent_hit")->Fill(idx,GEMGeoInfo::getRow(iV));
        plotter.get2D("numClusters_2D_noise_all")->Fill(idx,GEMGeoInfo::getRow(iV));
      }
    }

  }

  template<class Analyzer>
  void getSigNoise(Analyzer * ana, AnalyzeCSC&  csc, AnalyzeGEM&  gem, std::vector<bool>& isSignalVFat, std::vector<bool>& isNoiseVFat ){
    //Super stupid right now

    double sX = csc.segmentInfo.segment_pos_x->at(0);
    double sY = csc.segmentInfo.segment_pos_y->at(0);

    double spX = sX +gem.gemGeo.ZDIST*csc.segmentInfo.segment_dxdz->at(0);
    double spY = sY +gem.gemGeo.ZDIST*csc.segmentInfo.segment_dydz->at(0);

    const auto& geo = gem.gemGeo;
    const auto& vfatgeo = gem.gemGeo.vFat;
    auto isInVFat = [](double& in, const VFATGeo& geo, bool strict) -> bool { return (geo.isValid)  && (in < geo.yMax) && ( strict ? in > geo.yMin : 1.0) ;};

    if( isInVFat(spY, vfatgeo[0], false) || isInVFat(spY, vfatgeo[8], false) || isInVFat(spY, vfatgeo[16], false)  ){
      isSignalVFat[0] =true; isSignalVFat[8] =true; isSignalVFat[16] =true;
      isNoiseVFat[0 ] =false; isNoiseVFat[8 ] =false; isNoiseVFat[16] =false;
      isNoiseVFat[1 ] =false; isNoiseVFat[9 ] =false; isNoiseVFat[17] =false;
      isNoiseVFat[2 ] =false; isNoiseVFat[10] =false; isNoiseVFat[18] =false;
    } else if( isInVFat(spY, vfatgeo[1], false) || isInVFat(spY, vfatgeo[9], false) || isInVFat(spY, vfatgeo[17], false)  ){
      isSignalVFat[1] =true; isSignalVFat[9] =true; isSignalVFat[17] =true;
      isNoiseVFat[0 ] =false; isNoiseVFat[8 ] =false; isNoiseVFat[16] =false;
      isNoiseVFat[1 ] =false; isNoiseVFat[9 ] =false; isNoiseVFat[17] =false;
      isNoiseVFat[2 ] =false; isNoiseVFat[10] =false; isNoiseVFat[18] =false;
      isNoiseVFat[3 ] =false; isNoiseVFat[11] =false; isNoiseVFat[19] =false;
    } else if( isInVFat(spY, vfatgeo[2], false) || isInVFat(spY, vfatgeo[10], false) || isInVFat(spY, vfatgeo[18], false)  ){
      isSignalVFat[2] =true; isSignalVFat[10] =true; isSignalVFat[18] =true;
      isNoiseVFat[0 ] =false; isNoiseVFat[8 ] =false; isNoiseVFat[16] =false;
      isNoiseVFat[1 ] =false; isNoiseVFat[9 ] =false; isNoiseVFat[17] =false;
      isNoiseVFat[2 ] =false; isNoiseVFat[10] =false; isNoiseVFat[18] =false;
      isNoiseVFat[3 ] =false; isNoiseVFat[11] =false; isNoiseVFat[19] =false;
      isNoiseVFat[4 ] =false; isNoiseVFat[12] =false; isNoiseVFat[20] =false;
    } else if( isInVFat(spY, vfatgeo[3], false) || isInVFat(spY, vfatgeo[11], false) || isInVFat(spY, vfatgeo[19], false)  ){
      isSignalVFat[3] =true; isSignalVFat[11] =true; isSignalVFat[19] =true;
      isNoiseVFat[1 ] =false; isNoiseVFat[9 ] =false; isNoiseVFat[17] =false;
      isNoiseVFat[2 ] =false; isNoiseVFat[10] =false; isNoiseVFat[18] =false;
      isNoiseVFat[3 ] =false; isNoiseVFat[11] =false; isNoiseVFat[19] =false;
      isNoiseVFat[4 ] =false; isNoiseVFat[12] =false; isNoiseVFat[20] =false;
      isNoiseVFat[5 ] =false; isNoiseVFat[13] =false; isNoiseVFat[21] =false;
    } else if( isInVFat(spY, vfatgeo[4], false) || isInVFat(spY, vfatgeo[12], false) || isInVFat(spY, vfatgeo[20], false)  ){
      isSignalVFat[4] =true; isSignalVFat[12] =true; isSignalVFat[20] =true;
      isNoiseVFat[2 ] =false; isNoiseVFat[10] =false; isNoiseVFat[18] =false;
      isNoiseVFat[3 ] =false; isNoiseVFat[11] =false; isNoiseVFat[19] =false;
      isNoiseVFat[4 ] =false; isNoiseVFat[12] =false; isNoiseVFat[20] =false;
      isNoiseVFat[5 ] =false; isNoiseVFat[13] =false; isNoiseVFat[21] =false;
      isNoiseVFat[6 ] =false; isNoiseVFat[14] =false; isNoiseVFat[22] =false;
    } else if( isInVFat(spY, vfatgeo[5], false) || isInVFat(spY, vfatgeo[13], false) || isInVFat(spY, vfatgeo[21], false)  ){
      isSignalVFat[5] =true; isSignalVFat[13] =true; isSignalVFat[21] =true;
      isNoiseVFat[3 ] =false; isNoiseVFat[11] =false; isNoiseVFat[19] =false;
      isNoiseVFat[4 ] =false; isNoiseVFat[12] =false; isNoiseVFat[20] =false;
      isNoiseVFat[5 ] =false; isNoiseVFat[13] =false; isNoiseVFat[21] =false;
      isNoiseVFat[6 ] =false; isNoiseVFat[14] =false; isNoiseVFat[22] =false;
      isNoiseVFat[7 ] =false; isNoiseVFat[15] =false; isNoiseVFat[23] =false;
    } else if( isInVFat(spY, vfatgeo[6], false) || isInVFat(spY, vfatgeo[14], false) || isInVFat(spY, vfatgeo[22], false)  ){
      isSignalVFat[6] =true; isSignalVFat[14] =true; isSignalVFat[22] =true;
      isNoiseVFat[4 ] =false; isNoiseVFat[12] =false; isNoiseVFat[20] =false;
      isNoiseVFat[5 ] =false; isNoiseVFat[13] =false; isNoiseVFat[21] =false;
      isNoiseVFat[6 ] =false; isNoiseVFat[14] =false; isNoiseVFat[22] =false;
      isNoiseVFat[7 ] =false; isNoiseVFat[15] =false; isNoiseVFat[23] =false;
    } else if( isInVFat(spY, vfatgeo[7], false) || isInVFat(spY, vfatgeo[15], false) || isInVFat(spY, vfatgeo[23], false)  ){
      isSignalVFat[7] =true; isSignalVFat[15] =true; isSignalVFat[23] =true;
      isNoiseVFat[5 ] =false; isNoiseVFat[13] =false; isNoiseVFat[21] =false;
      isNoiseVFat[6 ] =false; isNoiseVFat[14] =false; isNoiseVFat[22] =false;
      isNoiseVFat[7 ] =false; isNoiseVFat[15] =false; isNoiseVFat[23] =false;
    } else {
      isNoiseVFat[5 ] =false; isNoiseVFat[13] =false; isNoiseVFat[21] =false;
      isNoiseVFat[6 ] =false; isNoiseVFat[14] =false; isNoiseVFat[22] =false;
      isNoiseVFat[7 ] =false; isNoiseVFat[15] =false; isNoiseVFat[23] =false;
    }

//        std::cout << "(" << spX <<","<<spY<<")"<<std::endl;
//        for(unsigned int iV = 0; iV < GEMGeoInfo::NVFAT; ++iV){
//          std::cout << iV << " "<<" " << isSignalVFat[iV]<< " "<< isNoiseVFat[iV]<<" "<< vfatgeo[iV].yMax<<std::endl;
//        }

  }

  void getClustersByVFAT(const std::vector<GEMCluster>& clusters ,std::vector<int>& nClus, std::vector<std::vector<int> >& clusSize){
    for(unsigned int iC = 0; iC < clusters.size(); ++iC){
      bool fill_col1 = false; //left to right
      bool fill_col2 = false;
      bool fill_col3 = false;

      if(clusters[iC].getFirstStrip() >= 256  ){
        fill_col1 = true;
        if(clusters[iC].getLastStrip() <= 255 ) fill_col2 = true;
        if(clusters[iC].getLastStrip() <= 127 ) fill_col3 = true;
      } else if (clusters[iC].getFirstStrip() >= 128  ) {
        fill_col2 = true;
        if(clusters[iC].getLastStrip() <= 127 ) fill_col3 = true;
      } else{
        fill_col3 = true;
      }

      std::vector<int> fillVFAT;
      if(fill_col1) fillVFAT.push_back(clusters[iC].nRow +16);
      if(fill_col2) fillVFAT.push_back(clusters[iC].nRow + 8);
      if(fill_col3) fillVFAT.push_back(clusters[iC].nRow);

      for(unsigned int iV = 0; iV < fillVFAT.size(); ++iV){
        nClus[fillVFAT[iV]]++;
        clusSize[fillVFAT[iV]].push_back(clusters[iC].getNStrips());
      }

    }
  }

};


class PlotVFATInfo {
public:
  void bookHistos(HistGetter& plotter){

    plotter.book2D("SegmentMap_NOGEMHIT"      ,";x[cm];y[cm]"     ,60,-30,30,90,-90,90);
    plotter.book2D("SegmentMap_GEMHIT"        ,";x[cm];y[cm]"     ,60,-30,30,90,-90,90);
    plotter.book2D("SegmentMap"                ,";x[cm];y[cm]"     ,60,-30,30,90,-90,90);
    plotter.book2D("ProjSegmentMap_NOGEMHIT"  ,";x[cm];y[cm]"     ,60,-30,30,90,-90,90);
    plotter.book2D("ProjSegmentMap_GEMHIT"    ,";x[cm];y[cm]"     ,60,-30,30,90,-90,90);
    plotter.book2D("ProjSegmentMap"            ,";x[cm];y[cm]"     ,60,-30,30,90,-90,90);
    plotter.book2D("VFATOcc"                   ,";VFAT #"          ,GEMGeoInfo::NCOLS, -0.5, GEMGeoInfo::NCOLS -.5,GEMGeoInfo::NROWS, -0.5, GEMGeoInfo::NROWS -.5);
    plotter.book2D("VFATOcc_all"                   ,";VFAT #"          ,GEMGeoInfo::NCOLS, -0.5, GEMGeoInfo::NCOLS -.5,GEMGeoInfo::NROWS, -0.5, GEMGeoInfo::NROWS -.5);

    plotter.book2D("StripMap"    ,";Strip #;VFAT #",130,-0.5,129.5,GEMGeoInfo::NVFAT,-0.5,GEMGeoInfo::NVFAT - 0.5 );
    plotter.book2D("StripMapPhysical"    ,"",384,-0.5,383.5,GEMGeoInfo::NROWS, -0.5, GEMGeoInfo::NROWS -.5);

    for(unsigned int iV = 0; iV < GEMGeoInfo::NVFAT; ++iV){
      plotter.book2D(TString::Format("SegmentMap_%u",iV)        ,TString::Format("vFat %u;x[cm];y[cm]",iV)     ,60,-30,30,90,-90,90);
      plotter.book2D(TString::Format("ProjSegmentMap_%u",iV)    ,TString::Format("Proj vFat %u;x[cm];y[cm]",iV),60,-30,30,90,-90,90);
    }
   }

  template<class Analyzer>
  void fillHistos(Analyzer * ana, HistGetter& plotter){
    AnalyzeCSC&  csc = ana->csc;
    AnalyzeGEM&  gem = ana->gem;

    if(csc.segmentInfo.segment_pos_x->size() == 0) return;
    bool pure = true;
    if(csc.segmentInfo.segment_pos_x->size() != 1) pure=false;
    if(csc.recHitInfo.rh_id->size() > csc.segmentInfo.segment_nHits->at(0) + 1 ) pure=false;

    for(unsigned int iV = 0; iV < GEMGeoInfo::NVFAT; ++iV){
      int idx=2;
      if(iV >= 8) idx = 1;
      if(iV  >= 16) idx = 0;
      plotter.get2D("VFATOcc_all")->Fill(idx,GEMGeoInfo::getRow(iV));

    }

    for(unsigned int iV = 0; iV < gem.gemInfo.vFats.size(); ++iV){
      auto& vFat = gem.gemInfo.vFats[iV];
      int idx=2;
      if(vFat.idx >= 8) idx = 1;
      if(vFat.idx  >= 16) idx = 0;
      plotter.get2D("VFATOcc")->Fill(idx,vFat.nRow);

      for(unsigned int iS = 0; iS < vFat.nStrips(); ++iS){
        plotter.get2D("StripMap")->Fill(vFat.strips[iS] ,vFat.idx);
        plotter.get2D("StripMapPhysical")->Fill(GEMGeoInfo::getGlobalStripXLeftIs0(vFat.idx,vFat.strips[iS]),vFat.nRow);
      }
    }

    if(pure){
      int iS = 0;
      double sX = csc.segmentInfo.segment_pos_x->at(iS);
      double sY = csc.segmentInfo.segment_pos_y->at(iS);

      double spX = sX +gem.gemGeo.ZDIST*csc.segmentInfo.segment_dxdz->at(iS);
      double spY = sY +gem.gemGeo.ZDIST*csc.segmentInfo.segment_dydz->at(iS);
      plotter.get2D("SegmentMap")->Fill(sX,sY);
      plotter.get2D("ProjSegmentMap")->Fill(spX,spY);
      if(gem.gemInfo.vFats.size()){
        plotter.get2D("SegmentMap_GEMHIT")->Fill(sX,sY);
        plotter.get2D("ProjSegmentMap_GEMHIT")->Fill(spX,spY);
      } else {
        plotter.get2D("SegmentMap_NOGEMHIT")->Fill(sX,sY);
        plotter.get2D("ProjSegmentMap_NOGEMHIT")->Fill(spX,spY);
      }

      if(gem.gemInfo.vFats.size() == 1){
        auto& vFat = gem.gemInfo.vFats[0];
        TString name = TString::Format("SegmentMap_%u",vFat.idx);
        TString name2 = TString::Format("ProjSegmentMap_%u",vFat.idx);

        for(unsigned int iS = 0; iS < csc.segmentInfo.segment_pos_x->size(); ++iS){

          plotter.get2D(name)->Fill(sX,sY);
          plotter.get2D(name2)->Fill(spX,spY);
        }
      }

    }

  }
};



class PlotEventInfo {
public:
  void bookHistos(HistGetter& plotter){
    plotter.book1D("BXDiff"   ,";CSC BX - GEM BX",2000, -1000, 1000);
    plotter.book1D("nEvents"   ,";nCSCEvents/nCSCEvents with segments/nGEMEvents/nGEMEvents with CSC segments ",4,-0.5,3.5);
  }

  template<class Analyzer>
  void fillHistos(Analyzer * ana, HistGetter& plotter){
    AnalyzeCSC&  csc = ana->csc;
    AnalyzeGEM&  gem = ana->gem;

    if(csc.wireInfo.wire_bx->at(0) - gem.gemInfo.BX > 0)
      plotter.get1D("BXDiff")->Fill(csc.wireInfo.wire_bx->at(0) - gem.gemInfo.BX);
    else
      plotter.get1D("BXDiff")->Fill(  csc.wireInfo.wire_bx->at(0) - gem.gemInfo.BX + 3564 );

    plotter.get1D("nEvents")->Fill(0);
    if(csc.segmentInfo.segment_pos_x->size()) plotter.get1D("nEvents")->Fill(1);
    if(gem.gemInfo.vFats.size())plotter.get1D("nEvents")->Fill(2);
    if(csc.segmentInfo.segment_pos_x->size() && gem.gemInfo.vFats.size())plotter.get1D("nEvents")->Fill(3);
  }

};

}




#endif
