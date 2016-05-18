#ifndef HSCPAnalysis_MuonQualityCuts_H
#define HSCPAnalysis_MuonQualityCuts_H

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

using namespace std;
using namespace edm;
using namespace reco;

class MuonQualityCuts {
 public:
  MuonQualityCuts(const edm::ParameterSet&);
  ~MuonQualityCuts();
  bool isGoodMuon(const edm::Event& iEvent, reco::MuonCollection::const_iterator muon, Handle<reco::BeamSpot>);
  bool isCosmicMuon(const edm::Event& iEvent, reco::MuonCollection::const_iterator muon);
 private:

};

#endif

