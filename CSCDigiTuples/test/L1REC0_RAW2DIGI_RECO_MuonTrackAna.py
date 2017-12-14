# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: L1REC0 -s RAW2DIGI,RECO --era=Run2_2016 --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW --customise=L1Trigger/Configuration/customiseUtils.L1TTurnOffUnpackStage2GtGmtAndCalo --conditions=80X_dataRun2_Prompt_v11 -n 20 --data --no_output --filein=/store/data/Run2016G/ZeroBias/RAW/v1/000/278/986/00000/12104D7A-0065-E611-B62C-FA163E52E986.root
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RECO',eras.Run2_2016)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/data/Run2016G/ZeroBias/RAW/v1/000/278/986/00000/12104D7A-0065-E611-B62C-FA163E52E986.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('L1REC0 nevts:20'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition
process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    ),
    #eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('output_l1_reco_2016G_test.root'),
    #fileName = cms.untracked.string('%s'%sys.argv[3]),
    outputCommands = cms.untracked.vstring('drop *',
                                           'keep *_simCscTriggerPrimitiveDigis_*_*',
                                           'keep *_csctfDigis_*_*',
                                           'keep *_*emtf*_*_*',
                                           'keep *_simEmtfDigis_*_*',
					   'keep CSCDetIdCSCCorrelatedLCTDigiMuonDigiCollection_*_*_*',
                                           'keep *_muonCSCDigis_*_*',
                                           'keep *_offlineBeamSpot_*_*',
										   'keep recoMuons_muons_*_RECO',
										   'keep *',
										   'keep *CSC*_*_*_*'
                                           ),
    splitLevel = cms.untracked.int32(0)
)

process.MakeNtuple = cms.EDAnalyzer("MuonTrackAnalyzer",
        NtupleFileName       = cms.untracked.string("MuonTrackTree.root"),
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




# Additional output definition

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('evtree.root')
                                   )
# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_Prompt_v11', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.MakeNtuple*process.endOfProcess)
process.output_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
#process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step, process.output_step, process.endjob_step)
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step, process.endjob_step)

# customisation of the process.

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseReEmul
from L1Trigger.Configuration.customiseReEmul import L1TReEmulFromRAW 

#call to customisation function L1TReEmulFromRAW imported from L1Trigger.Configuration.customiseReEmul
process = L1TReEmulFromRAW(process)

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseUtils
from L1Trigger.Configuration.customiseUtils import L1TTurnOffUnpackStage2GtGmtAndCalo 

#call to customisation function L1TTurnOffUnpackStage2GtGmtAndCalo imported from L1Trigger.Configuration.customiseUtils
process = L1TTurnOffUnpackStage2GtGmtAndCalo(process)

# End of customisation functions

