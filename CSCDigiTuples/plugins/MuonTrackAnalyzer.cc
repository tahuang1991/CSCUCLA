// -*- C++ -*-
//
// Package:    MuonTrackAnalyzer
// Class:      MuonTrackAnalyzer
// 
/**\class MuonTrackAnalyzer MuonTrackAnalyzer.cc Timing/NtupleMaker/src/MuonTrackAnalyzer.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/
//
// Original Author:  "Christopher Farrell"
//         Created:  Thu Jul 22 06:19:36 CDT 2010
// $Id$
//
//


// system include files
#include <memory>
#include <iterator>

// user include files
#include "CSCUCLA/CSCDigiTuples/include/MuonTrackAnalyzer.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
//


//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//

#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"


MuonTrackAnalyzer::MuonTrackAnalyzer(const edm::ParameterSet& iConfig)

{
	std::cout <<"MuonTrackAnalyzer constructor "<< std::endl;
    //now do what ever initialization is needed
    mc_token = consumes<reco::MuonCollection>( iConfig.getParameter<edm::InputTag>("muonCollection") );
    wd_token = consumes<CSCWireDigiCollection>( iConfig.getParameter<edm::InputTag>("wireDigiTag") );
    sd_token = consumes<CSCStripDigiCollection>( iConfig.getParameter<edm::InputTag>("stripDigiTag") );
    ad_token = consumes<CSCALCTDigiCollection>( iConfig.getParameter<edm::InputTag>("alctDigiTag") );
    cd_token = consumes<CSCCLCTDigiCollection>( iConfig.getParameter<edm::InputTag>("clctDigiTag") );
    ld_token = consumes<CSCCorrelatedLCTDigiCollection>( iConfig.getParameter<edm::InputTag>("lctDigiTag") );
    cod_token = consumes<CSCComparatorDigiCollection>( iConfig.getParameter<edm::InputTag>("compDigiTag") );
    obs_token = consumes<reco::BeamSpot>( iConfig.getParameter<edm::InputTag>("offlineBeamSpotTag") );
    tflct_token = consumes<CSCCorrelatedLCTDigiCollection>(iConfig.getParameter<edm::InputTag>("csctfDigiTag"));

    minPt     = iConfig.getParameter<double>("minPt");
    dR_seg_lct_     = iConfig.getParameter<double>("dR_seg_lct");

    //muonQualityCuts = new MuonQualityCuts(iConfig);

    evN = 0;

    edm::ParameterSet serviceParameters = iConfig.getParameter<edm::ParameterSet>("ServiceParameters");
    theService = new MuonServiceProxy(serviceParameters);

    edm::ParameterSet matchParameters = iConfig.getParameter<edm::ParameterSet>("MatchParameters");
    edm::ConsumesCollector iC  = consumesCollector();
    theMatcher = new MuonSegmentMatcher(matchParameters, iC);


    CSCSegmentTags_=matchParameters.getParameter<edm::InputTag>("CSCsegments");
    //CSCSegmentTags_ = iConfig.getParameter<edm::InputTag>("CSCsegments");
    allSegmentsCSCToken = iC.consumes<CSCSegmentCollection>(CSCSegmentTags_) ;
    //allSegmentsCSCToken = consumes<CSCSegmentCollection>(iConfig.getParameter<edm::InputTag>("cscSegmentTag"));

    filename = iConfig.getUntrackedParameter<std::string>("NtupleFileName");
    cout << endl << endl << "Filename " << filename << endl << endl;
    //file = new TFile(filename.c_str(), "RECREATE");
    //tree = new TTree("CSCDigiTree","Tree holding CSCDigis");
    /*
    hist = new TH1F("hist","hist",20,-0.5,19.5);
    chambernumber = new TH1F("chambernumber","chambernumber",600,0.5,600.5);
    ptmuon = new TH1F("ptmuon","ptmuon",200,0,200);
    etamuon = new TH1F("etamuon","etamuon",80,-4,4);
    ptmu1 = new TH1F("ptmu1","ptmu1",200,0,100);
    ptmu2 = new TH1F("ptmu2","ptmu2",200,0,100);
    ptsamuon = new TH1F("ptsamuon","ptsamuon",50,0,500);
    dimuonMos = new TH1F("dimuonMos","Mass of Dimuons;m_{#mu#mu} [GeV];Events",100,0,10);
    dimuonMss = new TH1F("dimuonMss","Mass of Dimuons;m_{#mu#mu} [GeV];Events",100,0,10);
    dimuon3M = new TH1F("dimuon3M","Mass of Dimuons;m_{#mu#mu} [GeV];Events",100,2.0,4.0);
    dimuonMos_1GS = new TH1F("dimuonMos_1GS","Mass of Dimuons;m_{#mu#mu} [GeV];Events",100,0,10);
    dimuon3M_1GS = new TH1F("dimuon3M_1GS","Mass of Dimuons;m_{#mu#mu} [GeV];Events",100,2.0,4.0);
    dimuonMos_1Gl = new TH1F("dimuonMos_1Gl","Mass of Dimuons;m_{#mu#mu} [GeV];Events",100,0,10);
    dimuon3M_1Gl = new TH1F("dimuon3M_1Gl","Mass of Dimuons;m_{#mu#mu} [GeV];Events",100,2.0,4.0);
    dimuonMos_1SA = new TH1F("dimuonMos_1SA","Mass of Dimuons;m_{#mu#mu} [GeV];Events",100,0,10);
    dimuon3M_1SA = new TH1F("dimuon3M_1SA","Mass of Dimuons;m_{#mu#mu} [GeV];Events",100,2.0,4.0);
    dimuonMos_2Gl = new TH1F("dimuonMos_2Gl","Mass of Dimuons;m_{#mu#mu} [GeV];Events",100,0,10);
    dimuon3M_2Gl = new TH1F("dimuon3M_2Gl","Mass of Dimuons;m_{#mu#mu} [GeV];Events",100,2.0,4.0);
    dimuonMos_2SA = new TH1F("dimuonMos_2SA","Mass of Dimuons;m_{#mu#mu} [GeV];Events",100,0,10);
    dimuon3M_2SA = new TH1F("dimuon3M_2SA","Mass of Dimuons;m_{#mu#mu} [GeV];Events",100,2.0,4.0);
    Nmuon_h = new TH1F("Nmuon_h","Number of Muons;Number of Muons;Events",11,-0.5,10.5);
    */



}


MuonTrackAnalyzer::~MuonTrackAnalyzer()
{
    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)
    //file->cd();
    //tree->Write();
    //hist->Write();
    //ptmuon->Write();
    //etamuon->Write();
    //Nmuon_h->Write();
    //chambernumber->Write();
    /*ptmu1->Write();
    ptmu2->Write();
    dimuonMos->Write();
    dimuonMss->Write();
    dimuon3M->Write();
    dimuonMos_1GS->Write();
    dimuon3M_1GS->Write();
    dimuonMos_1Gl->Write();
    dimuon3M_1Gl->Write();
    dimuonMos_1SA->Write();
    dimuon3M_1SA->Write();
    dimuonMos_2Gl->Write();
    dimuon3M_2Gl->Write();
    dimuonMos_2SA->Write();
    dimuon3M_2SA->Write();
    ptsamuon->Write();
	*/
    //file->Close();


}


void 
MuonTrackAnalyzer::clear(){

        segEc.clear();
        segSt.clear();
        segRi.clear();
        segCh.clear();
	segChamberType.clear();
	segId.clear();
        segeta.clear();
        segphi.clear();
        segx.clear();
        segy.clear();
        segz.clear();

        rhId.clear();
        rhLay.clear();
        rhPos.clear();
        rhE.clear();
        rhMax.clear();

        lctId.clear();
        lctQ.clear();
        lctBX.clear();
        lctPat.clear();
        lctKWG.clear();
        lctKHS.clear();
        lctBend.clear();
        lcteta.clear();
        lctphi.clear();
	dRSegLCT.clear();
        lcteta_fit.clear();
        lctphi_fit.clear();

        tflctId.clear();
        tflctQ.clear();
        tflctPat.clear();
        tflctKWG.clear();
        tflctKHS.clear();
        tflctBend.clear();

        clctId.clear();
        clctQ.clear();
        clctBX.clear();
        clctPat.clear();
        clctKHS.clear();
        clctCFEB.clear();
        clctBend.clear();

        alctId.clear();
        alctQ.clear();
        alctBX.clear();
        alctKWG.clear();
        alctAc.clear();
        alctPB.clear();

        compId.clear();
        compLay.clear();
        compStr.clear();
        compHS.clear();
        compTimeOn.clear();

        wireId.clear();
        wireLay.clear();
        wireGrp.clear();
        wireTimeOn.clear();

        stripId.clear();
        stripLay.clear();
        strip.clear();
        stripADCs.clear();

}

//
// member functions
//

// ------------ method called to for each event  ------------
    void
MuonTrackAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;
    using namespace std;

    Event_EventNumber     = iEvent.id().event();
    Event_RunNumber       = iEvent.id().run();
    Event_LumiSection     = iEvent.eventAuxiliary().luminosityBlock();
    Event_BXCrossing      = iEvent.eventAuxiliary().bunchCrossing();

    evN++;

    // Get the CSC Geometry :
    ESHandle<CSCGeometry> cscGeom;
    iSetup.get<MuonGeometryRecord>().get(cscGeom);
    theCSC = cscGeom.product();

    edm::Handle<CSCWireDigiCollection> cscWireDigi;
    iEvent.getByToken(wd_token,cscWireDigi);

    edm::Handle<CSCStripDigiCollection> cscStripDigi;
    iEvent.getByToken(sd_token,cscStripDigi);

    Handle<reco::MuonCollection> muons;
    //Handle<vector<pat::Muon>> muons;
    iEvent.getByToken(mc_token, muons);

    edm::Handle<CSCSegmentCollection> allSegmentsCSC;
    iEvent.getByToken(allSegmentsCSCToken, allSegmentsCSC);
    //hist->Fill(allSegmentsCSC->size());
    //if(allSegmentsCSC->size()>14) return;

    edm::Handle<CSCALCTDigiCollection> cscALCTDigi;
    iEvent.getByToken(ad_token, cscALCTDigi);

    edm::Handle<CSCCLCTDigiCollection> cscCLCTDigi;
    iEvent.getByToken(cd_token, cscCLCTDigi);

    edm::Handle<CSCCorrelatedLCTDigiCollection> cscLCTDigi;
    iEvent.getByToken(ld_token, cscLCTDigi);

    edm::Handle<CSCCorrelatedLCTDigiCollection> tfLCTs;
    iEvent.getByToken(tflct_token, tfLCTs);

    edm::Handle<CSCComparatorDigiCollection> compDigi;
    iEvent.getByToken(cod_token, compDigi);

    /*edm::Handle<reco::BeamSpot> beamSpotHandle;
    iEvent.getByToken(obs_token, beamSpotHandle);


	reco::BeamSpot beamSpot;
	if ( beamSpotHandle.isValid() )
	{
		beamSpot = *beamSpotHandle;

	} else
	{
		edm::LogInfo("MyAnalyzer")
		  << "No beam spot available from EventSetup \n";
	}

	beamspotx = beamSpot.x0();
	beamspoty = beamSpot.y0();
	beamspotz = beamSpot.z0();
	*/


    int Nmuon = 0;
    int N_SAmuon = 0;
    vector<TLorentzVector> muP4;
    vector<int> muQ;
    vector<bool> muGlobal;
    vector<bool> muSA;
    vector<bool> muTracker;
    TLorentzVector sp4(0.0,0.0,0.0,0.0);

    for (reco::MuonCollection::const_iterator muon = muons->begin(); muon!= muons->end(); muon++) 
    {
        Nmuon++;
        //ptmuon->Fill(muon->pt());
        //etamuon->Fill(muon->eta());
        TLorentzVector muBuf;
        muBuf.SetPtEtaPhiE(muon->pt(),muon->eta(),muon->phi(),muon->energy());
        muP4.push_back(muBuf);
        muQ.push_back(muon->charge());
        muGlobal.push_back(muon->isGlobalMuon());
        if(!muon->standAloneMuon()) muSA.push_back(false);
        else muSA.push_back(true);
        muTracker.push_back(muon->isTrackerMuon());
        if (!muon->standAloneMuon()) continue;
		/*
        if(N_SAmuon < 2) 
        {
            TLorentzVector pp;
            pp.SetPtEtaPhiE(muon->pt(),muon->eta(),muon->phi(),muon->energy());
            sp4 += pp;
        }*/
        N_SAmuon++;
		
       // ptsamuon->Fill(muon->standAloneMuon()->pt());
    }
     
    std::cout <<"number of muons "<< Nmuon << " standalone muons "<< N_SAmuon << std::endl;
    //Nmuon_h->Fill(Nmuon);
	/*
    int mu1Ios = -9;
    int mu2Ios = -9;
    int mu1Iss = -9;
    int mu2Iss = -9;
    for(int i = 0; i < int(muQ.size());i++)
    {
        for(int j = i + 1; j < int(muQ.size()); j++)
        {
            if(mu1Ios == -9 && (muGlobal.at(i) && muGlobal.at(j)) && muQ.at(i) != muQ.at(j))
            {
                mu1Ios = i;
                mu2Ios = j;
            }
            if(mu1Iss == -9 && (muGlobal.at(i) && muGlobal.at(j)) && muQ.at(i) == muQ.at(j))
            {
                mu1Iss = i;
                mu2Iss = j;
            }
        }
    }
    if(mu1Ios == -9 && mu1Iss == -9) return;
    if(muGlobal.at(mu1Ios) || muGlobal.at(mu2Ios))
    {
        dimuonMos_1Gl->Fill((muP4.at(mu1Ios) + muP4.at(mu2Ios)).M());
        dimuon3M_1Gl->Fill((muP4.at(mu1Ios) + muP4.at(mu2Ios)).M());
    }
    if(muGlobal.at(mu1Ios) && muGlobal.at(mu2Ios))
    {
        dimuonMos_2Gl->Fill((muP4.at(mu1Ios) + muP4.at(mu2Ios)).M());
        dimuon3M_2Gl->Fill((muP4.at(mu1Ios) + muP4.at(mu2Ios)).M());
    }
    if(muSA.at(mu1Ios) || muSA.at(mu2Ios))
    {
        dimuonMos_1SA->Fill((muP4.at(mu1Ios) + muP4.at(mu2Ios)).M());
        dimuon3M_1SA->Fill((muP4.at(mu1Ios) + muP4.at(mu2Ios)).M());
    }
    if(muSA.at(mu1Ios) && muSA.at(mu2Ios))
    {
        dimuonMos_2SA->Fill((muP4.at(mu1Ios) + muP4.at(mu2Ios)).M());
        dimuon3M_2SA->Fill((muP4.at(mu1Ios) + muP4.at(mu2Ios)).M());
    }
    if( (muSA.at(mu1Ios) || muSA.at(mu2Ios)) && (muGlobal.at(mu1Ios) || muGlobal.at(mu2Ios)) )
    {
        dimuonMos_1GS->Fill((muP4.at(mu1Ios) + muP4.at(mu2Ios)).M());
        dimuon3M_1GS->Fill((muP4.at(mu1Ios) + muP4.at(mu2Ios)).M());
    }

    float massJpsi = 3.0969;
    float massWin = 0.1;
    bool inMos = false;
    bool inMss = false;
    if(mu1Ios >= 0)
    {
        if( ( (muP4.at(mu1Ios) + muP4.at(mu2Ios)).M() < massJpsi - massWin ) || ( (muP4.at(mu1Ios) + muP4.at(mu2Ios)).M() > massJpsi + massWin ) ) inMos = true;
        ptmu1->Fill(muP4.at(mu1Ios).Pt());
        ptmu2->Fill(muP4.at(mu2Ios).Pt());
        dimuonMos->Fill((muP4.at(mu1Ios) + muP4.at(mu2Ios)).M());
        dimuon3M->Fill((muP4.at(mu1Ios) + muP4.at(mu2Ios)).M());
    }
    if(mu1Iss >= 0)
    {
        if( ( (muP4.at(mu1Iss) + muP4.at(mu2Iss)).M() < massJpsi - massWin ) || ( (muP4.at(mu1Iss) + muP4.at(mu2Iss)).M() > massJpsi + massWin ) ) inMss = true;
        ptmu1->Fill(muP4.at(mu1Iss).Pt());
        ptmu2->Fill(muP4.at(mu2Iss).Pt());
        dimuonMss->Fill((muP4.at(mu1Iss) + muP4.at(mu2Iss)).M());
    }
    if(!(inMos || inMss)) return;
    cout << "Event " << evN << " Processed!" << endl;
    cout << "inMos: " << inMos << " inMss: " << inMss << " mu1Ios: " << mu1Ios << " mu2Ios: " << mu2Ios << " mu1Iss: " << mu1Iss << " mu2Iss: " << mu2Iss << endl;
	*/

    int ind = -1;
    vector<int> ch_serialID;
    //Loop over muons applying the selection used for the timing analysis
    for (reco::MuonCollection::const_iterator muon = muons->begin(); muon!= muons->end(); muon++) 
    {
        ind++;
        os = false;
        ss = false;
		clear();
        //if( !((inMos && (ind==mu1Ios || ind==mu2Ios)) || (inMss && (ind==mu1Iss || ind==mu2Iss))) ) continue;
        //if(ind==mu1Ios || ind==mu2Ios) os = true;
        //if(ind==mu1Iss || ind==mu2Iss) ss = true;

        if (!muon->standAloneMuon()) continue;
		std::cout <<"muon pt "<< muon->pt() <<" eta "<< muon->eta() << std::endl;
        if (muon->pt()<minPt) continue;
        //if (!muonQualityCuts->isGoodMuon(iEvent, muon, beamSpotHandle)) continue;

        Pt=muon->pt();
        eta=muon->eta();
        phi=muon->phi();
        q=muon->charge();

        //Match selected muons to CSCSegments
        vector<const CSCSegment*> range = theMatcher->matchCSC(*muon->standAloneMuon(),iEvent);

        Nseg = range.size();


		std::cout <<"matched CSSegment size  "<< range.size() << std::endl;

        for (vector<const CSCSegment*>::iterator rechit = range.begin(); rechit!=range.end(); ++rechit) 
        {
            // Create the ChamberId
            DetId id = (*rechit)->geographicalId();

            CSCDetId chamberId(id.rawId());
            int chamber = chamberSerial(chamberId);
			
            segEc.push_back(chamberId.endcap());
            segSt.push_back(chamberId.station());
            segRi.push_back(chamberId.ring());
            segCh.push_back(chamberId.chamber());
	    segChamberType.push_back(chamberId.iChamberType());
            segId.push_back(chamber);
	    std::cout <<"matched CSSegment "<< *(*rechit) << std::endl;
            GlobalPoint seggp = theCSC->idToDet((*rechit)->cscDetId())->surface().toGlobal((*rechit)->localPosition());
	    segeta.push_back(seggp.eta());
	    segphi.push_back(seggp.phi());
	    std::cout <<"segment eta "<< seggp.eta() <<" phi "<< seggp.phi() << std::endl;
			


            const vector<CSCRecHit2D> hits2d = (*rechit)->specificRecHits();
            for (vector<CSCRecHit2D>::const_iterator hiti=hits2d.begin(); hiti!=hits2d.end(); hiti++)
            {
                DetId idd = (hiti)->geographicalId();
                CSCDetId hitID(idd.rawId());
		std::cout <<"\t recHits "<< *hiti << std::endl;
                //int nStr = hiti->nStrips();
                //int nWireG = hiti->nWireGroups();
                int idBuf = chamberSerial(hitID);
                if(idBuf != chamber) continue;
                rhId.push_back(idBuf);
                rhLay.push_back(hitID.layer());

                float rhMaxBuf = -999.0;
                int centerID = hiti->nStrips()/2;

                for(int tI = 0; tI < int(hiti->nTimeBins()); tI++)
                {
                    if(hiti->adcs(centerID,tI) > rhMaxBuf) rhMaxBuf = hiti->adcs(centerID,tI);
                }
                int centerStr = hiti->channels(centerID);
                float rhPosBuf = float(centerStr) + hiti->positionWithinStrip();
                float rhEBuf = hiti->energyDepositedInLayer();
                rhPos.push_back(rhPosBuf);
                rhE.push_back(rhEBuf);
                rhMax.push_back(rhMaxBuf);
            }

            // Check to see if chamber has already been extracted
            bool chDone = false;
            for(vector<int>::iterator chid = ch_serialID.begin(); chid!=ch_serialID.end(); ++chid)
            {
		std::cout <<"already looped the chamber "<< *chid <<" current chamber "<< chamber << std::endl;
                if((*chid) == chamber) chDone = true;
            }

            if(chDone) continue;
            ch_serialID.push_back(chamber);
            //chambernumber->Fill(chamber);

	    bool foundLCT = false;
	    float mindR = 99.0;
	    GlobalPoint gpLCT;
	    CSCCorrelatedLCTDigi matchedLCT; 
            // Extract LCT for all strips in this chamber
            for (CSCCorrelatedLCTDigiCollection::DigiRangeIterator lctDigi_id=cscLCTDigi->begin(); lctDigi_id!=cscLCTDigi->end(); lctDigi_id++)
            {
                CSCDetId lctID = (*lctDigi_id).first;
		//std::cout <<"to match LCT CSCid "<<  lctID << std::endl;
                int idBuf = chamberSerial(lctID);
                if(idBuf != chamber) continue;

                //vector<int> lctQBuf;
                //vector<int> lctPatBuf;
                //vector<int> lctKWGBuf;
                //vector<int> lctKHSBuf;
                //vector<int> lctBendBuf;

                const CSCCorrelatedLCTDigiCollection::Range& range =(*lctDigi_id).second;
                for(CSCCorrelatedLCTDigiCollection::const_iterator digiItr = range.first; digiItr != range.second; ++digiItr)
                {
			GlobalPoint gptmp = getCSCCorrelatedLCTDigiPoint(lctID.rawId(), *digiItr);
			float dR = reco::deltaR(seggp.eta(), seggp.phi(), gptmp.eta(), gptmp.phi());
			if (dR > dR_seg_lct_) continue;

			if (dR < mindR){
				matchedLCT = *digiItr;
				mindR = dR;
				foundLCT = true;
				gpLCT = gptmp;
				std::cout <<"Found matched LCT " <<  *digiItr <<" eta "<< gpLCT.eta()<<" phi "<< gpLCT.phi() << std::endl;
			}
                    //lctQBuf.push_back((*digiItr).getQuality());
                    //lctPatBuf.push_back((*digiItr).getPattern());
                    //lctKWGBuf.push_back((*digiItr).getKeyWG());
                    //lctKHSBuf.push_back((*digiItr).getStrip());
                    //lctBendBuf.push_back((*digiItr).getBend());
                }
                //lctId.push_back(idBuf);

                //lctQ.push_back(lctQBuf);
                //lctPat.push_back(lctPatBuf);
                //lctKWG.push_back(lctKWGBuf);
                //lctKHS.push_back(lctKHSBuf);
                //lctBend.push_back(lctBendBuf);
            }

	    if (foundLCT){//fill LCT info 
		cout <<"matchedLCT "<< matchedLCT << endl;
	    	lctId.push_back(chamber);
	    	lctQ.push_back(matchedLCT.getQuality());
	    	lctBX.push_back(matchedLCT.getBX());
	    	lctPat.push_back(matchedLCT.getPattern());
	    	lctKWG.push_back(matchedLCT.getKeyWG());
	    	lctKHS.push_back(matchedLCT.getStrip());
	    	lctBend.push_back(matchedLCT.getBend());
	    	lcteta.push_back(gpLCT.eta());
	    	lctphi.push_back(gpLCT.phi());
	    	dRSegLCT.push_back(mindR);

	    }else //skip this CSC segment 
		continue;

			/*
            //Extact tfLCTs in this event
            for (CSCCorrelatedLCTDigiCollection::DigiRangeIterator lctDigi_id=tfLCTs->begin(); lctDigi_id!=tfLCTs->end(); lctDigi_id++)
            {
                CSCDetId tflctID = (*lctDigi_id).first;
                int idBuf = chamberSerial(tflctID);
                if(idBuf != chamber) continue;
                tflctId.push_back(idBuf);

                vector<int> tflctQBuf;
                vector<int> tflctPatBuf;
                vector<int> tflctKWGBuf;
                vector<int> tflctKHSBuf;
                vector<int> tflctBendBuf;

                const CSCCorrelatedLCTDigiCollection::Range& range =(*lctDigi_id).second;
                for(CSCCorrelatedLCTDigiCollection::const_iterator digiItr = range.first; digiItr != range.second; ++digiItr)
                {
                    tflctQBuf.push_back((*digiItr).getQuality());
                    tflctPatBuf.push_back((*digiItr).getPattern());
                    tflctKWGBuf.push_back((*digiItr).getKeyWG());
                    tflctKHSBuf.push_back((*digiItr).getStrip());
                    tflctBendBuf.push_back((*digiItr).getBend());
                }
                tflctQ.push_back(tflctQBuf);
                tflctPat.push_back(tflctPatBuf);
                tflctKWG.push_back(tflctKWGBuf);
                tflctKHS.push_back(tflctKHSBuf);
                tflctBend.push_back(tflctBendBuf);
            }*/

            // Extract CLCT for all strips in this chamber
	    CSCCLCTDigi matchedCLCT;
	    bool foundCLCT =false;;
            for (CSCCLCTDigiCollection::DigiRangeIterator clctDigi_id=cscCLCTDigi->begin(); clctDigi_id!=cscCLCTDigi->end(); clctDigi_id++)
            {
                CSCDetId clctID = (*clctDigi_id).first;
                int idBuf = chamberSerial(clctID);
                if(idBuf != chamber) continue;

                //vector<int> clctQBuf;
                //vector<int> clctPatBuf;
                //vector<int> clctKHSBuf;
                //vector<int> clctCFEBBuf;
                //vector<int> clctBendBuf;

                const CSCCLCTDigiCollection::Range& range =(*clctDigi_id).second;
                for(CSCCLCTDigiCollection::const_iterator digiItr = range.first; digiItr != range.second; ++digiItr)
                {
		    cout << "CLCT "<< (*digiItr) << endl;
		    if ((*digiItr).getKeyStrip() ==  matchedLCT.getStrip()){
			matchedCLCT = (*digiItr);
			foundCLCT = true;
		    }
                    //clctQBuf.push_back((*digiItr).getQuality());
                    //clctPatBuf.push_back((*digiItr).getPattern());
                    //clctKHSBuf.push_back((*digiItr).getStrip());
                    //clctCFEBBuf.push_back((*digiItr).getCFEB());
                    //clctBendBuf.push_back((*digiItr).getBend());
                }
                //clctQ.push_back(clctQBuf);
                //clctPat.push_back(clctPatBuf);
                //clctKHS.push_back(clctKHSBuf);
                //clctCFEB.push_back(clctCFEBBuf);
                //clctBend.push_back(clctBendBuf);
            }
	    if (foundCLCT){
                clctId.push_back(chamber);
		clctQ.push_back(matchedCLCT.getQuality());
		clctBX.push_back(matchedCLCT.getBX());
                clctPat.push_back(matchedCLCT.getPattern());
                clctKHS.push_back(matchedCLCT.getKeyStrip());
                clctCFEB.push_back(matchedCLCT.getCFEB());
                clctBend.push_back(matchedCLCT.getBend());
	    }


            // Extract ALCT for all strips in this chamber
	    CSCALCTDigi matchedALCT;
	    bool foundALCT = false;
            for (CSCALCTDigiCollection::DigiRangeIterator alctDigi_id=cscALCTDigi->begin(); alctDigi_id!=cscALCTDigi->end(); alctDigi_id++)
            {
                CSCDetId alctID = (*alctDigi_id).first;
                int idBuf = chamberSerial(alctID);
                if(idBuf != chamber) continue;
                //alctId.push_back(idBuf);

                //vector<int> alctQBuf;
                //vector<int> alctKWGBuf;
                //vector<int> alctAcBuf;
                //vector<int> alctPBBuf;

                const CSCALCTDigiCollection::Range& range =(*alctDigi_id).second;
                for(CSCALCTDigiCollection::const_iterator digiItr = range.first; digiItr != range.second; ++digiItr)
                {
		    cout << "ALCT "<< (*digiItr) << endl;
		    if ((*digiItr).getKeyWG() ==  matchedLCT.getKeyWG()){
			matchedALCT = (*digiItr);
			foundALCT = true;
		    }
                    //alctQBuf.push_back((*digiItr).getQuality());
                    //alctKWGBuf.push_back((*digiItr).getKeyWG());
                    //alctAcBuf.push_back((*digiItr).getAccelerator());
                    //alctPBBuf.push_back((*digiItr).getCollisionB());
                }
                //alctQ.push_back(alctQBuf);
                //alctKWG.push_back(alctKWGBuf);
                //alctAc.push_back(alctAcBuf);
                //alctPB.push_back(alctPBBuf);
            }
	    if (foundALCT){
		alctId.push_back(chamber);
                alctQ.push_back(matchedALCT.getQuality());
                alctBX.push_back(matchedALCT.getBX());
                alctKWG.push_back(matchedALCT.getKeyWG());
                alctAc.push_back(matchedALCT.getAccelerator());
                alctPB.push_back(matchedALCT.getCollisionB());
	    }

	    /*
            // Extract Comparator Data
            for (CSCComparatorDigiCollection::DigiRangeIterator compDigi_id=compDigi->begin(); compDigi_id!=compDigi->end(); compDigi_id++)
            {
                CSCDetId compID = (*compDigi_id).first;
                int idBuf = chamberSerial(compID);
                if(idBuf != chamber) continue;
                compId.push_back(idBuf);
                compLay.push_back(compID.layer());

                vector<int> compStrBuf;
                vector<int> compHSBuf;
                vector<vector<int> > compTimeOnBuf;

                const CSCComparatorDigiCollection::Range& range =(*compDigi_id).second;
                for(CSCComparatorDigiCollection::const_iterator digiItr = range.first; digiItr != range.second; ++digiItr)
                {
                    compStrBuf.push_back((*digiItr).getStrip());
                    compHSBuf.push_back((*digiItr).getComparator());
                    compTimeOnBuf.push_back((*digiItr).getTimeBinsOn());
                }
                compStr.push_back(compStrBuf);
                compHS.push_back(compHSBuf);
                compTimeOn.push_back(compTimeOnBuf);
            }

            // Extract data from WireDigis
            for (CSCWireDigiCollection::DigiRangeIterator wireDigi_id=cscWireDigi->begin(); wireDigi_id!=cscWireDigi->end(); wireDigi_id++)
            {
                CSCDetId wireID = (*wireDigi_id).first;
                int idBuf = chamberSerial(wireID);
                if(idBuf != chamber) continue;
                wireId.push_back(idBuf);
                wireLay.push_back(wireID.layer());

                vector<int> wireGrpBuf;
                vector<vector<int> > wireTimeOnBuf;

                const CSCWireDigiCollection::Range& range =(*wireDigi_id).second;
                for(CSCWireDigiCollection::const_iterator digiItr = range.first; digiItr != range.second; ++digiItr)
                {
                    wireGrpBuf.push_back((*digiItr).getWireGroup());
                    wireTimeOnBuf.push_back((*digiItr).getTimeBinsOn());
                }
                wireGrp.push_back(wireGrpBuf);
                wireTimeOn.push_back(wireTimeOnBuf);
            }

            // Extract ADCCounts for all strips in this chamber
            for (CSCStripDigiCollection::DigiRangeIterator stripDigi_id=cscStripDigi->begin(); stripDigi_id!=cscStripDigi->end(); stripDigi_id++)
            {
                CSCDetId stripID = (*stripDigi_id).first;
                int idBuf = chamberSerial(stripID);
                if(idBuf != chamber) continue;
                stripId.push_back(idBuf);
                stripLay.push_back(stripID.layer());

                vector<int> stripBuf;
                vector<vector<int>> stripADCBuf;

                vector<CSCStripDigi>::const_iterator digiItr = (*stripDigi_id).second.first;
                vector<CSCStripDigi>::const_iterator last = (*stripDigi_id).second.second;
                for( ; digiItr != last; ++digiItr)
                {
                    stripBuf.push_back(digiItr->getStrip());
                    stripADCBuf.push_back(digiItr->getADCCounts());
                }
                strip.push_back(stripBuf);
                stripADCs.push_back(stripADCBuf);

            }*/

	    if (foundLCT){
		  // fetch the CSC comparator digis in this chamber
		  CSCComparatorDigiContainerIds compDigisIds;
		  
		  for (int iLayer=1; iLayer<=6; ++iLayer){
			CSCDetId layerId(chamberId.endcap(), chamberId.station(), chamberId.ring(), chamberId.chamber(), iLayer);
			// get the digis per layer
			auto compRange = compDigi->get(layerId);
			CSCComparatorDigiContainer compDigisLCT;
			for (auto compDigiItr = compRange.first; compDigiItr != compRange.second; compDigiItr++) {
			  auto compDigi = *compDigiItr;
			  //if (stub.getTimeBin() < 4 or stub.getTimeBin() > 8) continue;
			  int stubHalfStrip(getHalfStrip(compDigi));
			  // these comparator digis never fit the pattern anyway!
			  if (std::abs(stubHalfStrip - matchedLCT.getStrip())>5) continue;
			  // check if this comparator digi fits the pattern
			  //if(verbose) std::cout << "Comparator digi L1Mu " << layerId << " " << compDigi << " HS " << stubHalfStrip << " stubKeyHS " << stub.getStrip() << std::endl;
			  if (comparatorInLCTPattern(matchedLCT.getStrip(), matchedLCT.getPattern(), iLayer, stubHalfStrip)) {
				//if(verbose) std::cout<<"\tACCEPT"<<std::endl;
				compDigisLCT.push_back(compDigi);
			  }
			  // else{
			  //   if(verbose) std::cout<<"\tDECLINE!"<<std::endl;
			  // }
			}
			// if(verbose) if (compDigis.size() > 2) std::cout << ">>> INFO: " << compDigis.size() << " matched comp digis in this layer!" << std::endl;
			compDigisIds.push_back(std::make_pair(layerId, compDigisLCT));
		  }

		  //add a fitting module to fit comparator digis
		  // get the z and phi positions
		  auto cscChamber = theCSC->chamber(chamberId);
		  float perp = 0.0;
		  std::vector<float> phis;
		  std::vector<float> zs;
		  std::vector<float> ephis;
		  std::vector<float> ezs;
		  std::vector<float> status;
		  for (auto p: compDigisIds){
			auto detId = p.first;
			float phi_tmp = 0.0;
			float perp_tmp = 0.0;
			float z_tmp = 0.0;
			if (p.second.size()==0) continue;
			for (auto hit: p.second){
			  float fractional_strip = getFractionalStrip(hit);
			  auto layer_geo = cscChamber->layer(detId.layer())->geometry();
			  float wire = layer_geo->middleWireOfGroup(matchedLCT.getKeyWG() + 1);
			  LocalPoint csc_intersect = layer_geo->intersectionOfStripAndWire(fractional_strip, wire);
			  GlobalPoint csc_gp = theCSC->idToDet(detId)->surface().toGlobal(csc_intersect);
			  float gpphi = csc_gp.phi();

			  if (phis.size()>0 and gpphi>0 and phis[0]<0 and  (gpphi-phis[0])>3.1416)
				phi_tmp += (gpphi-2*3.1415926);
			  else if (phis.size()>0 and gpphi<0 and phis[0]>0 and (gpphi-phis[0])<-3.1416)
				phi_tmp += (gpphi+2*3.1415926);
			  else
				phi_tmp += (csc_gp.phi());

			  z_tmp = csc_gp.z();
			  perp_tmp += csc_gp.perp();
			}
			//in case there are more than one comparator digis in one layer
			//take the average position 
			perp_tmp = perp_tmp/(p.second).size();
			phi_tmp = phi_tmp/(p.second).size();
			//std::cout <<"detid "<< detId <<" perp "<< perp_tmp <<" phi "<< phi_tmp <<" z "<< z_tmp << std::endl;
			perp += perp_tmp;
			phis.push_back(phi_tmp);
			zs.push_back(z_tmp);
			ezs.push_back(0);
			// phis.push_back(csc_gp.phi());
			ephis.push_back(cscHalfStripWidth(detId)/sqrt(12));
		  }


		  CSCDetId key_id(chamberId.endcap(), chamberId.station(), chamberId.ring(), chamberId.chamber(), CSCConstants::KEY_CLCT_LAYER);
		  float fractional_strip = 0.5 * (matchedLCT.getStrip() + 1) - 0.25;
		  auto layer_geo = cscChamber->layer(CSCConstants::KEY_CLCT_LAYER)->geometry();
		  // LCT::getKeyWG() also starts from 0
		  float wire = layer_geo->middleWireOfGroup(matchedLCT.getKeyWG() + 1);
		  LocalPoint csc_intersect = layer_geo->intersectionOfStripAndWire(fractional_strip, wire);
		  GlobalPoint csc_gp = theCSC->idToDet(key_id)->surface().toGlobal(csc_intersect);
		  perp = csc_gp.perp();
		  //float stripPhiPitch = layer_geo->stripPhiPitch();
		  //if (stripBits > 0)
		 //  stripPhiPitch = stripPhiPitch/stripBits;

		  // use average perp
		  //perp = perp/phis.size();
		  // do a fit to the comparator digis
		  float alpha = -99., beta = 0.;
		  float fit_z_layers[6] = {0.,0.,0.,0.,0.,0.};
		  float fit_phi_layers[6] = {0.,0.,0.,0.,0.,0.};
		  PtassignmentHelper::calculateAlphaBeta(zs, phis, ezs, ephis, status, alpha, beta);
		  if (phis.size() <= 2 or fabs(alpha)>=99){
			  std::cout <<"warning, falied to fit comparator digis,num of digis: "<< phis.size()<<" alpha "<< alpha <<" beta "<< beta <<" just LCT position as fitting position"<< std::endl;
			  alpha = csc_gp.phi();
			  beta = 0.0;
		  }
			  //std::cout <<"fitting results: alpha "<< alpha <<" beta "<< beta << std::endl;
		  vector<float> phifitbuf;
		  for (int i=0; i<6; i++){
			  fit_z_layers[i] = cscChamber->layer(i+1)->centerOfStrip(20).z();
			  fit_phi_layers[i] = PtassignmentHelper::normalizePhi(alpha + beta * fit_z_layers[i]);
			  phifitbuf.push_back(fit_phi_layers[i]);
			  if (i == CSCConstants::KEY_CLCT_LAYER){
				  //lcteta.push_back(gpLCT.eta());
				  lctphi_fit.push_back(fit_phi_layers[i] );

			  }
			  //if (stripBits > 0)
				//fit_phi_layers[i] = (std::floor(fit_phi_layers[i]/stripPhiPitch) + 0.5)*stripPhiPitch;
			  //if (verbose_>0)
			//std::cout <<"i "<< i <<" fit_z "<< fit_z_layers[i]<< " fit_phi "<< fit_phi_layers[i]<<" perp "<< perp << std::endl;
		  }
		  compphi_fit.push_back(phifitbuf);

		}//foundLCT



        }// Matched CSCSegment loop

        cout << "Mouon pt "<<muon->pt()<<" eta "<< muon->eta()<<" phi "<< muon->phi() <<" Number of pats: " << clctPat.size() << " number of segments: " << range.size() <<  endl;
        if(range.size() > 0) tree->Fill();
    }// Muon loop
}


int MuonTrackAnalyzer::chamberSerial( CSCDetId id ) {
    int st = id.station();
    int ri = id.ring();
    int ch = id.chamber();
    int ec = id.endcap();
    //int lay = id.layer();
    int kSerial = ch;
    //cout<<"Endcap "<<ec<<" Station "<<st<<" Ring "<<ri<<" Chamber "<<ch<<" Layer "<<lay<<endl;
    if (st == 1 && ri == 1) kSerial = ch;
    if (st == 1 && ri == 2) kSerial = ch + 36;
    if (st == 1 && ri == 3) kSerial = ch + 72;
    if (st == 1 && ri == 4) kSerial = ch;
    if (st == 2 && ri == 1) kSerial = ch + 108;
    if (st == 2 && ri == 2) kSerial = ch + 126;
    if (st == 3 && ri == 1) kSerial = ch + 162;
    if (st == 3 && ri == 2) kSerial = ch + 180;
    if (st == 4 && ri == 1) kSerial = ch + 216;
    if (st == 4 && ri == 2) kSerial = ch + 234;  // one day...
    if (ec == 2) kSerial = kSerial + 300;
    return kSerial;
}

double MuonTrackAnalyzer::FindAnodeTime(vector<CSCRecHit2D>::const_iterator  hiti,  const edm::Handle<CSCWireDigiCollection> cscWireDigi, double local_t0) {
    // Create the ChamberId
    DetId id = (hiti)->geographicalId();
    CSCDetId chamberId(id.rawId());
    double Anode_time=0;

    int wire_digis=0;
    double time_guess=local_t0+205;   // 205 is anode cathode time offset
    //  double time_guess=local_t0;
    for (CSCWireDigiCollection::DigiRangeIterator j=cscWireDigi->begin(); j!=cscWireDigi->end(); j++)
    {
        CSCDetId wire_cscId = (*j).first;
        //  Check if in same chamber and layer.  Wire digis only use rings 1-3, RecHits 1-4 (for ME1/1A)
        if(chamberSerial(wire_cscId) == chamberSerial(chamberId) && wire_cscId.layer()==chamberId.layer()) {
            //	CSCRecHit2D::ChannelContainer wgroups = hiti->wgroups();
            int hit_wire = hiti->hitWire();

            vector<CSCWireDigi>::const_iterator digiItr = (*j).second.first;
            vector<CSCWireDigi>::const_iterator last = (*j).second.second;
            for( ; digiItr != last; ++digiItr) {
                if (digiItr->getWireGroup()==hit_wire) {
                    vector <int> TimeBins = digiItr->getTimeBinsOn();
                    wire_digis++;
                    double Digi_time=-99999.;
                    double diff_min=99999., best_match=-1;
                    for (uint j=0; j<TimeBins.size(); j++) {
                        double diff=(time_guess-TimeBins[j]*25);
                        if (abs(diff)<abs(diff_min)) {
                            diff_min=diff;
                            best_match=j;
                            Digi_time=TimeBins[j];
                        }
                    }
                    int side=diff_min/abs(diff_min);
                    bool unchanged=true;
                    if ((best_match+side)>-1 && (best_match+side)<TimeBins.size()) {      // Make sure one next to it exists
                        if (TimeBins[best_match]==(TimeBins[best_match+side]-side)) {
                            Digi_time=Digi_time+(double)side/2.;
                            unchanged=false;
                        }
                    }

                    if ((best_match-side)>-1 && (best_match-side)<TimeBins.size() && unchanged) {
                        if (TimeBins[best_match]==(TimeBins[best_match-side]+side)) {
                            Digi_time=Digi_time-(double)side/2.;
                            unchanged=false;
                        }
                    }
                    Anode_time+=Digi_time;
                }
            }
        }
    }
    if (wire_digis==0) Anode_time=-1;
    else Anode_time=(Anode_time/wire_digis)*25.;  //Convert from time bin to ns
    return Anode_time;
}



GlobalPoint
MuonTrackAnalyzer::getCSCCorrelatedLCTDigiPoint(unsigned int rawId, const CSCCorrelatedLCTDigi& lct) const
{
  // taken from https://github.com/cms-sw/cmssw/blob/dc9f78b6af4ad56c9342cf14041b6485a60b0691/L1Trigger/CSCTriggerPrimitives/src/CSCMotherboardME11GEM.cc
  CSCDetId cscId = CSCDetId(rawId);
  CSCDetId key_id(cscId.endcap(), cscId.station(), cscId.ring(), cscId.chamber(), CSCConstants::KEY_CLCT_LAYER);

  auto cscChamber = theCSC->chamber(cscId);
  // "strip" here is actually a half-strip in geometry's terms
  // note that LCT::getStrip() starts from 0
  float fractional_strip = 0.5 * (lct.getStrip() + 1) - 0.25;
  auto layer_geo = cscChamber->layer(CSCConstants::KEY_CLCT_LAYER)->geometry();
  // LCT::getKeyWG() also starts from 0
  float wire = layer_geo->middleWireOfGroup(lct.getKeyWG() + 1);
  LocalPoint csc_intersect = layer_geo->intersectionOfStripAndWire(fractional_strip, wire);
  GlobalPoint csc_gp = theCSC->idToDet(key_id)->surface().toGlobal(csc_intersect);
  //std::cout << "\t\t>>> other CSC LCT phi " << csc_gp.phi() << std::endl;
  //return getCSCSpecificPoint(rawId, lct).phi();
  return csc_gp;
}

int
MuonTrackAnalyzer::getHalfStrip(const CSCComparatorDigi& digi)
{
  return (digi.getStrip() - 1) * 2 + digi.getComparator();
}

float
MuonTrackAnalyzer::getFractionalStrip(const CSCComparatorDigi&d)
{
  return d.getStrip() + d.getComparator()/2. - 3/4.;
}

float
MuonTrackAnalyzer::cscHalfStripWidth(CSCDetId id)
{
	  // ME1a ME1b ME12 ME13 ME21 ME22 ME31 ME32 ME41 ME42
 int strips[10] = {48,64,80,64, 80,80, 80,80, 80,80};
 int degrees[10] = {10,10,10,10, 20,10, 20,10, 20,10};
 int index = id.iChamberType()-1;
 float width = float(degrees[index]) * 3.14159265358979323846/180. / float(2 * strips[index]);
 return width;	 
}

// ------------ method called once each job just before starting event loop  ------------
    void 
MuonTrackAnalyzer::beginJob()
{
	tree = fs->make<TTree>("CSCDigiTree","Muon Track Analyzer tree");

    //tree->Branch("Event_EventNumber",&Event_EventNumber,"Event_EventNumber/I");
    //tree->Branch("Event_RunNumber",&Event_RunNumber,"Event_RunNumber/I");
    //tree->Branch("Event_LumiSection",&Event_LumiSection,"Event_LumiSection/I");
    //tree->Branch("Event_BXCrossing",&Event_BXCrossing,"Event_BXCrossing/I");

    //tree->Branch("ss",&ss);
    //tree->Branch("os",&os);
    tree->Branch("Pt",&Pt,"Pt/D");
    tree->Branch("eta",&eta,"eta/D");
    tree->Branch("phi",&phi,"phi/D");
    tree->Branch("q",&q,"q/I");

    tree->Branch("Nseg",&Nseg,"Nseg/I");
    tree->Branch("segEc",&segEc);
    tree->Branch("segSt",&segSt);
    tree->Branch("segRi",&segRi);
    tree->Branch("segCh",&segCh);
    tree->Branch("segId",&segId);
    tree->Branch("segChamberType",&segChamberType);
    tree->Branch("segeta",&segeta);
    tree->Branch("segphi",&segphi);
    tree->Branch("segx",&segx);
    tree->Branch("segy",&segy);
    tree->Branch("segz",&segz);

    tree->Branch("rhId",&rhId);
    tree->Branch("rhLay",&rhLay);
    tree->Branch("rhPos",&rhPos);
    tree->Branch("rhE",&rhE);
    tree->Branch("rhMax",&rhMax);

    tree->Branch("lctId",&lctId);
    tree->Branch("lctQ",&lctQ);
    tree->Branch("lctBX",&lctBX);
    tree->Branch("lctPat",&lctPat);
    tree->Branch("lctKWG",&lctKWG);
    tree->Branch("lctKHS",&lctKHS);
    tree->Branch("lctBend",&lctBend);
    tree->Branch("lcteta",&lcteta);
    tree->Branch("lctphi",&lctphi);
    tree->Branch("dRSegLCT",&dRSegLCT);
    tree->Branch("lcteta_fit",&lcteta_fit);
    tree->Branch("lctphi_fit",&lctphi_fit);

	/*
    tree->Branch("tflctId",&tflctId);
    tree->Branch("tflctQ",&tflctQ);
    tree->Branch("tflctPat",&tflctPat);
    tree->Branch("tflctKWG",&tflctKWG);
    tree->Branch("tflctKHS",&tflctKHS);
    tree->Branch("tflctBend",&tflctBend);
    */

    tree->Branch("clctId",&clctId);
    tree->Branch("clctQ",&clctQ);
    tree->Branch("clctBX",&clctBX);
    tree->Branch("clctPat",&clctPat);
    tree->Branch("clctKHS",&clctKHS);
    tree->Branch("clctCFEB",&clctCFEB);
    tree->Branch("clctBend",&clctBend);

    tree->Branch("alctId",&alctId);
    tree->Branch("alctQ",&alctQ);
    tree->Branch("alctBX",&alctBX);
    tree->Branch("alctKWG",&alctKWG);
    tree->Branch("alctAc",&alctAc);
    tree->Branch("alctPB",&alctPB);

    /*tree->Branch("wireId",&wireId);
    tree->Branch("wireLay",&wireLay);
    tree->Branch("wireGrp",&wireGrp);
    tree->Branch("wireTimeOn",&wireTimeOn);

    tree->Branch("stripId",&stripId);
    tree->Branch("stripLay",&stripLay);
    tree->Branch("strip",&strip);
    tree->Branch("stripADCs",&stripADCs);

    tree->Branch("compId",&compId);
    tree->Branch("compLay",&compLay);
    tree->Branch("compStr",&compStr);
    tree->Branch("compHS",&compHS);
    tree->Branch("compphi_fit",&compphi_fit);
    tree->Branch("compTimeOn",&compTimeOn);
    */

}

// ------------ method called once each job just after ending the event loop  ------------
void 
MuonTrackAnalyzer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(MuonTrackAnalyzer);
