
#include "../include/GEMGeometry.h"
#include "../include/GEMGeoPlotting.h"
#include <TLine.h>


void GEMGeoPlotting::plotGEM(TPad * pad, int lineColor, int lineWidth) {
     CSCGEMTuples::GEMGeometry geo;

     for(unsigned int iL = 0; iL < geo.getNRows(); ++iL){
       const auto& part = geo.getPartition(iL);
       const auto cen = geo.getPartitionCenter(iL);
       double topY      =    cen.y() +   part.height()/2;
       double bottomY   =    cen.y() -   part.height()/2;
       double topX1     = -1*part.topEdgeSize()/2;
       double topX2     =    part.topEdgeSize()/2;
       double bottomX1  = -1*part.bottomEdgeSize()/2;
       double bottomX2  =    part.bottomEdgeSize()/2;

       auto drawLine = [&] (float x1, float y1, float x2, float y2) {
         TLine * line = new TLine(x1,y1,x2,y2);
         line->SetLineColor(lineColor); line->SetLineWidth(lineWidth);
         pad->cd(); line->Draw();};

       drawLine(topX1,topY,topX2,topY);
       drawLine(bottomX1,bottomY,bottomX2,bottomY);
       drawLine(bottomX1,bottomY,topX1,topY);
       drawLine(bottomX2,bottomY,topX2,topY);
     }

   }

