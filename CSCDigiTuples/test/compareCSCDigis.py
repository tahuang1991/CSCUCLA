import ROOT
import random
import os
import sys
import numpy as np
import array
import math

ROOT.gROOT.SetBatch(0)
ROOT.gStyle.SetStatW(0.07)
ROOT.gStyle.SetStatH(0.06)

#ROOT.gStyle.SetOptStat(111110)

#ROOT.gStyle.SetErrorX(0)
#ROOT.gStyle.SetErrorY(0)

ROOT.gStyle.SetTitleStyle(0)
ROOT.gStyle.SetTitleAlign(13) ## coord in top left
ROOT.gStyle.SetTitleX(0.)
ROOT.gStyle.SetTitleY(1.)
ROOT.gStyle.SetTitleW(1)
ROOT.gStyle.SetTitleH(0.058)
ROOT.gStyle.SetTitleBorderSize(0)

ROOT.gStyle.SetPadLeftMargin(0.126)
ROOT.gStyle.SetPadRightMargin(0.10)
ROOT.gStyle.SetPadTopMargin(0.06)
ROOT.gStyle.SetPadBottomMargin(0.13)

ROOT.gStyle.SetMarkerStyle(1)


xmin=0
xmax=60
xbins=30
BINM=22
offsetLUT = {}
#binLow = [0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,12.0,14.0,16.0,18.0,20.0,24.0,28.0,32.0,36.0,42.0,50.0,60.0]
binLow = [0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,12.0,14.0,16.0,18.0,20.0,24.0,28.0,32.0,36.0,42.0,50.0]
ptbins = np.asarray(binLow)
	
def gethist1D(chain, den, todraw, x_bins):
	
    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    h1 = ROOT.TH1F("h1","h1",xBins,xminBin,xmaxBin)
    chain.Draw("fabs(%s) >> h1"%todraw,den)
    #print "gethist1D, den ",den
    #h1.Print("ALL")
    return h1
		



def draw2Dplots_1(ch, xaxis, yaxis, x_bins, y_bins, xtitle, ytitle,cuts, text, picname):

    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    yBins = int(y_bins[1:-1].split(',')[0])
    yminBin = float(y_bins[1:-1].split(',')[1])
    ymaxBin = float(y_bins[1:-1].split(',')[2])

    todrawb0 = "%s"%yaxis+":"+"%s>>b0"%xaxis
    
    c0 = ROOT.TCanvas("c0","c0")
    c0.SetGridx()
    c0.SetGridy()
    c0.SetTickx()
    c0.SetTicky()
    b0 = ROOT.TH2F("b0","b0",xBins,xminBin,xmaxBin,yBins,yminBin,ymaxBin)
    b0.GetXaxis().SetTitle("%s"%xtitle)
    b0.GetYaxis().SetTitle("%s"%ytitle)
    b0.GetXaxis().SetTitleSize(.04)
    b0.GetYaxis().SetTitleSize(.04)
    b0.SetTitle("#scale[1.4]{#font[61]{CMS}} #font[52]{ preliminary}"+"  "*15+"Run=281976")
    #b0.SetTitle("%s Vs %s,%s"%(ytitle0, xtitle, st_title)) 
    #b1.SetTitleFont(62)
    b0.SetTitleSize(0.05)
    #b1.SetMaximum(30)
    b0.SetStats(0)
    #hasxy = "&& fabs(%s)>0 && fabs(%s)>0"%(xaxis, yaxis0)
    ch.Draw(todrawb0,cuts,"colz")
    print "todraw ",todrawb0, " cut ",cuts

    #tex1 = TLatex(0.35,.8,"#splitline{%s}{%.1f<|#eta|<%.1f,20<p_{T}<50}"%(st_title,etamin,etamax))
    #tex1 = ROOT.TLatex(0.15,.8,"%s"%(text))
    tex1 = ROOT.TLatex(0.2,.8,"%s"%(text))
    tex1.SetTextSize(0.05)
    tex1.SetTextFont(62)
    tex1.SetNDC()
    tex1.Draw("same")
    c0.SaveAs("%s"%picname+".png")
    c0.SaveAs("%s"%picname+".pdf")
    c0.SaveAs("%s"%picname+".C")

def draw1D_compare(filelist, chs, xaxis_list, x_bins, xtitle, legs, cuts, text, picname):

    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    tfilelist = []
    for f in filelist:
	tfilelist.append(ROOT.TFile(f, "READ"))
    color = [ROOT.kBlue, ROOT.kRed, ROOT.kGreen+2,ROOT.kMagenta+2, ROOT.kCyan+2]
    maker = [20,21,22,23,33]
    #title = "#scale[1.4]{#font[61]{CMS}} #font[52]{ preliminary}"+"  "*15+"Run=281976"
    title = "#scale[1.4]{#font[61]{CMS}} #font[52]{ Simulation preliminary}"+"  "*15
    hs1 = ROOT.THStack("hs1","%s"%title)
    hs2 = ROOT.THStack("hs2","%s"%title)
    legend = ROOT.TLegend(0.15,0.55,0.5,0.7)
    legend.SetFillColor(ROOT.kWhite)
    #legend.SetTextFont(42)
    #legend.SetTextSize(.05)
    
    i=0
    meanlist = []
    for ch in chs:
	tfilelist[i].cd()
	print "todraw ","%s>>hist%d"%(xaxis_list[i], i)," cut ",cuts[i]
	hist = ROOT.TH1F("hist%d"%i,"hist%d"%i, xBins, xminBin, xmaxBin)
	ch.Draw("%s>>hist%d"%(xaxis_list[i], i),cuts[i])
	ROOT.SetOwnership(hist, False)
	hist.SetLineColor(color[i])
	hist.SetLineWidth(2)
	hist.Sumw2()
	hs1.Add(hist)
	hs2.Add(hist.Scale(1.0/hist.Integral()))
	mean = hist.GetMean()
	meanlist.append(mean)
	rms = hist.GetRMS()
	#legend.AddEntry(hist, "%s, Mean: %.4f, RMS: %.4f"%(legs[i], mean, rms),"l")
	legend.AddEntry(hist, "%s, Mean: %.4f"%(legs[i], mean),"l")
	i +=1

    c0 = ROOT.TCanvas("c0","c0")
    c0.SetGridx()
    c0.SetGridy()
    c0.SetTickx()
    c0.SetTicky()
    c0.SetLogy()
    hs1.SetMinimum(.001)
    hs1.Draw("nostacke") 
    hs1.GetHistogram().GetXaxis().SetTitle(xtitle+", "+xaxis_list[0])
    hs1.GetHistogram().GetXaxis().SetTitleSize(.05)
    hs1.GetHistogram().GetYaxis().SetTitle("Normalized to unity")
    hs1.GetHistogram().GetYaxis().SetTitleSize(.05)
    #hs1.GetHistogram().GetYaxis().SetRangeUser(.001,2)
    legend.Draw("same")
    tex1 = ROOT.TLatex(0.2,.8,"%s"%(text))
    tex1.SetTextSize(0.05)
    tex1.SetTextFont(62)
    tex1.SetNDC()
    tex1.Draw("same")
    c0.SaveAs("%s"%picname+".C")
    #c0.SaveAs("%s"%picname+".png")
    c0.SaveAs("%s"%picname+".pdf")
#    c0.SaveAs("%s"%picname+".C")

    return meanlist
    """
    c1 = ROOT.TCanvas("c1","c1")
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()
    hs2.Draw("nostack") 
    hs2.GetHistogram().GetXaxis().SetTitle("%s"%xtitle)
    hs2.GetHistogram().GetYaxis().SetTitle("Normalized to unity")
    legend.Draw("same")
    tex1.SetNDC()
    tex1.Draw("same")
    c1.SaveAs("%s"%picname+"_normalized.png")
    c1.SaveAs("%s"%picname+"_normalized.C")
    """

#######################################################
###1 data
#file1 = "TPEHists_run2016H_v1_281976.root"
#file1 = "simEmtfDigis_runH_v1_281976.root"
file1 = "/fdata/hepx/store/user/tahuang/CSCDigisNtuples/DataNtuple_20171017.root"
#file1 = "emtfStage2Digis_Run2016H_v1_281976.root"
## MC
#file2 = "simEmtfDigis_MC.root"
#file2 = "simEmtfDigis_Pt100_PU0_MC_bunchTimingOffsetZeore_20171017.root"
#file2 = "DataCSCdigi.root"
file2 = "/fdata/hepx/store/user/tahuang/CSCDigisNtuples/MCNtuple_20171017.root"
ch1 = ROOT.TChain("CSCDigistree")
#ch1 = ROOT.TChain("TFHits")
#ch2 = ROOT.TChain("simEmtfDigis")
ch2 = ROOT.TChain("CSCDigistree")
ch1.Add(file1)
ch2.Add(file2)
#outputdir = "Run2016H_281976_And_MCPU0_Zerooffset_20171018/"
outputdir = "Test/"
os.system("mkdir -p "+outputdir)

cscstations = [ [0,0], 
                [1,1], [1,2], [1,3],[1,4],
                [2,1], [2,2],
                [3,1], [3,2],
                [4,1], [4,2],]
chambernames = ["all",
		"ME1/b","ME1/2","ME1/3","ME1/a",
		"ME2/1","ME2/2",
		"ME3/1","ME3/2",
		"ME4/1","ME4/2",]
allvars = ["quality","bend","pattern","key_WG","key_hs"]
xbinsall = ["(6,10.5,16.5)","(2,-0.5,1.5)","(10,1.5,11.5)","(121,-0.5,120.5)","(257,-0.5,256.5)"]
bx_bins = "(16, 0.5,16.5)"

chs = [ch1, ch2]
filelist = [file1, file2]
bx_bins = "(9,-4.5,4.5)" 
xtitle =  "Timing"
allxaxis = ["alctBX", "clctBX"]
#xaxis_list = [xaxis, xaxis]
legs = ["Data","MC, Zerooffsets"]
ptcut = 30
cuts = ["muonpt>=%d"%ptcut, "muonpt>=%d"%ptcut]
#text = "#splitline{Re-emulation}{after unpacker}"
text = "trigger primitives associated to Reco Muon"
#draw1D_compare(chs, xaxis_list, bx_bins, xtitle, legs, cuts, text, outputdir+"stubscompare_bx_emualtor")


Endcaps = {'positive':1,'negative':-1, 'all':0}
for  xaxis in allxaxis:
 xaxis_list = [xaxis, xaxis]
 for key in Endcaps:
  cap = Endcaps[key]  
  ichamber = 0
  #if cap==1 or cap==2:
  #    continue
  for ichtype in range(0, len(cscstations)):
    x = cscstations[ichtype]
    if x==[0,0]:
	detid = "&& endcap==%d"%(cap)
	if key == 'all':
		detid = " "
	cuts_new = [cuts[0]+detid, cuts[1]+detid]
	#text_new = text+", all %s CSC chambers"%key
	text_new = "#splitline{%s}{p_{T}^{L1}>=%d, all %s CSC chambers}"%(text, ptcut, key)
	meanlist = draw1D_compare(filelist, chs, xaxis_list, bx_bins, xtitle, legs, cuts_new, text_new, outputdir+"stubscompare_%s_emualtor_MC_ptcut%d_all%sCSCs"%(xaxis, ptcut, key))
	
	ichamber +=1
	continue
    station = x[0]
    ring = x[1]
    detid = "&& endcap==%d && station==%d && ring==%d"%(cap, station, ring)
    if key == 'all':
	    detid = "&& station==%d && ring==%d"%(station, ring)

    cuts_new = [cuts[0]+detid, cuts[1]+detid]
    #text_new = text+", %s endcap, %s"%(key, chambernames[ichamber])
    #text_new = text+", %s"%(chambernames[ichamber])
    text_new = "#splitline{%s}{p_{T}^{Reco}>=%d, %s, %s}"%(text, ptcut, key, chambernames[ichamber])
    meanlist = draw1D_compare(filelist, chs, xaxis_list, bx_bins, xtitle, legs, cuts_new, text_new, outputdir+"stubscompare_%s_emualtor_MC_ptcut%d_endcap%s_st%d_ring%d"%(xaxis, ptcut, key, station, ring))
    offsetLUT[chambernames[ichamber]] = (meanlist[1]-meanlist[0])*25.0
    ichamber +=1
  print "Endcap ",key, " type ",xaxis, " offsetLUT ",offsetLUT    

