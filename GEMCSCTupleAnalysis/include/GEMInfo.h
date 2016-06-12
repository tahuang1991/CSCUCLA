#ifndef CSCUCLA_CSCDIGITUPLES_ANALYZETUPLES_GEMINFO_H
#define CSCUCLA_CSCDIGITUPLES_ANALYZETUPLES_GEMINFO_H

#include "GEMGeoInfo.h"
#include "GEMConfigInfo.h"
#include <vector>

class Event;

namespace CSCGEMTuples {
class VFAT {
public:
  VFAT() {}
  VFAT(int vfat_idx, std::vector<int>& onStrips);
  int nStrips() const {return  strips.size();}
  int  idx =-1;
  std::vector<int> strips;
  int nRow = 0; //short side is row 0
};

class GEMCluster {
public:
  GEMCluster(int nRow, int idx,  int nStrips) : nRow(nRow), idx(idx),  nStrips(nStrips) {}
  int getNStrips() const {return nStrips;}
  int getFirstStrip() const {return idx;}
  int getLastStrip() const {return idx - nStrips +1;}
  int nRow= -1;
  int idx=-1; //In global strip number (383->0)
  int nStrips = 0;

};
class GEMInfo {
public:
  GEMInfo(const GEMConfigInfo& gemInfo);
  void build(Event * event);
  std::vector<VFAT> vFats;
  std::vector<GEMCluster> clusters;
  int BX = -1;
  GEMChannelMapping channelMapping;
private:
  void getClusters();

};
}

#endif
