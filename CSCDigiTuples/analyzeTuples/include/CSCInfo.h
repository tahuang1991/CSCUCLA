#ifndef CSCUCLA_CSCDIGITUPLES_ANALYZETUPLES_CSCINFO_H
#define CSCUCLA_CSCDIGITUPLES_ANALYZETUPLES_CSCINFO_H

#include <TFile.h>
#include <TString.h>
#include <TTree.h>
#include <assert.h>
#include <iostream>
#include "BaseTupleAnalyzer.h"


class EventInfo {
public:
  //Event Info
  int Event_EventNumber;
  int Event_RunNumber  ;
  int Event_LumiSection;
  int Event_BXCrossing ;
  void load(BaseTupleAnalyzer * analyzer) {
    analyzer->setBranchAddress("Event_EventNumber", &Event_EventNumber);
    analyzer->setBranchAddress("Event_RunNumber", &Event_RunNumber);
    analyzer->setBranchAddress("Event_LumiSection", &Event_LumiSection);
    analyzer->setBranchAddress("Event_BXCrossing", &Event_BXCrossing);
  }
};

class RecHitInfo {
public:
  std::vector<int>  * rh_id          ;
  std::vector<int>  * rh_lay         ;
  std::vector<float>* rh_pos_x       ;
  std::vector<float>* rh_pos_y       ;
  std::vector<int>  * rh_strip_chan1 ;
  std::vector<int>  * rh_strip_chan2 ;
  std::vector<int>  * rh_strip_chan3 ;
  std::vector<float>* rh_pos_strip   ;
  std::vector<int>  * rh_n_wiregroups;

  RecHitInfo() {
    rh_id           = new std::vector<int>;
    rh_lay          = new std::vector<int>;
    rh_pos_x        = new std::vector<float>;
    rh_pos_y        = new std::vector<float>;
    rh_strip_chan1  = new std::vector<int>;
    rh_strip_chan2  = new std::vector<int>;
    rh_strip_chan3  = new std::vector<int>;
    rh_pos_strip    = new std::vector<float>;
    rh_n_wiregroups = new std::vector<int>;
  }

  void load(BaseTupleAnalyzer * analyzer) {
    analyzer->setBranchAddress("rh_id"          ,&rh_id          );
    analyzer->setBranchAddress("rh_lay"         ,&rh_lay         );
    analyzer->setBranchAddress("rh_pos_x"       ,&rh_pos_x       );
    analyzer->setBranchAddress("rh_pos_y"       ,&rh_pos_y       );
    analyzer->setBranchAddress("rh_strip_chan1" ,&rh_strip_chan1 );
    analyzer->setBranchAddress("rh_strip_chan2" ,&rh_strip_chan2 );
    analyzer->setBranchAddress("rh_strip_chan3" ,&rh_strip_chan3 );
    analyzer->setBranchAddress("rh_pos_strip"   ,&rh_pos_strip   );
    analyzer->setBranchAddress("rh_n_wiregroups",&rh_n_wiregroups);
  }
};




#endif
