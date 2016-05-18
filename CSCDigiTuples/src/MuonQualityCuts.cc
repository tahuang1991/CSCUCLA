// user include files
#include "CSCUCLA/CSCDigiTuples/src/MuonQualityCuts.h"
//
// constructors and destructor
//
MuonQualityCuts::MuonQualityCuts(const edm::ParameterSet& iConfig
        )

{
    //now do what ever initialization is needed

}


MuonQualityCuts::~MuonQualityCuts()
{

    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}

bool MuonQualityCuts::isGoodMuon(const edm::Event& iEvent, reco::MuonCollection::const_iterator muon, Handle<reco::BeamSpot>) 
{
    cout << "Is this a good muon?" << endl;
    bool isGlobal=false;
    if (muon->isGlobalMuon() && muon->isTrackerMuon()) 
    {
        reco::TrackRef gm = muon->globalTrack();
        int trackerHits = gm->hitPattern().numberOfValidTrackerHits();
        double normalizedChi2 = gm->normalizedChi2();
        int pixelHits = gm->hitPattern().numberOfValidPixelHits();
        int muonHits = gm->hitPattern().numberOfValidMuonHits();
        cout << "pixelHits: " << pixelHits << " muonHits: " << muonHits << " trackerHits: " << trackerHits << " normalizedChi2: " << normalizedChi2 << endl;
        if (pixelHits>0 && muonHits>0 && trackerHits>10 && normalizedChi2<10) isGlobal=true;
    }
    return isGlobal;

}

bool MuonQualityCuts::isCosmicMuon(const edm::Event& iEvent, reco::MuonCollection::const_iterator muon) {
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
    //  if(isCosmic) std::cout<<"Cosmic"<<std::endl;
    return isCosmic;
}
