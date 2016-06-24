
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "../include/BaseCSCAndGEMAnalyzer.h"
#include "include/GEMPlottingInfo.h"

#include<iostream>

using namespace std;
using namespace CSCGEMTuples;

class Analyze : public AnalyzeBoth {
    public:
        Analyze(std::string cscFileName, std::string gemFileName,const GEMConfigInfo* info) : AnalyzeBoth(cscFileName,gemFileName,info)
    {
        Ntot = 0;
        sameVFAT = 0;
        sameY = 0;
        Rand = 0;
        plotter.book1D("clus_dist","Cluster Distances;Distance (strips);Entries",10,0.5,10.5);
        plotter.book2D("clus_sizes2","Cluster Sizes;Cluster 1 Size;Cluster 2 Size",10,0.5,10.5,10,0.5,10.5);
        plotter.book2D("clus_sizes4up","Cluster Sizes;Cluster 1 Size;Cluster 2 Size",10,0.5,10.5,10,0.5,10.5);
    }
        virtual  ~Analyze() {};

        void write(TString outFileName)
        { 
            plotter.write(outFileName);
            cout << "Ntot: " << Ntot << " sameVFAT: " << sameVFAT << " sameY: " << sameY << " Rand: " << Rand << endl;
            cout << "sameVFAT%: " << 100.0*float(sameVFAT)/float(Ntot) << " sameY%: " << 100.0*float(sameY)/float(Ntot) << " Rand%: " << 100.0*float(Rand)/float(Ntot) << endl;
        }

        virtual void runAEvent() 
        {

            //Make CSC quality cuts
            if(!pureSample(csc)) return;

            //Fill basic data structs to be analyzed
            int nC = gem.gemInfo.clusters.size();
            if(nC != 2) return;
            Ntot++;

            int C1FS = gem.gemInfo.clusters.at(0).getFirstStrip();
            int C1LS = gem.gemInfo.clusters.at(0).getLastStrip();
            int C1R  = gem.gemInfo.clusters.at(0).nRow;
            int C1S  = gem.gemInfo.clusters.at(0).getNStrips();
            int C2FS = gem.gemInfo.clusters.at(1).getFirstStrip();
            int C2LS = gem.gemInfo.clusters.at(1).getLastStrip();
            int C2R  = gem.gemInfo.clusters.at(1).nRow;
            int C2S  = gem.gemInfo.clusters.at(1).getNStrips();
            if(C1R == C2R && (C1FS/128 == C2FS/128 || C1FS/128 == C2LS/128 || C1LS/128 == C2FS/128 || C1LS/128 == C2LS/128)) 
            {
                sameVFAT++;
                float dis1 = C1FS - C2LS;
                float dis2 = C2FS - C1LS;
                if(fabs(dis1) < fabs(dis2)) plotter.get1D("clus_dist")->Fill(fabs(dis1));
                else plotter.get1D("clus_dist")->Fill(fabs(dis2));
                if(fabs(dis1) == 2 || fabs(dis2) == 2) plotter.get2D("clus_sizes2")->Fill(C1S,C2S);
                else if(fabs(dis1) > 4 && fabs(dis2) > 4) plotter.get2D("clus_sizes4up")->Fill(C1S,C2S);
                return;
            }
            if((C1R+1 == C2R || C1R == C2R+1) 
                && 
                ((C2FS <= C1FS && C2FS >= C1LS) || (C2LS <= C1FS && C2LS >= C1LS)) 
              ) {sameY++; return;}

            Rand++;
        }

        int Ntot;
        int sameVFAT;
        int sameY;
        int Rand;

        PlotClusterInfo plotClusterInfo;
        PlotVFATInfo    plotVFATInfo;
        PlotEventInfo   plotEventInfo;
        HistGetter plotter;
};

#endif

void PlotGEMClusters(std::string cscfileName="csc_forsync.root",std::string gemfilename = "gem_forsync.root",std::string outFileName = "plotGEMClusters_out.root"){
    GEMConfigInfo info;
    info.geoName               = "gemGeo.txt";
    info.vFATChanMapName       = "slot_table_904_june09.csv";
    //  info.vFATChanMapName       = "slot_table.csv";
    Analyze a(cscfileName,gemfilename,&info);
    a.analyze(10000);
    a.write(outFileName);
    //  a.draw();
}
