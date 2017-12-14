#!/usr/bin/python
import os
import sys


jobdir = "TestJobsMC"
os.system("mkdir -p %s" % jobdir)
submitscript = open("submitallAnaMC.sh","w")
submitscript.write("""#!/bin/bash
cd $CMSSW_BASE/src/CSCUCLA/CSCDigiTuples/test/
	""")

## helper for files on dCache/EOS (LPC)
def useInputDir(inputDir, onEOS = True):
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
            #theInputFiles.extend(['file:' + my_dir[:] + x for x in ls if x.endswith('.root')])
            theInputFiles.extend([my_dir[:] + x for x in ls if x.endswith('.root')])
        else:
	    print "not on EOS, this part is not working "
            ## this works only if you pass the location on pnfs - FIXME for files staring with store/user/...                                                            
            theInputFiles.extend([my_dir[16:] + x for x in ls if x.endswith('.root')])

    #process.source.fileNames = cms.untracked.vstring(*theInputFiles)
    print "InputFiles ",theInputFiles
    return theInputFiles


#Inputdir = ['/fdata/hepx/store/user/tahuang/SingleMuon/RAW2DIGI_RECO_Muons_Run281976/170616_102321/0000/']
#Inputdir = ['/fdata/hepx/store/user/tahuang/SingleMu_80X_200k_Pt100_Endcaponly_run2MC_RECO_bunchTimingOffsetsZero/']
Inputdir = ['/fdata/hepx/store/user/tahuang/SingleMu_80X_200k_Pt100_Endcaponly_run2MC_RECO_updatebunchTimingOffsetsv1/']
inputfiles =  useInputDir(Inputdir)
#outdir = "/fdata/hepx/store/user/tahuang/SingleMuon/MuonTrackAna/"
#outdir = "/fdata/hepx/store/user/tahuang/SingleMu_80X_200k_Pt100_Endcaponly_run2MC_RECO_bunchTimingOffsetsZero/MuonTrackAna_v2/"
outdir = "/fdata/hepx/store/user/tahuang/SingleMu_80X_200k_Pt100_Endcaponly_run2MC_RECO_updatebunchTimingOffsetsv1/MuonTrackAna/"
os.system("mkdir -p "+outdir)
for ijob in range(0, len(inputfiles)):

        infile = inputfiles[ijob]
	outputfile = outdir+"evtree_%d.root"%ijob
	jobscript = open("{0}/Send_RunAna_{1}.slrm".format(jobdir, ijob), "w")
	jobscript.write("""#!/bin/bash
#SBATCH -J RunMCana
#SBATCH -p background-4g
#SBATCH -n 1
#SBATCH --mem-per-cpu=2000
#SBATCH --time=72:00:00
#SBATCH -o batchjobs_Ana-%A-%a.out
#SBATCH -e batchjobs_Ana-%A-%a.err
#SBATCH --ntasks-per-core=1

echo "starting at `date` on `hostname`"
echo "SLURM_JOBID=$SLURM_JOBID"
jobid=$SLURM_JOBID
source ~/.bashrc
. /etc/profile.d/modules.sh
cd $CMSSW_BASE/src/CSCUCLA/CSCDigiTuples/test/
eval `scramv1 runtime -sh`
cmsRun runMuonTrackAna_MC_cfg.py inputFiles={infile} outputFile={outputfile} &> /dev/null
echo "job$jobid starts, `date`"
echo "job$jobid is done, `date`"
exit 0""".format(infile=infile, outputfile = outputfile))
	jobscript.close()

	submitscript.write("""
sbatch {0}/Send_RunAna_{1}.slrm""".format(jobdir, ijob))
submitscript.close()
os.system("chmod +x submitallAnaMC.sh")

#python PlotterProducer.py -b HaddNo {jobtype}
#os.system("./submitallAna.sh")

