#if !defined(__CINT__) || defined(__MAKECINT__)
#include "include/BaseTupleAnalyzer.h"
#include "include/CSCInfo.h"
#include "include/Plotter.h"

using namespace std;

class Analyzer : public BaseTupleAnalyzer{
public:
  Analyzer(TString fileName, TString treeName) : BaseTupleAnalyzer(fileName,treeName){
    eventInfo.load(this);
    recHitInfo.load(this);

    bookHistos();
  }

  void bookHistos() {
    plotter.book2D("RecHitMap",";x[cm];y[cm]",600,-30,30,900,-90,90);
  }

  virtual void runAEvent() {
    for(unsigned int iH = 0; iH < recHitInfo.rh_pos_x->size(); ++iH){
      plotter.get2D("RecHitMap")->Fill((*recHitInfo.rh_pos_x)[iH],(*recHitInfo.rh_pos_y)[iH]);
    }
  }

  void write(TString fileName){ plotter.write(fileName);}

  Plotter plotter;
  EventInfo eventInfo;
  RecHitInfo recHitInfo;
};

#endif

void AnalyzeTuples(std::string fileName,std::string treeName = "CSCDigiTree",std::string outFileName = "plots.root"){
  Analyzer a(fileName,treeName);
  a.analyze();
  a.write(outFileName);
}
