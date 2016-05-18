// user include files

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <DataFormats/CSCDigi/interface/CSCALCTDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCCLCTDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCComparatorDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigiCollection.h>
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "RecoMuon/TrackingTools/interface/MuonServiceProxy.h"
#include "RecoMuon/TrackingTools/interface/MuonSegmentMatcher.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/TrackReco/interface/DeDxData.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenuFwd.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "CSCUCLA/CSCDigiTuples/include/MuonQualityCuts.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCSegment.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "TFile.h"
#include "TTree.h"
#include "TProfile2D.h"
#include "TNtuple.h"
#include <memory>
#include <vector>
#include <iterator>
#include "TH1F.h"

using namespace std;
using namespace edm;
using namespace reco;


class MakeSimpleCSCTree : public edm::EDAnalyzer {
    public:
        explicit MakeSimpleCSCTree(const edm::ParameterSet&);
        ~MakeSimpleCSCTree();


    private:
        virtual void beginJob() {};
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void endJob() {};

        // ----------member data ---------------------------

        int chamberSerial( CSCDetId id );
        double FindAnodeTime(std::vector<CSCRecHit2D>::const_iterator  hiti,  const edm::Handle<CSCWireDigiCollection> cscWireDigi, double local_t0);

    private:
        edm::EDGetTokenT<CSCWireDigiCollection> wd_token;
        edm::EDGetTokenT<CSCStripDigiCollection> sd_token;
        edm::EDGetTokenT<CSCALCTDigiCollection> ad_token;
        edm::EDGetTokenT<CSCCLCTDigiCollection> cd_token;
        edm::EDGetTokenT<CSCCorrelatedLCTDigiCollection> ld_token;
        edm::EDGetTokenT<CSCComparatorDigiCollection> cod_token;

        edm::InputTag CSCSegmentTags_;

        int Event_EventNumber;
        int Event_RunNumber;
        int Event_LumiSection;
        int Event_BXCrossing;

        double Pt;
        double eta;
        double phi;

        //Segment data
        int Nseg;
        vector<int> segEc;
        vector<int> segSt;
        vector<int> segRi;
        vector<int> segCh;

        //RecHit data
        vector<int> rhId;
        vector<int> rhLay;
        vector<float> rhPos;
        vector<float> rhE;

        //LCT data
        vector<int> lctId;
        vector<vector<int>> lctQ;
        vector<vector<int>> lctPat;
        vector<vector<int>> lctKWG;
        vector<vector<int>> lctKHS;
        vector<vector<int>> lctBend;

        //CLCT data
        vector<int> clctId;
        vector<vector<int>> clctQ;
        vector<vector<int>> clctPat;
        vector<vector<int>> clctKHS;
        vector<vector<int>> clctCFEB;
        vector<vector<int>> clctBend;

        //ALCT data
        vector<int> alctId;
        vector<vector<int>> alctQ;
        vector<vector<int>> alctKWG;
        vector<vector<int>> alctAc;
        vector<vector<int>> alctPB;

        //Comparator data
        vector<int> compId;
        vector<int> compLay;
        vector<vector<int>> compStr;
        vector<vector<int>> compHS;
        vector<vector<vector<int>>> compTimeOn;

        //Wire Data
        vector<int> wireId;
        vector<int> wireLay;
        vector<vector<int>> wireGrp;
        vector<vector<vector<int> > > wireTimeOn;

        //Strip Data
        vector<int> stripId;
        vector<int> stripLay;
        vector<vector<int>> strip;
        vector<vector<vector<int>>> stripADCs;

        // double anodeTime;
        //    double stripTime;
        //    double wireTime;
        string filename;
        edm::EDGetTokenT<CSCSegmentCollection> allSegmentsCSCToken;

        TTree *tree;
        TH1F * hist;
        TH1F * ptmuon;
        TH1F * etamuon;
        TH1F * ptsamuon;
        TH1F * chambernumber;
        TFile *file;

};


MakeSimpleCSCTree::MakeSimpleCSCTree(const edm::ParameterSet& iConfig)
{
  wd_token = consumes<CSCWireDigiCollection>( iConfig.getParameter<edm::InputTag>("wireDigiTag") );
  sd_token = consumes<CSCStripDigiCollection>( iConfig.getParameter<edm::InputTag>("stripDigiTag") );
  ad_token = consumes<CSCALCTDigiCollection>( iConfig.getParameter<edm::InputTag>("alctDigiTag") );
  cd_token = consumes<CSCCLCTDigiCollection>( iConfig.getParameter<edm::InputTag>("clctDigiTag") );
  ld_token = consumes<CSCCorrelatedLCTDigiCollection>( iConfig.getParameter<edm::InputTag>("lctDigiTag") );
  cod_token = consumes<CSCComparatorDigiCollection>( iConfig.getParameter<edm::InputTag>("compDigiTag") );
}


MakeSimpleCSCTree::~MakeSimpleCSCTree()
{

}

void
MakeSimpleCSCTree::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

}

//define this as a plug-in
DEFINE_FWK_MODULE(MakeSimpleCSCTree);
