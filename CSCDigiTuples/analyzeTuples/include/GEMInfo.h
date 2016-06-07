#ifndef CSCUCLA_CSCDIGITUPLES_ANALYZETUPLES_GEMINFO_H
#define CSCUCLA_CSCDIGITUPLES_ANALYZETUPLES_GEMINFO_H

#include <TFile.h>
#include <TString.h>
#include <TTree.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include<regex>

#include "gemtreewriter/include/Event.h"
#include "dqm-root/include/GEMClusterization/GEMStrip.h"
#include "dqm-root/include/GEMClusterization/GEMStripCollection.h"
#include "dqm-root/include/GEMClusterization/GEMClusterizer.h"
#include "../gemdaq-testing/gemreadout/include/gem/readout/GEMslotContents.h"

namespace GEMInfo {
const int NVFAT = 24;
const int NROWS = 8;
const int NCOLS = 3;

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

struct GEMGeo {
  GEMGeo() {vFat.resize(NVFAT);}
  double ZDIST =-1;
  std::vector<VFATGeo> vFat;
};

void buildGEMGeo (TString geoFileName, GEMGeo& geo){
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
      if(geo.ZDIST > 0) throw std::invalid_argument("GEMGeo::buildGEMGeo -> Found more than one ZDist!");
      float v;
      if(!std::regex_match (line,sm,zdistEX) || sm.size() != 3 || !convFloat(v,sm[2]) )
        throw std::invalid_argument("GEMGeo::buildGEMGeo -> Do not understand your ZDIST value!");
//      std::cout << " (" << v <<")";
      geo.ZDIST = v;
    } else if(sm[1] == "VFAT"){
//      std::cout <<"--> VFAT ";
      if(!std::regex_match (line,sm,vfatEX) || sm.size() != 9 )
        throw std::invalid_argument("GEMGeo::buildGEMGeo -> Do not understand your VFAT value!");

      float vFN = -1;
      if(!convFloat(vFN,sm[2])) throw std::invalid_argument("GEMGeo::buildGEMGeo -> Do not understand your VFAT number value!");
      if(geo.vFat[vFN].isValid)throw std::invalid_argument("GEMGeo::buildGEMGeo -> You already cofigured this VFAT value!");

      if(!convFloat(geo.vFat[vFN].x,sm[3]) || !convFloat(geo.vFat[vFN].xMin,sm[4]) || !convFloat(geo.vFat[vFN].xMax,sm[5])) throw std::invalid_argument("GEMGeo::buildGEMGeo -> Do not understand your VFAT x location value!");
      if(!convFloat(geo.vFat[vFN].y,sm[6]) || !convFloat(geo.vFat[vFN].yMin,sm[7]) || !convFloat(geo.vFat[vFN].yMax,sm[8])) throw std::invalid_argument("GEMGeo::buildGEMGeo -> Do not understand your VFAT x location value!");
      if((geo.vFat[vFN].xMin != geo.vFat[vFN].xMax) && (geo.vFat[vFN].yMin != geo.vFat[vFN].yMax) )
        geo.vFat[vFN].isValid = true;
//      geo.vFat[vFN].print(std::cout);
    }
//    std::cout << std::endl;
  }
}

std::vector<std::map<int,int> > getStripMap(std::string csvPath = "/Users/nmccoll/Dropbox/Work/GitRepositories/gem-daq-code"){
  std::vector<std::map<int,int> > map;
  map.clear();
  map.resize(NVFAT);
  for (int iV = 0; iV < NVFAT; iV++){
    std::string path = csvPath;
    if (iV < 2) {
      path += "/gem-light-dqm/dqm-root/data/v2b_schema_chips0-1.csv";
    } else if (iV < 16) {
      path += "/gem-light-dqm/dqm-root/data/v2b_schema_chips2-15.csv";
    } else if (iV < 18) {
      path += "/gem-light-dqm/dqm-root/data/v2b_schema_chips16-17.csv";
    } else {
      path += "/gem-light-dqm/dqm-root/data/v2b_schema_chips18-23.csv";
    }

    std::ifstream icsvfile_;
    icsvfile_.open(path);
    if(!icsvfile_.is_open()) {
      std::cout << "\nThe file: " << path << " is missing.\n" << std::endl;
      return map;
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

      if(map[iV].find(map_.first) != map[iV].end() ){
        std::cout << iV << " "<< map_.first <<" "<< map_.second << std::endl;
      }

      //if (DEBUG) std::cout << "[gemTreeReader]: Second val recorded : " << map_.first << std::endl;
      map[iV].insert(map_);
    }


}

  return map;

}

int getStripNumber(int vFat, int channel){
  static  std::vector<std::map<int,int> > stripMap = getStripMap();
  auto it = stripMap.at(vFat).find(channel + 1);
  if (it ==  stripMap.at(vFat).end() || it->second > 383){
    std::cout << vFat <<" "<< channel +1 <<" ";
    if(it !=  stripMap.at(vFat).end()) std::cout << it->second ;
    std::cout << std::endl;
    return -1;
  }
  return stripMap.at(vFat).find(channel+1)->second ;
}

int getGlobalStripX(int vFat, int vfStripNum){
  return 127 - vfStripNum + ((int) vFat/8)*128; //383 ->0
}
int getGlobalStripXLeftIs0(int vFat, int vfStripNum){
  return 2*128 - ((int) vFat/8)*128 +vfStripNum; // 0 -> 383
}
int getLocalStripX(int vFat, int globStripNum){
  return  127 + ((int) vFat/8)*128 - globStripNum;
}
int getRow(int vFat){
  return vFat % (8);
}



class VFAT {
public:
  VFAT() {}
  VFAT(int vfat_idx, std::vector<int>& onStrips) : idx(vfat_idx), strips(onStrips), nRow(getRow(idx)) {std::sort(strips.begin(),strips.end());}
  int nStrips() const {return  strips.size();}
  std::vector<int> strips;
  int  idx =-1;
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

void getClusters(const std::vector<VFAT>& vFats, std::vector<GEMCluster>& clusters){
  clusters.clear();
  std::vector<std::vector<int> > stripsPerRow (NROWS, std::vector<int>(0));
//  std::cout << std::endl << "Start event!"<<std::endl;
  for(const auto& vf : vFats){
//    std:: cout <<vf.idx <<" "<< vf.nRow <<" ";
    for(unsigned int iS = 0; iS < vf.strips.size(); ++iS){
      int globalStripNumber = getGlobalStripX(vf.idx,vf.strips[iS]);
      stripsPerRow[vf.nRow].push_back(globalStripNumber);
//      std::cout <<"("<<vf.strips[iS]<<","<<globalStripNumber<<") ";
    }
//    std::cout << std::endl;
  }

  for(unsigned int iR = 0; iR < stripsPerRow.size(); ++iR){
    std::sort(stripsPerRow[iR].begin(),stripsPerRow[iR].end(), std::greater<int>());
//    std::cout << iR <<" -> ";
    for(unsigned int iC = 0; iC < stripsPerRow[iR].size(); ++iC){
//      std::cout << " " << stripsPerRow[iR][iC];
      if(iC == 0 || TMath::Abs(stripsPerRow[iR][iC -1] - stripsPerRow[iR][iC] ) > 1 ){
        clusters.emplace_back(iR,stripsPerRow[iR][iC],1 );
      } else{
        clusters.back().nStrips++;
      }
    }
//    std::cout << std::endl;
  }

//  for(unsigned int iC = 0; iC < clusters.size(); ++iC){
//    std::cout << "(" << clusters[iC].nRow <<","<<clusters[iC].getNStrips()<<","<<clusters[iC].getFirstStrip()<<","<<clusters[iC].getLastStrip() <<") ";
//  }
//  std::cout << std::endl;

}


  void fillGEMInfo(std::vector<VFAT>& vFats,std::vector<GEMCluster>& clusters, Event * event) {
    vFats.clear();
    std::vector<AMC13Event> v_amc13 = event->amc13s();
    std::vector<AMCdata> v_amc = v_amc13[0].amcs();
    std::vector<GEBdata> v_geb;
        v_geb = v_amc[0].gebs();
        for (Int_t j = 0; j < v_geb.size(); j++) {
          // create vector of VFATdata. For data format details look at Event.h
          std::vector<VFATdata> v_vfat;
          v_vfat = v_geb.at(j).vfats();

          // loop over vfats
          for (Int_t k = 0; k < v_vfat.size(); k++) {
            auto * m_vfat = &v_vfat[k];
            std::vector<int> firedChannels;
            uint16_t chan0xfFiredchip = 0;
            for (int chan = 0; chan < 128; ++chan) {
              if (chan < 64) {
                chan0xfFiredchip = ((m_vfat->lsData() >> chan) & 0x1);
                if (chan0xfFiredchip) {
                  firedChannels.push_back(chan);
                }
              } else {
                chan0xfFiredchip = ((m_vfat->msData() >> (chan - 64)) & 0x1);
                if (chan0xfFiredchip) {
                  firedChannels.push_back(chan);
                }
              }
            }

            static std::unique_ptr<gem::readout::GEMslotContents> slotInfo_ =
                std::unique_ptr<gem::readout::GEMslotContents>(
                    new gem::readout::GEMslotContents("slot_table.csv"));
            int sn_ = slotInfo_->GEBslotIndex(v_vfat[k].ChipID());
            std::vector<int> strips;
            if (firedChannels.size()) {

              for(unsigned int iC = 0; iC < firedChannels.size(); ++iC){
                int stripNumberOnVF = getStripNumber(sn_,firedChannels[iC]);
                strips.push_back(stripNumberOnVF);
              }
              VFAT fat(sn_,strips);
              vFats.push_back(fat);
            }
          }
        }
        getClusters(vFats,clusters);

//        for(unsigned)
//
//
//        GEMClusterizer clizer;
//        GEMClusterContainer cls = clizer.doAction(ieta->second);


  }



};



#endif
