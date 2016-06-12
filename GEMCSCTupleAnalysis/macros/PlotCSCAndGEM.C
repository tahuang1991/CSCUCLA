
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "../include/BaseCSCAndGEMAnalyzer.h"
#include "include/GEMPlottingInfo.h"

#include<iostream>

using namespace std;
using namespace CSCGEMTuples;

class Analyze : public AnalyzeBoth {
public:
  Analyze(std::string cscFileName, std::string gemFileName,const GEMConfigInfo* info) : AnalyzeBoth(cscFileName,gemFileName,info)
  {
    plotClusterInfo.bookHistos(plotter);
    plotVFATInfo.bookHistos(plotter);
    PlotEventInfo.bookHistos(plotter);
  }
  virtual  ~Analyze() {};

  void write(TString outFileName){ plotter.write(outFileName);}

  virtual void runAEvent() {
    plotClusterInfo.fillHistos(this,plotter);
    plotVFATInfo.fillHistos(this,plotter);
    PlotEventInfo.fillHistos(this,plotter);
  }

  PlotClusterInfo plotClusterInfo;
  PlotVFATInfo    plotVFATInfo;
  PlotEventInfo   PlotEventInfo;
  HistGetter plotter;
};

#endif

void PlotCSCAndGEM(std::string cscfileName="csc_forsync.root",std::string gemfilename = "gem_forsync.root",std::string outFileName = "plotCSCAndGEM_out.root"){
  GEMConfigInfo info;
  info.geoName               = "gemGeo.txt";
  info.vFATChanMapName       = "slot_table_904_june09.csv";
  Analyze a(cscfileName,gemfilename,&info);
  a.analyze();
  a.write(outFileName);
//  a.draw();
}
