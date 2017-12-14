import FWCore.ParameterSet.Config as cms
import os
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

isData = False
## helper for files on dCache/EOS (LPC)
def useInputDir(process, inputDir, onEOS = True):
    theInputFiles = []
    for d in range(len(inputDir)):
        my_dir = inputDir[d]
        if not os.path.isdir(my_dir):
            print "ERROR: This is not a valid directory: ", my_dir
            if d==len(inputDir)-1:
                print "ERROR: No input files were selected"
                exit()
            continue
        print "Proceed to next directory"
        ls = os.listdir(my_dir)
        if onEOS:
            theInputFiles.extend(['file:' + my_dir[:] + x for x in ls if x.endswith('.root')])
        else:
	    print "not on EOS, this part is not working "
            ## this works only if you pass the location on pnfs - FIXME for files staring with store/user/...                                                            
            theInputFiles.extend([my_dir[16:] + x for x in ls if x.endswith('.root')])

    process.source.fileNames = cms.untracked.vstring(*theInputFiles)
    return process


from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')
options.inputFiles = 'out_reco_1.root'
#options.inputFiles = 'output_l1_reco_2016G_allcollections.root'
#options.inputFiles = 'output_l1_reco_2016G.root'
#options.inputFiles = '/store/user/tahuang/SingleMuon/RAW2DIGI_RECO_Muons_Run281976/170616_102321/0000/out_reco_1.root'
#options.inputFiles = '/store/data/Run2016G/ZeroBias/RAW/v1/000/278/986/00000/12104D7A-0065-E611-B62C-FA163E52E986.root'
#options.inputFiles = ['/fdata/hepx/store/user/tahuang/SingleMuon/RAW2DIGI_RECO_Muons_Run281976/170616_102321/0000/out_reco_1.root']
#options.inputFiles = ['/fdata/hepx/store/user/tahuang/JPsiToMuMu_Pt20to100-pythia8-gun/RAW2DIGI_RECO_Muons_JPsiToMuMu_v2/170712_091000/0000/out_reco_1.root']
#options.outputFile = '/fdata/hepx/store/user/taohuang/CSCDigiFitting/evttree.root'
options.parseArguments()

#import re
#m = re.match("(.*).root", options.inputFiles[0])
#options.outputFile = m.group(1) + options.outputFile
options.inputFiles[0] = "file://" + options.inputFiles[0] 

print "inputFiles ", options.inputFiles[0]

from Configuration.AlCa.GlobalTag import GlobalTag
if isData:
    process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v8'
else:
    process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_v12'
#process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v11'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#process.options = cms.untracked.PSet( 
#SkipEvent = cms.untracked.vstring('ProductNotFound') )

process.source = cms.Source ("PoolSource",
        fileNames = cms.untracked.vstring(options.inputFiles )        

)

Inputdir = ['/fdata/hepx/store/user/tahuang/SingleMuon/RAW2DIGI_RECO_Muons_Run281976/170616_102321/0000/']
Inputdir = ['/fdata/hepx/store/user/tahuang/JPsiToMuMu_Pt20to100-pythia8-gun/RAW2DIGI_RECO_Muons_JPsiToMuMu_v2/170712_091000/0000/']
#Inputdir = ['/fdata/hepx/store/user/tahuang/SingleMu_80X_200k_Pt100_Endcaponly_run2MC_GEN_SIM_DIGI_L1_ACLTTiming_updatebunchTimingOffsetsTo0/SingleMu_80X_200k_Pt100_Endcaponly_run2MC_GEN_SIM_DIGI_L1_ACLTTiming_updatebunchTimingOffsetsTo0/171017_141416/0000/']
#Inputdir = ['/fdata/hepx/store/user/tahuang/SingleMu_80X_200k_Pt100_Endcaponly_run2MC_RECO_updatebunchTimingOffsetsv1/']
process =  useInputDir(process, Inputdir)


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
process.load("L1Trigger.CSCTriggerPrimitives.cscTriggerPrimitiveDigis_cfi")
#process.cscTriggerPrimptiveDiigs
process.cscTriggerPrimitiveDigis.CSCComparatorDigiProducer = "muonCSCDigis:MuonCSCComparatorDigi"
process.cscTriggerPrimitiveDigis.CSCWireDigiProducer = "muonCSCDigis:MuonCSCWireDigi"



process.MakeNtuple = cms.EDAnalyzer("MuonTrackAnalyzer",
        NtupleFileName       = cms.untracked.string(options.outputFile),
	muonCollection = cms.InputTag("muons"),	
        wireDigiTag = cms.InputTag("muonCSCDigis", "MuonCSCWireDigi"),
        stripDigiTag = cms.InputTag("muonCSCDigis", "MuonCSCStripDigi"),
        #alctDigiTag = cms.InputTag("muonCSCDigis", "MuonCSCALCTDigi"),
        #clctDigiTag = cms.InputTag("muonCSCDigis", "MuonCSCCLCTDigi"),
        #lctDigiTag = cms.InputTag("muonCSCDigis", "MuonCSCCorrelatedLCTDigi"),
        alctDigiTag = cms.InputTag("cscTriggerPrimitiveDigis"),
        clctDigiTag = cms.InputTag("cscTriggerPrimitiveDigis"),
        lctDigiTag  = cms.InputTag("cscTriggerPrimitiveDigis"),
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

process.out = cms.OutputModule("PoolOutputModule",
	fileName = cms.untracked.string('file:out_L1.root'),
	)
#process.p = cms.Path(process.muonCSCDigis * process.csc2DRecHits * process.cscSegments *  process.MakeNtuple)
#process.p = cms.Path(process.muonCSCDigis * process.csc2DRecHits * process.cscSegments )
process.p = cms.Path( process.cscTriggerPrimitiveDigis + process.MakeNtuple )
#process.p = cms.Path(process.MakeNtuple)
#process.p = cms.Path( process.cscTriggerPrimitiveDigis)
#process.endp = cms.EndPath(process.out)

print "============ inputs ================"
print process.source.fileNames
print "============ output ================"
print process.TFileService.fileName
