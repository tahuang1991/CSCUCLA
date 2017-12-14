# Run quiet mode
import sys
sys.argv.append( '-b' )
import ROOT
ROOT.gROOT.SetBatch(1)

ROOT.gErrorIgnoreLevel=1001
from ROOT import *
#from Helpers import *
import random
import os
import numpy as n
import math




#______________________________________________________________________________
def addfiles(ch, dirname="./", ext=".root"):
  theInputFiles = []
  if os.path.isfile(dirname):
      ch.Add(dirname)
      return ch
  elif not os.path.isdir(dirname):
    print "ERROR: This is not a valid directory: ", dirname
    exit()
  ls = os.listdir(dirname)
  theInputFiles.extend([dirname[:] + x for x in ls if x.endswith(ext)])
  for pfile in theInputFiles:
    #print pfile
    ch.Add(pfile)

  return ch

#_____________________________________________________________________________
def deltaR(eta1, phi1, eta2, phi2):
    dPhi = phi1-phi2
    if dPhi > math.pi:
      dPhi = dPhi - 2*math.pi
    elif dPhi < -math.pi:
      dPhi = dPhi + 2*math.pi
    dEta = eta1-eta2
    #print "dPhi ",dPhi, " dEta ",dEta
    return math.sqrt(dPhi*dPhi + dEta*dEta)


#______________________________________________________________________________
if __name__ == "__main__":
    #label = "Neutrino_Pt2to20_gun_TTI2023Upg14D_PU140bx25_ILT_SLHC14_20170207_v2_1k_test"; pu = 'PU140'; eff = False
  label = "Run2_Data"; pu = 'Run2'; eff = False
  if len(sys.argv)>=2:
  	label = sys.argv[1]

  chamberLUT = ["","ME1/b","ME1/2","ME1/3","ME1/a","ME2/1","ME2/2","ME3/1","ME3/2","ME4/1","ME4/2"]
  ## extension for figures - add more?
  ext = ".png"

  ## Style
  #gStyle.SetStatStyle(0)

  print "Making the plots"

  ########set_style()

  doTest = False


  targetDir = label + "/"
  if not os.path.exists(targetDir):
      os.makedirs(targetDir)
  verbose = False

  ## copy index file
  #import shutil
  #shutil.copy2('index.php', targetDir + 'index.php')

  def CSCFitPlots(inputdir, outputroot):

      #ch = TChain("DisplacedL1MuFilter_PhaseIIGE21_%s/L1MuTree"%ME0Segment)
    ch = TChain("MakeNtuple/CSCDigiTree")

    location0 = 'evttree.root'
    location0 = 'Dataevtree.root'

    if doTest:
      treeHits = addfiles(ch, location0)
      #treeHits = addfiles(ch, "out_ana_rate_192_20170607_10k.root")
      #treeHits = addfiles(ch, location0+"out_ana_0.root")
      #treeHits = addfiles(ch, location0+"out_ana_1.root")
      #treeHits = addfiles(ch, location0+"out_ana_2.root")
    else:
      #treeHits = addfiles(ch, "out_ana_rate.root")
      print "not doing test"
      treeHits = addfiles(ch, inputdir)

    ## helper functions to make many plots!!
    #mapTH1F = ROOT.std.map("string,TH1F")()
    #mapTH2F = ROOT.std.map("string,TH2F")()

     
    ## binning

    CSCDigistree = TTree("CSCDigistree", "CSCDigistree")
    muonpt = n.zeros(1, dtype=float) 
    muoneta = n.zeros(1, dtype=float) 
    muonphi = n.zeros(1, dtype=float) 
    muonq = n.zeros(1, dtype=float) 

    Nseg = n.zeros(1, dtype=int)
    segeta = n.zeros(1, dtype=float)
    segphi = n.zeros(1, dtype=float)
    segEc = n.zeros(1, dtype=int)
    segSt = n.zeros(1, dtype=int)
    segRi = n.zeros(1, dtype=int)
    segCh = n.zeros(1, dtype=int)
    segChamberType = n.zeros(1, dtype=int)
    segId = n.zeros(1, dtype=int)

    lctPat = n.zeros(1, dtype=int)
    lctBX = n.zeros(1, dtype=int)
    lctKWG = n.zeros(1, dtype=int)
    lctKHS = n.zeros(1, dtype=int)
    lctBend = n.zeros(1, dtype=int)

    lctId = n.zeros(1, dtype=int)
    lcteta = n.zeros(1, dtype=float)
    lctphi = n.zeros(1, dtype=float)
    lcteta_fit = n.zeros(1, dtype=float)
    lctphi_fit = n.zeros(1, dtype=float)
    dRSegLCT = n.zeros(1, dtype=float)

    nofit_phiDiff = n.zeros(1, dtype=float)
    fit_phiDiff = n.zeros(1, dtype=float)

    alctBX = n.zeros(1, dtype=int)
    alctKWG = n.zeros(1, dtype=int)
    clctBX = n.zeros(1, dtype=int)
    clctBend = n.zeros(1, dtype=int)
    clctKHS = n.zeros(1, dtype=int)

    CSCDigistree.Branch("muonpt",muonpt,"muonpt/D")
    CSCDigistree.Branch("muoneta",muoneta,"muoneta/D")
    CSCDigistree.Branch("muonphi",muonphi,"muonphi/D")
    CSCDigistree.Branch("muonq",muonq,"muonq/D")

    CSCDigistree.Branch("Nseg",Nseg,"Nseg/I")
    CSCDigistree.Branch("segeta",segeta,"segeta/D")
    CSCDigistree.Branch("segphi",segphi,"segphi/D")
    CSCDigistree.Branch("segId",segId,"segId/I")
    CSCDigistree.Branch("endcap",segEc,"segEc/I")
    CSCDigistree.Branch("station",segSt,"segSt/I")
    CSCDigistree.Branch("ring",segRi,"segRi/I")
    CSCDigistree.Branch("chamber",segCh,"segCh/I")
    CSCDigistree.Branch("segChamberType",segChamberType,"segChamberType/I")

    CSCDigistree.Branch("lctPat", lctPat, "lctPat/I")
    CSCDigistree.Branch("lctBend", lctBend, "lctBend/I")
    CSCDigistree.Branch("lctBX", lctBX, "lctBX/I")
    CSCDigistree.Branch("lctKWG", lctKWG, "lctKWG/I")
    CSCDigistree.Branch("lctKHS", lctKHS, "lctKHS/I")
    CSCDigistree.Branch("lctId",lctId,"lctId/I")
    CSCDigistree.Branch("lcteta",lcteta,"lcteta/D")
    CSCDigistree.Branch("lctphi",lctphi,"lctphi/D")
    CSCDigistree.Branch("lcteta_fit",lcteta_fit,"lcteta_fit/D")
    CSCDigistree.Branch("lctphi_fit",lctphi_fit,"lctphi_fit/D")
    CSCDigistree.Branch("dRSegLCT", dRSegLCT,"dRSegLCT/D")

    CSCDigistree.Branch("lctPat", lctPat, "lctPat/I")
    CSCDigistree.Branch("lctBend", lctBend, "lctBend/I")
    CSCDigistree.Branch("lctId",lctId,"lctId/I")
    CSCDigistree.Branch("nofit_phiDiff", nofit_phiDiff, "nofit_phiDiff/D")
    CSCDigistree.Branch("fit_phiDiff", fit_phiDiff, "fit_phiDiff/D")

    CSCDigistree.Branch("clctBX", clctBX, "clctBX/I")
    CSCDigistree.Branch("clctBend", clctBend, "clctBend/I")
    CSCDigistree.Branch("clctKHS", clctKHS, "clctKHS/I")
    CSCDigistree.Branch("alctBX", alctBX, "alctBX/I")
    CSCDigistree.Branch("alctKWG", alctKWG, "alctKWG/I")


    #h_eventcount = TH1F("h_eventcount","",10,0,10)

    maxEntries = ch.GetEntries()
    #if doTest:
    #  maxEntries = 1000000

    nEvents = maxEntries
    print "nEvents", nEvents
    for k in range(0,nEvents):
      if k%10==0: print "Processing event", k
      print "nEvents", nEvents, "k = ",k

      #h_eventcount.Fill(1)
      ch.GetEntry(k)
      treeHits = ch

      def initbranches():
	    #init branches
	#muonpt[0] = -1
        #muonphi[0] = 99
        #muoneta[0] = 99
        #muonq[0] = 99
        #Nseg[0] = 0
        segEc[0] = 99
        segSt[0] = 99
        segRi[0] = 99
        segCh[0] = 99
        segId[0] = 9999
	segChamberType[0] = 99
        segeta[0] = 99
        segphi[0] = 99
        lcteta[0] = 99
        lctphi[0] = 99
        lctphi_fit[0] = 99
        lcteta_fit[0] = 99
        lctId[0] = 99
        lctBX[0] = 99
        lctKHS[0] = 99
        lctKWG[0] = 99
        lctPat[0] = 99
        lctBend[0] = 99
        nofit_phiDiff[0] = 99
        fit_phiDiff[0] = 99
        alctBX[0] = 99
        alctKWG[0] = 99
        clctBX[0] = 99
        clctBend[0] = 99
        clctKHS[0] = 99

      def phi_difference (phi1, phi2):
        dPhi = phi1-phi2
        if dPhi > math.pi:
          dPhi = dPhi - 2*math.pi
        elif dPhi < -math.pi:
          dPhi = dPhi + 2*math.pi
        return dPhi;






      muonpt[0] = treeHits.Pt
      muoneta[0] = treeHits.eta
      muonphi[0] = treeHits.phi
      muonq[0] = treeHits.q
      Nseg[0] = treeHits.Nseg
      for i in range(0,Nseg):
        initbranches()
        segEc[0] = treeHits.segEc[i]
        segSt[0] = treeHits.segSt[i]
        segRi[0] = treeHits.segRi[i]
        segCh[0] = treeHits.segCh[i]
	segChamberType[0] = treeHits.segChamberType[i]
        segId[0] = treeHits.segId[i]
        segeta[0] = treeHits.segeta[i]
        segphi[0] = treeHits.segphi[i]
        dRnew = 99
        mindR = 99
        lctindex = 99
        #to Find matched LCT
        for j in range(0, len(list(treeHits.lcteta))):
            lcteta[0] = treeHits.lcteta[j]
            lctphi[0] = treeHits.lctphi[j]
            dRnew = deltaR(segeta[0], segphi[0], lcteta[0], lctphi[0])
        
            #print "j ",j," lctId ",treeHits.lctId[j]," Segid ", segId[0]," dRnew ",dRnew
            if treeHits.lctId[j] == segId[0] and dRnew <0.1 and dRnew < mindR:
              lctindex = j
              mindR = dRnew
            #lcteta_fit[0] = treeHits.lcteta_fit[j]
        if lctindex != 99:
            lcteta[0] = treeHits.lcteta[lctindex]
            lctphi[0] = treeHits.lctphi[lctindex]
            lctphi_fit[0] = treeHits.lctphi_fit[lctindex]
            lctId[0] = treeHits.lctId[lctindex]
            dRSegLCT[0] = treeHits.dRSegLCT[lctindex]
            lctPat[0] = treeHits.lctPat[lctindex]
            lctBend[0] = treeHits.lctBend[lctindex]
            lctBX[0] = treeHits.lctBX[lctindex]
            lctKHS[0] = treeHits.lctKHS[lctindex]
            lctKWG[0] = treeHits.lctKWG[lctindex]
        
            #if 5 < muonpt[0] < 10:
            nofit_phiDiff[0] = phi_difference(segphi[0], lctphi[0])
            fit_phiDiff[0] = phi_difference(segphi[0], lctphi_fit[0])
        #dRnew = deltaR(segeta, segphi, lcteta, lctphi)
	clctindex = 99
        for i in range(0, len(list(treeHits.clctId))):
	    if (treeHits.clctId[i] == segId[0]):
		clctindex = i
			
	if clctindex != 99:
	    print "clct index ", clctindex,  " lct index ",lctindex
            clctBend[0] = treeHits.clctBend[clctindex]
            clctBX[0] = treeHits.clctBX[clctindex]
            clctKHS[0] = treeHits.clctKHS[clctindex]
        
	if (clctindex < 99 and lctindex < 99 and treeHits.clctKHS[clctindex] != treeHits.lctKHS[lctindex]):
		print "id ",treeHits.clctId[clctindex], " CLCL HS ",treeHits.clctKHS[clctindex]," LCT HS ",treeHits.lctKHS[lctindex]
	alctindex = 99
        for i in range(0, len(list(treeHits.alctId))):
	    if (treeHits.alctId[i] == segId[0]):
		alctindex = i
	if alctindex != 99:
            alctBX[0] = treeHits.alctBX[alctindex]
            alctKWG[0] = treeHits.alctKWG[alctindex]
        if lctindex != 99: 
		#print "muon pt ",muonpt," Nseg ",Nseg[0], " segeta ",segeta[0]," segphi ",segphi[0], " lcteta ",lcteta[0], " lctphi ",lctphi[0]," lctphi_fit ",lctphi_fit[0] ," dRSegLCT ",dRSegLCT[0]," mindR ",mindR," lctBend ",lctBend[0]," lctPat ",lctPat
            CSCDigistree.Fill()




    #makeDPhiPlot(h_dphi_ME11_ME21, "dphi_ME11_ME21")
    #c = TCanvas("c","c",800,600)
    #c.Clear()
    #h_eventcount.Draw()
    #c.SaveAs("h_eventcount.png")
    #make plots before write into root 
    targetroot = TFile(outputroot,"RECREATE")
    #h_eventcount.Write()
    #h_dphi_ME11_ME21.Write()
    CSCDigistree.Write()
    targetroot.Close()
 

    ## trigger rate plots
  #inputdir_data = "/fdata/hepx/store/user/tahuang/SingleMuon/MuonTrackAna/"
  #output_data = "/fdata/hepx/store/user/tahuang/CSCDigisNtuples/DataNtuple_20171017.root"
  #inputdir_MC = "/fdata/hepx/store/user/tahuang/SingleMu_80X_200k_Pt100_Endcaponly_run2MC_RECO_bunchTimingOffsetsZero/MuonTrackAna/"
  #output_MC = "/fdata/hepx/store/user/tahuang/CSCDigisNtuples/MCNtuple_20171017.root"
  #inputdir_MC = "/fdata/hepx/store/user/tahuang/SingleMu_80X_200k_Pt100_Endcaponly_run2MC_RECO_updatebunchTimingOffsetsv1/MuonTrackAna/evtree_all_v2.root"
  #output_MC = "/fdata/hepx/store/user/tahuang/CSCDigisNtuples/MCNtuple_updatebunchtimingv1_20171017.root"
  inputdir_MC = "/fdata/hepx/store/user/tahuang/SingleMu_80X_200k_Pt100_Endcaponly_run2MC_RECO_updatebunchTimingOffsetsv1/MuonTrackAna/evtree_JPsi_noffsetchange.root"
  output_MC = "/fdata/hepx/store/user/tahuang/CSCDigisNtuples/MCNtuple_beforeupdatebunchtiming_20171017.root"
  #CSCFitPlots(inputdir_data, output_data)
  CSCFitPlots(inputdir_MC, output_MC)
  #displacedL1MuHybridTriggerRate("FakePadsPU250")

