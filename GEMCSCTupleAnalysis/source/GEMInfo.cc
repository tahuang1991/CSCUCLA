
#include "../include/GEMInfo.h"
#include "../../gem-light-dqm/gemtreewriter/include/Event.h"

namespace CSCGEMTuples {
VFAT::VFAT(int vfat_idx, std::vector<int>& onStrips) : idx(vfat_idx), strips(onStrips), nRow(GEMGeoInfo::getRow(idx)) {std::sort(strips.begin(),strips.end());}

GEMInfo::GEMInfo(const GEMConfigInfo& gemInfo) :channelMapping(gemInfo) {

}
void GEMInfo::build(Event * event) {
  vFats.clear();

  std::vector<AMC13Event> v_amc13 = event->amc13s();
  std::vector<AMCdata> v_amc = v_amc13[0].amcs();
  BX = v_amc[0].BX();
  evtN = event->GetEventNumber();
  std::vector<GEBdata> v_geb;
      v_geb = v_amc[0].gebs();
      for (unsigned int j = 0; j < v_geb.size(); j++) {
        // create vector of VFATdata. For data format details look at Event.h
        std::vector<VFATdata> v_vfat;
        v_vfat = v_geb.at(j).vfats();

        // loop over vfats
        for (unsigned int k = 0; k < v_vfat.size(); k++) {
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

          int sn_ = channelMapping.getGEBSlotIndex(v_vfat[k].ChipID());
          std::vector<int> strips;
          if (firedChannels.size()) {

            for(unsigned int iC = 0; iC < firedChannels.size(); ++iC){
              int stripNumberOnVF = channelMapping.getStripNumber(sn_,firedChannels[iC]);
              strips.push_back(stripNumberOnVF);
            }
            VFAT fat(sn_,strips);
            vFats.push_back(fat);
          }
        }
      }
      getClusters();

}

void GEMInfo::getClusters() {
clusters.clear();
std::vector<std::vector<int> > stripsPerRow (GEMGeoInfo::NROWS, std::vector<int>(0));
//  std::cout << std::endl << "Start event!"<<std::endl;
for(const auto& vf : vFats){
//    std:: cout <<vf.idx <<" "<< vf.nRow <<" ";
  for(unsigned int iS = 0; iS < vf.strips.size(); ++iS){
    int globalStripNumber = GEMGeoInfo::getGlobalStripX(vf.idx,vf.strips[iS]);
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

}
