#include "../include/GEMGeoInfo.h"
#include <fstream>
#include <sstream>
#include<regex>

namespace CSCGEMTuples {


GEMGeo::GEMGeo(TString geoFileName) {
  vFat.resize(GEMGeoInfo::NVFAT);
  build(geoFileName);
}

void GEMGeo::build(TString geoFileName) {

  std::string line;
  std::ifstream inFile (geoFileName);
  if(!inFile.is_open()){
    std::cout <<"When trying to read geometry, could not find: " << geoFileName << std::endl;
    return;
  }


  std::regex typeEx ("^(\\S+)\\s+.*$");
  std::regex zdistEX ("^(\\S+)\\s+(\\S+).*$");
  std::regex vfatEX ("^(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+).*$");
  std::smatch sm;

  auto convFloat = [](float& in, std::string str) -> bool {try {in = stof(str);}catch (const std::invalid_argument& ia) {return false;} return true;};

  while(getline(inFile,line)){
//    std::cout << line << std::endl;
    TString thisLine = line;
    if (!std::regex_match (line,sm,typeEx) || sm.size() != 2) continue;
    if(sm[1] == "ZDIST"){
//      std::cout <<"--> ZDIST ";
      if(ZDIST > 0) throw std::invalid_argument("GEMGeo::buildGEMGeo -> Found more than one ZDist!");
      float v;
      if(!std::regex_match (line,sm,zdistEX) || sm.size() != 3 || !convFloat(v,sm[2]) )
        throw std::invalid_argument("GEMGeo::buildGEMGeo -> Do not understand your ZDIST value!");
//      std::cout << " (" << v <<")";
      ZDIST = v;
    } else if(sm[1] == "VFAT"){
//      std::cout <<"--> VFAT ";
      if(!std::regex_match (line,sm,vfatEX) || sm.size() != 9 )
        throw std::invalid_argument("GEMGeo::buildGEMGeo -> Do not understand your VFAT value!");

      float vFN = -1;
      if(!convFloat(vFN,sm[2])) throw std::invalid_argument("GEMGeo::buildGEMGeo -> Do not understand your VFAT number value!");
      if(vFat[vFN].isValid)throw std::invalid_argument("GEMGeo::buildGEMGeo -> You already cofigured this VFAT value!");

      if(!convFloat(vFat[vFN].x,sm[3]) || !convFloat(vFat[vFN].xMin,sm[4]) || !convFloat(vFat[vFN].xMax,sm[5])) throw std::invalid_argument("GEMGeo::build -> Do not understand your VFAT x location value!");
      if(!convFloat(vFat[vFN].y,sm[6]) || !convFloat(vFat[vFN].yMin,sm[7]) || !convFloat(vFat[vFN].yMax,sm[8])) throw std::invalid_argument("GEMGeo::build -> Do not understand your VFAT x location value!");
      if((vFat[vFN].xMin != vFat[vFN].xMax) && (vFat[vFN].yMin != vFat[vFN].yMax) )
        vFat[vFN].isValid = true;
//      geo.vFat[vFN].print(std::cout);
    }
//    std::cout << std::endl;
  }
}


int GEMGeoInfo::getGlobalStripX(int vFat, int vfStripNum){
  return 127 - vfStripNum + ((int) vFat/8)*128; //383 ->0
}
int GEMGeoInfo::getGlobalStripXLeftIs0(int vFat, int vfStripNum){
  return 2*128 - ((int) vFat/8)*128 +vfStripNum; // 0 -> 383
}
int GEMGeoInfo::getLocalStripX(int vFat, int globStripNum){
  return  127 + ((int) vFat/8)*128 - globStripNum;
}
int GEMGeoInfo::getRow(int vFat){
  return vFat % (NROWS);
}


GEMChannelMapping::GEMChannelMapping(const GEMConfigInfo& gemConfig){
  getStripMap(gemConfig);
  getGEBSlotMap(gemConfig);
}

int GEMChannelMapping::getGEBSlotIndex(const uint32_t& GEBChipID) {
  int indxslot = -1;
  //std::cout << "\nUsing slot file: " << slotFile_ << std::endl;
  for (int islot = 0; islot < 24; islot++) {
    if ( (GEBChipID & 0x0fff ) == gebSlotMap[islot] ) indxslot = islot;
  }//end for slot

  return (indxslot);
}

int GEMChannelMapping::getStripNumber(int vFat, int channel) {
  auto it = vFatStripMap.at(vFat).find(channel + 1);
  if (it ==  vFatStripMap.at(vFat).end() || it->second > 383){
    std::cout << vFat <<" "<< channel +1 <<" ";
    if(it !=  vFatStripMap.at(vFat).end()) std::cout << it->second ;
    std::cout << std::endl;
    return -1;
  }
  return vFatStripMap.at(vFat).find(channel+1)->second ;
}

void GEMChannelMapping::getGEBSlotMap(const GEMConfigInfo& gemConfig){
  gebSlotMap.clear();
  gebSlotMap.resize(GEMGeoInfo::NVFAT);
  std::ifstream ifile;
  ifile.open(gemConfig.dataDir+gemConfig.vFATChanMapName);

  if(!ifile.is_open()) {
    std::cout << "[GEMslotContents]: The file: " << gemConfig.dataDir+gemConfig.vFATChanMapName << " is missing.\n" << std::endl;
  };

  for (int row = 0; row < GEMGeoInfo::NCOLS; row++) {
    std::string line;
    std::getline(ifile, line);
    std::istringstream iss(line);
    if ( !ifile.good() ) break;
    for (int col = 0; col < GEMGeoInfo::NROWS; col++) {
      std::string val;
      std::getline(iss,val,',');
      std::stringstream convertor(val);
      convertor >> std::hex >> gebSlotMap[8*row+col];
    }
  }
  ifile.close();
}

void GEMChannelMapping::getStripMap(const GEMConfigInfo& gemConfig){
  vFatStripMap.clear();
  vFatStripMap.resize(GEMGeoInfo::NVFAT);
  for (int iV = 0; iV < GEMGeoInfo::NVFAT; iV++){
    std::string path = gemConfig.dataDir;
    if (iV < 2) {
      path += gemConfig.stripChannelMap_0_1;
    } else if (iV < 16) {
      path += gemConfig.stripChannelMap_2_15;
    } else if (iV < 18) {
      path += gemConfig.stripChannelMap_16_17;
    } else {
      path += gemConfig.stripChannelMap_18_23;
    }

    std::ifstream icsvfile_;
    icsvfile_.open(path);
    if(!icsvfile_.is_open()) {
      std::cout << "\nThe file: " << path << " is missing.\n" << std::endl;
      return;
    }
    for (int il = 0; il < 128; il++) {
      std::string line;
      std::getline(icsvfile_, line);
      //if (DEBUG) std::cout << "[gemTreeReader]: Read line : " << line << std::endl;
      std::istringstream iss(line);
      std::pair<int,int> map_;
      std::string val;
      std::getline(iss,val,',');
      //if (DEBUG) std::cout << "[gemTreeReader]: First val : " << val << std::endl;
      std::stringstream convertor(val);
      convertor >> std::dec >> map_.second;
      //if (DEBUG) std::cout << "[gemTreeReader]: First val recorded : " << map_.second << std::endl;
      std::getline(iss,val,',');
      //if (DEBUG) std::cout << "[gemTreeReader]: Second val : " << val << std::endl;
      convertor.str("");
      convertor.clear();
      convertor << val;
      convertor >> std::dec >> map_.first;

      if(vFatStripMap[iV].find(map_.first) != vFatStripMap[iV].end() ){
        std::cout << iV << " "<< map_.first <<" "<< map_.second << std::endl;
      }

      //if (DEBUG) std::cout << "[gemTreeReader]: Second val recorded : " << map_.first << std::endl;
      vFatStripMap[iV].insert(map_);
    }

}
}

}
