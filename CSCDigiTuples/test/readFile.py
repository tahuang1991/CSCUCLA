# This must be ran from CMSSW_6_2_X
# Then just use cmsRun readFile.py after changing the .raw file below to the file you would like to unpack
#
import FWCore.ParameterSet.Config as cms

process = cms.Process("reader")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cout.placeholder = cms.untracked.bool(False)
process.MessageLogger.cout.threshold = cms.untracked.string('INFO')
process.MessageLogger.debugModules = cms.untracked.vstring('*')

process.source = cms.Source("DaqSource",
    readerPluginName = cms.untracked.string('CSCFileReader'),
    readerPset = cms.untracked.PSet(
        firstEvent  = cms.untracked.int32(0),
        tfDDUnumber = cms.untracked.int32(0),
        FED750 = cms.untracked.vstring('RUI00'),
        RUI00  = cms.untracked.vstring('csc_00000001_EmuRUI01_Monitor_000_160513_123324_UTC.raw'),
  )
)

process.FEVT = cms.OutputModule("PoolOutputModule",
        fileName = cms.untracked.string("test.root"),
        outputCommands = cms.untracked.vstring("keep *")
)

process.outpath = cms.EndPath(process.FEVT)
