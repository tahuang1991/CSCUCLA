
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "../include/BaseCSCAndGEMAnalyzer.h"
#include "include/iqagent.h"
#include "include/GEMPlottingInfo.h"

#include<iostream>
#include<fstream>
#include<regex>

using namespace std;
using namespace CSCGEMTuples;

class Analyze : public AnalyzeBoth {
public:
  Analyze(std::string cscFileName, std::string gemFileName) : AnalyzeBoth(cscFileName,gemFileName)
  {
    quantQueuesX.resize(GEMGeoInfo::NVFAT);
    quantQueuesY.resize(GEMGeoInfo::NVFAT);
    for(unsigned int iV = 0; iV < GEMGeoInfo::NVFAT; ++iV){
      quantQueuesX[iV].resize(nZSlices);
      quantQueuesY[iV].resize(nZSlices);
    }
  }
  virtual  ~Analyze() {};

  void write(TString outFileName){
    std::ofstream outFile;
    outFile.open (outFileName, ios::out | ios::trunc);
    assert(outFile.is_open());

    //First output the ZDist
    outFile << "ZDIST "<< minZ <<endl;
    int iZ = 103;
    //Now output vFatInfo
    for(unsigned int iV = 0; iV < GEMGeoInfo::NVFAT; ++iV){
      outFile << "VFAT "<< iV <<" "
                              << quantQueuesX[iV][iZ].report(.5) << " "
                              << quantQueuesX[iV][iZ].report(.1) << " "
                              << quantQueuesX[iV][iZ].report(.9) << " "
                              << quantQueuesY[iV][iZ].report(.5) << " "
                              << quantQueuesY[iV][iZ].report(.1) << " "
                              << quantQueuesY[iV][iZ].report(.9) << " "
                              <<endl;
    }
    outFile.close();

    for(unsigned int iV = 0; iV < GEMGeoInfo::NVFAT; ++iV){
      TString name = TString::Format("vF_%u_projY_50",iV);
      TString name2 = TString::Format("vF_%u_projY_80",iV);
      plotter.book1D(name,";Z projection [cm];size of center 50% of data [cm]",nZSlices,minZ,maxZ);
      plotter.book1D(name2,";Z projection [cm];size of center 80% of data [cm]",nZSlices,minZ,maxZ);
      for(unsigned int iZ = 0; iZ < nZSlices; ++iZ){
        plotter.get1D(name)->SetBinContent(iZ+1,  quantQueuesY[iV][iZ].report(.75) - quantQueuesY[iV][iZ].report(.25)   );
        plotter.get1D(name2)->SetBinContent(iZ+1, quantQueuesY[iV][iZ].report(.9)  - quantQueuesY[iV][iZ].report(.1)   );
      }
    }
    plotter.write(outFileName.ReplaceAll(".txt",".root"));
  }

  virtual void runAEvent() {

    //First filter out any events with more than one segment
    if(!pureSample(csc)) return;
    //And with more than one cluster
    if(gem.gemInfo.clusters.size() != 1) return;

      double interval = float(maxZ - minZ)/float(nZSlices);
      int yV = gem.gemInfo.vFats[0].idx;
      for(unsigned int iI = 0; iI < nZSlices; ++iI){
        double projZ = minZ + interval*iI;
        double projX,projY,projXE,projYE;
        csc.projSement(0,projZ,projX,projY,projXE,projYE);
        quantQueuesX[yV][iI].add(projX);
        quantQueuesY[yV][iI].add(projY);
        if(yV < 8){
          quantQueuesX[yV+8][iI].add(projX);
          quantQueuesY[yV+8][iI].add(projY);
          quantQueuesX[yV+16][iI].add(projX);
          quantQueuesY[yV+16][iI].add(projY);
        } else if (yV < 16){
          quantQueuesX[yV+8][iI].add(projX);
          quantQueuesY[yV+8][iI].add(projY);
          quantQueuesX[yV-8][iI].add(projX);
          quantQueuesY[yV-8][iI].add(projY);
        }else {
          quantQueuesX[yV-16][iI].add(projX);
          quantQueuesY[yV-16][iI].add(projY);
          quantQueuesX[yV-8][iI].add(projX);
          quantQueuesY[yV-8][iI].add(projY);
        }
      }
  }

  int nZSlices = 150;
//  int minZ = 53;
//  int maxZ= 54;
  double minZ = -50;
  double maxZ= 100;
  std::vector<std::vector<IQagent> > quantQueuesX; //[VF][z]
  std::vector<std::vector<IQagent> > quantQueuesY; //[VF][z]
  HistGetter plotter;

};

#endif

void BuildGEMGeometry(std::string cscfileName="csc_forsync.root",std::string gemfilename = "gem_forsync.root",std::string outFileName = "gemGeo.txt"){
  Analyze a(cscfileName,gemfilename);
  a.analyze();
  a.write(outFileName);
//  a.draw();
}
