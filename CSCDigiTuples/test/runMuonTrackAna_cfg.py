import FWCore.ParameterSet.Config as cms
import subprocess

from Configuration.StandardSequences.Eras import eras

process = cms.Process('TEST',eras.Run2_2016)

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load("Configuration/StandardSequences/GeometryDB_cff")
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
#process.load("Configuration/StandardSequences/MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#process.load('Configuration/StandardSequences/RawToDigi_Data_cff')
#process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
#process.load("Configuration.StandardSequences.Reconstruction_cff")




from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')
options.inputFiles = 'out_reco_1.root'
#options.inputFiles = 'output_l1_reco_2016G_allcollections.root'
#options.inputFiles = 'output_l1_reco_2016G.root'
#options.inputFiles = '/store/user/tahuang/SingleMuon/RAW2DIGI_RECO_Muons_Run281976/170616_102321/0000/out_reco_1.root'
#options.inputFiles = '/store/data/Run2016G/ZeroBias/RAW/v1/000/278/986/00000/12104D7A-0065-E611-B62C-FA163E52E986.root'
options.outputFile = 'evttree.root'
options.parseArguments()

#import re
#m = re.match("(.*).root", options.inputFiles[0])
#options.outputFile = m.group(1) + options.outputFile
options.inputFiles[0] = "file://" + options.inputFiles[0] 


from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v8'
#process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_v12'
process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v11'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
#process.options = cms.untracked.PSet( 
#SkipEvent = cms.untracked.vstring('ProductNotFound') )

process.source = cms.Source ("PoolSource",
        fileNames = cms.untracked.vstring(options.inputFiles )        

)
process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        default = cms.untracked.PSet( limit = cms.untracked.int32(100) ),
        FwkJob = cms.untracked.PSet( limit = cms.untracked.int32(0) )
    ),
    categories = cms.untracked.vstring('FwkJob'),
    destinations = cms.untracked.vstring('cout')
)



"""Customise digi/reco geometry to use unganged ME1/a channels"""
#process.CSCGeometryESModule.useGangedStripsInME1a = False
#process.idealForDigiCSCGeometry.useGangedStripsInME1a = False



process.MakeNtuple = cms.EDAnalyzer("MuonTrackAnalyzer",
        NtupleFileName       = cms.untracked.string(options.outputFile),
	    muonCollection = cms.InputTag("muons"),	
        wireDigiTag = cms.InputTag("muonCSCDigis", "MuonCSCWireDigi"),
        stripDigiTag = cms.InputTag("muonCSCDigis", "MuonCSCStripDigi"),
        alctDigiTag = cms.InputTag("muonCSCDigis", "MuonCSCALCTDigi"),
        clctDigiTag = cms.InputTag("muonCSCDigis", "MuonCSCCLCTDigi"),
        lctDigiTag = cms.InputTag("muonCSCDigis", "MuonCSCCorrelatedLCTDigi"),
        compDigiTag = cms.InputTag("muonCSCDigis", "MuonCSCComparatorDigi"),
        csctfDigiTag = cms.InputTag("csctfDigis"),
        #cscSegmentTag = cms.InputTag("cscSegments"),
        recHitTag = cms.InputTag("csc2DRecHits"),
        offlineBeamSpotTag = cms.InputTag("offlineBeamSpot"),
		minPt = cms.double(2),
		dR_seg_lct = cms.double(0.2),
        MatchParameters = cms.PSet(
            DTsegments = cms.InputTag("dt4DSegments"),
            DTradius = cms.double(0.1),
            CSCsegments = cms.InputTag("cscSegments"),
            TightMatchDT = cms.bool(False),
            TightMatchCSC = cms.bool(True)
            ),
        ServiceParameters = cms.PSet(
            Propagators = cms.untracked.vstring('SteppingHelixPropagatorAny'),
            RPCLayers = cms.bool(True)
            )
        
        )


process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(options.outputFile)
                                   )

#process.p = cms.Path(process.muonCSCDigis * process.csc2DRecHits * process.cscSegments *  process.MakeNtuple)
#process.p = cms.Path(process.muonCSCDigis * process.csc2DRecHits * process.cscSegments )
process.p = cms.Path( process.MakeNtuple )

