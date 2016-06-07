
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "include/BaseCSCAndGEMAnalyzer.h"
#include "include/iqagent.h"

#include<iostream>
#include<regex>

using namespace std;

class Analyze : public AnalyzeBoth {
public:
  Analyze(TString cscFileName, TString gemFileName) : AnalyzeBoth(cscFileName,gemFileName)
  {
    quantQueuesX.resize(GEMInfo::NVFAT);
    quantQueuesY.resize(GEMInfo::NVFAT);
    for(unsigned int iV = 0; iV < GEMInfo::NVFAT; ++iV){
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
    int iZ = 0;
    //Now output vFatInfo
    for(unsigned int iV = 0; iV < GEMInfo::NVFAT; ++iV){
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
  }

  virtual void runAEvent() {

    //First filter out any events with more than one segment
    if(csc.segmentInfo.segment_pos_x->size() != 1) return;
    //Now filter out any evens with too many exra rec hits
    if(csc.recHitInfo.rh_id->size() > csc.segmentInfo.segment_nHits->at(0) + 1 ) return;
    //Filter out gem events with more than one or 0 fired VFATs
    if(gem.vFats.size() != 1 ) return;

      double interval = float(maxZ - minZ)/float(1);
      int yV = gem.vFats[0].idx;
      for(unsigned int iI = 0; iI < nZSlices; ++iI){
        double projZ = minZ + interval*iI;
        double projX,projY;
        projSement(&csc.segmentInfo,0,projZ,projX,projY);
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

  int nZSlices = 1;
//  int minZ = 53;
//  int maxZ= 54;
  int minZ = 53;
  int maxZ= 54;
  std::vector<std::vector<IQagent> > quantQueuesX; //[VF][z]
  std::vector<std::vector<IQagent> > quantQueuesY; //[VF][z]

};

#endif

void BuildGEMGeometry(std::string cscfileName="csc_forsync.root",std::string gemfilename = "gem_forsync.root",std::string outFileName = "gemGeo.txt"){
  Analyze a(cscfileName,gemfilename);
  a.analyze();
  a.write(outFileName);
//  a.draw();
}
