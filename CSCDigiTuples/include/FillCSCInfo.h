#ifndef CSCUCLA_CSCDIGITUPLES_FILLCSCINFO_H
#define CSCUCLA_CSCDIGITUPLES_FILLCSCINFO_H

#include "FWCore/Framework/interface/Event.h"

#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCComparatorDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegmentCollection.h>
#include <DataFormats/CSCDigi/interface/CSCCLCTDigiCollection.h>
#include "CSCUCLA/CSCDigiTuples/include/CSCHelper.h"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"


typedef   unsigned char        size8 ; // 8 bit 0->255
typedef   unsigned short int   size16; //16 bit 0->65536
typedef   unsigned int         size  ; //32 bit 0->4294967296

class TreeContainer {
public:
  TreeContainer(TString fileName, TString treeName, TString treeTitle){
    file = new TFile(fileName, "RECREATE");
    tree = new TTree(treeName,treeTitle);
  }
  void write() {
    file->cd();
    tree->Write();
    file->Close();
    delete file;
  }

  void fill() {tree->Fill();}
  TFile * file;
  TTree * tree;

};

class FillInfo {
public:
  FillInfo(TreeContainer& tree) : fTree(&tree) {reset();};
  virtual ~FillInfo() {};
protected:
  virtual void reset() {};


  //Book single variable
  template<class T>
  void    book(const char *name, T& var, const char *type) { fTree->tree->Branch(name, &var, TString(name).Append("/").Append(type).Data()); }

  //Book vector
  template<class T>
  void    book(const char *name, std::vector<T>& varv)   { fTree->tree->Branch(name, &varv); }

  TreeContainer * fTree;

};


class FillEventInfo : public FillInfo {
public:

  FillEventInfo(TreeContainer& tree) :FillInfo(tree) {
    book("Event_EventNumber",Event_EventNumber,"I");
    book("Event_RunNumber"  ,Event_RunNumber  ,"I");
    book("Event_LumiSection",Event_LumiSection,"I");
    book("Event_BXCrossing" ,Event_BXCrossing ,"I");
  }
  virtual ~FillEventInfo() {};

private:
  int Event_EventNumber;
  int Event_RunNumber  ;
  int Event_LumiSection;
  int Event_BXCrossing ;

  virtual void reset(){
    Event_EventNumber  = -1;
    Event_RunNumber    = -1;
    Event_LumiSection  = -1;
    Event_BXCrossing   = -1;
  }

  public:

  void fill(const edm::Event& iEvent);

};


class FillRecHitInfo : public FillInfo {
public:

  FillRecHitInfo(TreeContainer& tree) :FillInfo(tree) {
    book("rh_id"          ,rh_id          );
    book("rh_lay"         ,rh_lay         );
    book("rh_pos_x"       ,rh_pos_x       );
    book("rh_pos_y"       ,rh_pos_y       );
    book("rh_strip_1"     ,rh_strip_1 );
    book("rh_strip_2"     ,rh_strip_2 );
    book("rh_strip_3"     ,rh_strip_3 );
    book("rh_pos_strip"   ,rh_pos_strip   );
    book("rh_n_wiregroups",rh_n_wiregroups);


  }
  virtual ~FillRecHitInfo() {};

private:
  std::vector<size16>   rh_id          ;
  std::vector<size8>    rh_lay         ;
  std::vector<float>    rh_pos_x       ;
  std::vector<float>    rh_pos_y       ;
  std::vector<size8>    rh_strip_1  ;
  std::vector<size8>    rh_strip_2  ;
  std::vector<size8>    rh_strip_3  ;
  std::vector<float>    rh_pos_strip   ;
  std::vector<size8>    rh_n_wiregroups;


  virtual void reset(){
    rh_id          .clear();
    rh_lay         .clear();
    rh_pos_x       .clear();
    rh_pos_y       .clear();
    rh_strip_1  .clear();
    rh_strip_2  .clear();
    rh_strip_3  .clear();
    rh_pos_strip   .clear();
    rh_n_wiregroups.clear();
  }

  public:

  void fill(const CSCRecHit2DCollection& recHits);

};

class FillStripInfo : public FillInfo {
public:

  FillStripInfo(TreeContainer& tree) :FillInfo(tree) {
    book("strip_id"           ,strip_id           );
    book("strip_lay"          ,strip_lay          );
    book("strip_number"       ,strip_number      );



  }
  virtual ~FillStripInfo() {};

private:
  std::vector<size16>   strip_id     ;
  std::vector<size8>    strip_lay    ;
  std::vector<size8>    strip_number ;


  virtual void reset(){
    strip_id     .clear();
    strip_lay    .clear();
    strip_number .clear();
  }

  public:

  void fill(const CSCStripDigiCollection& strips);

};

class FillCompInfo : public FillInfo {
public:

  FillCompInfo(TreeContainer& tree) :FillInfo(tree) {
    book("comp_id"      ,comp_id          );
    book("comp_lay"     ,comp_lay         );
    book("comp_strip"   ,comp_strip       );
    book("comp_comp"    ,comp_comp        );



  }
  virtual ~FillCompInfo() {};

private:
  std::vector<size16> comp_id    ;
  std::vector<size8>  comp_lay   ;
  std::vector<size8>  comp_strip ;
  std::vector<size8>  comp_comp  ;


  virtual void reset(){
    comp_id    .clear();
    comp_lay   .clear();
    comp_strip .clear();
    comp_comp  .clear();
  }

  public:

  void fill(const CSCComparatorDigiCollection& strips);

};


class FillWireInfo : public FillInfo {
public:

  FillWireInfo(TreeContainer& tree) :FillInfo(tree) {
    book("wire_id"     ,wire_id         );
    book("wire_lay"    ,wire_lay        );
    book("wire_grp"    ,wire_grp        );
    book("wire_time"   ,wire_time       );
    book("wire_bx"     ,wire_bx         );



  }
  virtual ~FillWireInfo() {};

private:
  std::vector<size16>     wire_id  ;
  std::vector<size8>      wire_lay ;
  std::vector<size8>      wire_grp ;
  std::vector<size8>      wire_time;
  std::vector<int>        wire_bx;


  virtual void reset(){
    wire_id   .clear();
    wire_lay  .clear();
    wire_grp  .clear();
    wire_time .clear();
    wire_bx   .clear();
  }

  public:

  void fill(const CSCWireDigiCollection& wires);

};


class FillLCTInfo : public FillInfo {
public:

  FillLCTInfo(TreeContainer& tree) :FillInfo(tree) {

    book("lct_id"          ,lct_id          );
    book("lct_quality"     ,lct_quality     );
    book("lct_pattern"     ,lct_pattern     );
    book("lct_bend"        ,lct_bend        );
    book("lct_keyWireGroup",lct_keyWireGroup);
    book("lct_keyHalfStrip",lct_keyHalfStrip);



  }
  virtual ~FillLCTInfo() {};

private:
   std::vector<size16> lct_id          ;
   std::vector<size8>  lct_quality     ;
   std::vector<size8>  lct_pattern     ;
   std::vector<size8>  lct_bend        ;
   std::vector<size8>  lct_keyWireGroup;
   std::vector<size8>  lct_keyHalfStrip;


  virtual void reset(){
    lct_id           .clear();
    lct_quality      .clear();
    lct_pattern      .clear();
    lct_bend         .clear();
    lct_keyWireGroup .clear();
    lct_keyHalfStrip .clear();
  }

  public:

  void fill(const CSCCorrelatedLCTDigiCollection& lcts);

};


class FillSegmentInfo : public FillInfo {
public:

  FillSegmentInfo(TreeContainer& tree) :FillInfo(tree) {

    book("segment_id"          ,segment_id         );
    book("segment_pos_x"       ,segment_pos_x      );
    book("segment_pos_y"       ,segment_pos_y      );
    book("segment_dxdz"        ,segment_dxdz  );
    book("segment_dydz"        ,segment_dydz    );
    book("segment_chisq"       ,segment_chisq      );
    book("segment_nHits"       ,segment_nHits      );
    book("segment_recHitIdx_1" ,segment_recHitIdx_1);
    book("segment_recHitIdx_2" ,segment_recHitIdx_2);
    book("segment_recHitIdx_3" ,segment_recHitIdx_3);
    book("segment_recHitIdx_4" ,segment_recHitIdx_4);
    book("segment_recHitIdx_5" ,segment_recHitIdx_5);
    book("segment_recHitIdx_6" ,segment_recHitIdx_6);



  }
  virtual ~FillSegmentInfo() {};

private:
     std::vector<size16>  segment_id             ;
     std::vector<float>   segment_pos_x          ;
     std::vector<float>   segment_pos_y          ;
     std::vector<float>   segment_dxdz      ;
     std::vector<float>   segment_dydz        ;
     std::vector<float>   segment_chisq          ;
     std::vector<size8>   segment_nHits          ;
     std::vector<size16>  segment_recHitIdx_1    ;
     std::vector<size16>  segment_recHitIdx_2    ;
     std::vector<size16>  segment_recHitIdx_3    ;
     std::vector<size16>  segment_recHitIdx_4    ;
     std::vector<size16>  segment_recHitIdx_5    ;
     std::vector<size16>  segment_recHitIdx_6    ;


  virtual void reset(){
    segment_id          .clear();
    segment_pos_x       .clear();
    segment_pos_y       .clear();
    segment_dxdz        .clear();
    segment_dydz        .clear();
    segment_chisq       .clear();
    segment_nHits       .clear();
    segment_recHitIdx_1 .clear();
    segment_recHitIdx_2 .clear();
    segment_recHitIdx_3 .clear();
    segment_recHitIdx_4 .clear();
    segment_recHitIdx_5 .clear();
    segment_recHitIdx_6 .clear();

  }

  public:

  void fill(const CSCSegmentCollection& segments, const CSCRecHit2DCollection * recHits = 0);
  size16 findRecHitIdx(const CSCRecHit2D& hit, const CSCRecHit2DCollection* allRecHits);

};

class FillCLCTInfo : public FillInfo {
public:

  FillCLCTInfo(TreeContainer& tree) :FillInfo(tree) {

    book("clct_id"         , clct_id        );
    book("clct_isvalid"    , clct_isvalid   );
    book("clct_quality"    , clct_quality   );
    book("clct_pattern"    , clct_pattern   );
    book("clct_stripType"  , clct_stripType );
    book("clct_bend"       , clct_bend      );
    book("clct_halfStrip"  , clct_halfStrip );
    book("clct_CFEB"       , clct_CFEB      );
    book("clct_BX"         , clct_BX        );
    book("clct_trkNumber"  , clct_trkNumber );
    book("clct_keyStrip"   , clct_keyStrip  );



  }
  virtual ~FillCLCTInfo() {};

private:
   std::vector<size16> clct_id          ;
   std::vector<size8>  clct_isvalid     ;
   std::vector<size8>  clct_quality     ;
   std::vector<size8>  clct_pattern     ;
   std::vector<size8>  clct_stripType   ;
   std::vector<size8>  clct_bend        ;
   std::vector<size8>  clct_halfStrip   ;
   std::vector<size8>  clct_CFEB        ;
   std::vector<size8>  clct_BX          ;
   std::vector<size8>  clct_trkNumber   ;
   std::vector<size8>  clct_keyStrip    ;


  virtual void reset(){
    clct_id         .clear();
    clct_isvalid    .clear();
    clct_quality    .clear();
    clct_pattern    .clear();
    clct_stripType  .clear();
    clct_bend       .clear();
    clct_halfStrip  .clear();
    clct_CFEB       .clear();
    clct_BX         .clear();
    clct_trkNumber  .clear();
    clct_keyStrip   .clear();
  }

  public:

  void fill(const CSCCLCTDigiCollection& clcts);

};


#endif /*CSCUCLA_CSCDIGITUPLES_FILLCSCINFO_H*/
