// -*- C++ -*-
//
// Package:    FilterGoodMuons
// Class:      FilterGoodMuons
// 
/**\class FilterGoodMuons FilterGoodMuons.cc 

 Description: Filter good muons

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Amanda DEISHER ()
//         Created:  Fri Dec 11 20:37:30 CET 2009
// $Id$
//
//


// system include files

#include <memory>
// user include files

#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include <FWCore/Framework/interface/ConsumesCollector.h>
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include <DataFormats/CSCDigi/interface/CSCALCTDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigiCollection.h>
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
//#include "RecoLocalMuon/CSCValidation/src/MuonQualityCuts.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"



#include "DataFormats/CSCRecHit/interface/CSCSegment.h"


using namespace std;
using namespace edm;

//
// class declaration
//

class FilterGoodMuons : public edm::EDFilter {
   public:
      explicit FilterGoodMuons(const edm::ParameterSet&);
      ~FilterGoodMuons();
  

      int haloDirection(edm::Event&, const edm::EventSetup&);
      bool isGoodMuon(const edm::Event& iEvent, reco::MuonCollection::const_iterator muon, Handle<reco::BeamSpot>);
      bool isCosmicMuon(const edm::Event& iEvent, reco::MuonCollection::const_iterator muon);

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      


      // ----------member data ---------------------------

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
FilterGoodMuons::FilterGoodMuons(const edm::ParameterSet& pset)
{
   //now do what ever initialization is needed
  // Create the root file for the histograms
  //  MuonQualityCuts *muonQualityCuts;

}


FilterGoodMuons::~FilterGoodMuons()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
FilterGoodMuons::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{


   bool save_this_event = false;

   Handle<reco::MuonCollection> muons;
   iEvent.getByLabel("muons", muons);

      Handle<reco::BeamSpot> beamSpotHandle;
      iEvent.getByLabel("offlineBeamSpot", beamSpotHandle);

   
   for (reco::MuonCollection::const_iterator muon = muons->begin(); muon!= muons->end(); muon++) {

     //     std::cout<<"Muon "<<std::endl;
     if (!muon->standAloneMuon()) continue;
     //     std::cout<<"Pt threshold "<<muon->pt()<<std::endl;
     if (muon->pt()<10) continue;
     //     std::cout<<"Pt "<<std::endl;
     if (!isGoodMuon(iEvent, muon, beamSpotHandle) || isCosmicMuon(iEvent, muon)) continue;
     //     std::cout<<"Eta "<<std::endl;
     if (fabs(muon->eta())<1.2) continue;
     save_this_event=true;
   }
  return save_this_event;

}

// ------------ method called once each job just before starting event loop  ------------
void 
FilterGoodMuons::beginJob()
{

}

// ------------ method called once each job just after ending the event loop  ------------
void 
FilterGoodMuons::endJob() {
 

}

int
FilterGoodMuons::haloDirection(edm::Event& iEvent, const edm::EventSetup& iSetup){

  return 1;


}

bool FilterGoodMuons::isGoodMuon(const edm::Event& iEvent, reco::MuonCollection::const_iterator muon, Handle<reco::BeamSpot>) {
  //Handle<reco::BeamSpot> beamSpotHandle;                                                                                                                                                                                                   
  //iEvent.getByLabel("offlineBeamSpot", beamSpotHandle);                                                                                                                                                                                    

  edm::Handle< std::vector<reco::Vertex> > pvHandle;
  iEvent.getByLabel( "offlinePrimaryVertices", pvHandle );
  reco::Vertex primaryVertex = pvHandle->at(0);

  bool isGlobal=false;
  if (muon->isGlobalMuon() && muon->isTrackerMuon()) {
    reco::TrackRef gm = muon->globalTrack();
    //double SumPt = muon->isolationR03().sumPt;                                                                                                                                                                                             
    int trackerHits = gm->hitPattern().numberOfValidTrackerHits();
    double dxy = gm->dxy(primaryVertex.position());
    double normalizedChi2 = gm->normalizedChi2();
    int pixelHits = gm->hitPattern().numberOfValidPixelHits();
    //int nMatches = muon->numberOfMatchedStations();                                                                                                                                                                                        
    int muonHits = gm->hitPattern().numberOfValidMuonHits();
    if(pixelHits==0) std::cout<<"Pixel Hits"<<std::endl;
    if(muonHits==0) std::cout<<"Muon Hits"<<std::endl;
    if(fabs(dxy)>0.2) std::cout<<"d0"<<std::endl;
    if(trackerHits<11) std::cout<<"Tracker Hits"<<std::endl;
    if(normalizedChi2>10) std::cout<<"chi2"<<std::endl;
    if (pixelHits>0 && muonHits>0 && fabs(dxy)<0.2 && trackerHits>10 && normalizedChi2<10) isGlobal=true;
  }
  return isGlobal;

}

bool FilterGoodMuons::isCosmicMuon(const edm::Event& iEvent, reco::MuonCollection::const_iterator muon) {
  bool isCosmic=false;

  Handle<reco::MuonCollection> muons;
  iEvent.getByLabel("muons", muons);

  if (!muon->track().isNull()) {
    for (reco::MuonCollection::const_iterator muon2 = muons->begin(); muon2!= muons->end(); muon2++) {
      if (!muon2->isTrackerMuon()) continue;
      double angle = acos(-muon->track()->momentum().Dot(muon2->track()->momentum()/muon->track()->p()/muon2->track()->p()));
      if (muon!=muon2 && angle<0.02) isCosmic=true;
    }
  }
  if(isCosmic) std::cout<<"Cosmic"<<std::endl;
  return isCosmic;
}

//define this as a plug-in
DEFINE_FWK_MODULE(FilterGoodMuons);
