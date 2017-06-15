import FWCore.ParameterSet.Config as cms
import subprocess

process = cms.Process("TEST")

process.load("Configuration/StandardSequences/GeometryDB_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load("Configuration/StandardSequences/RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")


from Configuration.AlCa.GlobalTag import GlobalTag


from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')
#options.inputFiles = 'csc_forsync.root'
options.inputFiles = '/store/data/Run2016G/ZeroBias/RAW/v1/000/278/986/00000/12104D7A-0065-E611-B62C-FA163E52E986.root'
options.outputFile = 'evttree.root'
options.parseArguments()

#import re
#m = re.match("(.*).root", options.inputFiles[0])
#options.outputFile = m.group(1) + options.outputFile
#options.inputFiles[0] = "file://" + options.inputFiles[0] 


process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v8'
# process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_v12'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.options = cms.untracked.PSet( SkipEvent =
cms.untracked.vstring('ProductNotFound') )

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
process.CSCGeometryESModule.useGangedStripsInME1a = False
process.idealForDigiCSCGeometry.useGangedStripsInME1a = False



process.MakeNtuple = cms.EDAnalyzer("MuonTrackAnalyzer",
        NtupleFileName       = cms.untracked.string(options.outputFile),
        wireDigiTag = cms.InputTag("muonCSCDigis", "MuonCSCWireDigi"),
        stripDigiTag = cms.InputTag("muonCSCDigis", "MuonCSCStripDigi"),
        alctDigiTag = cms.InputTag("muonCSCDigis", "MuonCSCALCTDigi"),
        clctDigiTag = cms.InputTag("muonCSCDigis", "MuonCSCCLCTDigi"),
        lctDigiTag = cms.InputTag("muonCSCDigis", "MuonCSCCorrelatedLCTDigi"),
        compDigiTag = cms.InputTag("muonCSCDigis", "MuonCSCComparatorDigi"),
        segmentTag = cms.InputTag("cscSegments"),
        
        recHitTag = cms.InputTag("csc2DRecHits"),
		minPt = cms.double(2),
		dR_seg_lct = cms.double(0.2),
        
        )



#process.p = cms.Path(process.muonCSCDigis * process.csc2DRecHits * process.cscSegments *  process.MakeNtuple)
process.p = cms.Path(process.muonCSCDigis * process.csc2DRecHits * process.cscSegments )

