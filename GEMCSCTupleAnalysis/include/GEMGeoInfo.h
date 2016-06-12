#ifndef CSCUCLA_GEMCSCTUPLEANALYSIS_GEMGEOINFO_H
#define CSCUCLA_GEMCSCTUPLEANALYSIS_GEMGEOINFO_H

#include <TFile.h>
#include <TString.h>
#include <TTree.h>
#include <iostream>
#include <string>

#include "GEMConfigInfo.h"

namespace CSCGEMTuples {

namespace GEMGeoInfo {
const int NVFAT = 24;
const int NROWS = 8;
const int NCOLS = 3;
//Global strip number for one row of VFATS [383-0] used for clustering
int getGlobalStripX(int vFat, int vfStripNum);
//Global strip number for one row of VFATS [0-383] used for plotting
int getGlobalStripXLeftIs0(int vFat, int vfStripNum);
//Go from global (for clustering) to vfat strip number
int getLocalStripX(int vFat, int globStripNum);
//Get VFAT row number
int getRow(int vFat);
}

struct VFATGeo {
  float x =-1;
  float y =-1;
  float xMin =-1;
  float xMax =-1;
  float yMin =-1;
  float yMax =-1;
  bool isValid =false;
  void print(std::ostream& os) const {os << "x: "<< x <<" (" << xMin <<","<<xMax<<") y: "<< y<<" (" << yMin <<","<<yMax<<")";}
};

class GEMGeo {
public:
  GEMGeo(TString geoFileName);
  double ZDIST =-1;
  std::vector<VFATGeo> vFat;
private:
  void build(TString geoFileName);
};

class GEMChannelMapping {
public:
  GEMChannelMapping(const GEMConfigInfo& gemConfig);
  //Gets the actual vFat slot number
  int getGEBSlotIndex(const uint32_t& GEBChipID);
  //Extracts phyiscal number [0-127] from channel number
  int getStripNumber(int vFat, int channel);

  std::vector<std::map<int,int> > vFatStripMap;
  std::vector<int> gebSlotMap;
private:
  //Reads map of GEB channels to slot numbers
  void getGEBSlotMap(const GEMConfigInfo& gemConfig);
  //Reads map of per vfat to physical location
  void getStripMap(const GEMConfigInfo& gemConfig);
};
}


#endif
