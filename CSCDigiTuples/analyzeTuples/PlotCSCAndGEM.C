
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "include/BaseCSCAndGEMAnalyzer.h"
#include "include/GEMPlottingInfo.h"

#include<iostream>

using namespace std;

class Analyze : public AnalyzeBoth {
public:
  Analyze(TString cscFileName, TString gemFileName) : AnalyzeBoth(cscFileName,gemFileName)
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
  Analyze a(cscfileName,gemfilename);
  a.analyze();
  a.write(outFileName);
//  a.draw();
}
