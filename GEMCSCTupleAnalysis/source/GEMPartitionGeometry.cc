
#include "../include/GEMPartitionGeometry.h"
#include <cmath>
namespace CSCGEMTuples {

GEMPartitionGeometry::GEMPartitionGeometry(float bottomEdge,float topEdge,float height,int theNumberOfStrips) :
    bottomEdge(bottomEdge),topEdge(topEdge),theNumberOfStrips(theNumberOfStrips),
    thePitch((bottomEdge + topEdge)/theNumberOfStrips ),
    theDistToBeam(height*(bottomEdge + topEdge)/(topEdge - bottomEdge)),
    theDetHeight(height*2),
    theOffset(-theNumberOfStrips/2 *thePitch ),
    theYAxOr(1)
    {}
//Strips go left to right 1.....nStips
Point2D  GEMPartitionGeometry::centerOfStrip(int strip) const {
  float s = static_cast<float>(strip) - 0.5;
  return localPosition(s);
}
Point2D  GEMPartitionGeometry::localPosition(float strip) const{
  return Point2D( strip*thePitch + theOffset, 0.0);
}
float GEMPartitionGeometry::strip(const Point2D& lp) const{
  float aStrip =
     ((lp.x()*theDistToBeam/(theYAxOr*lp.y()+theDistToBeam))-theOffset)/thePitch;
   if (aStrip < 0 ) aStrip = 0;
  else if (aStrip > theNumberOfStrips)  aStrip = theNumberOfStrips;
  return aStrip;
}
float GEMPartitionGeometry::strip(int strip) const {
  return static_cast<float>(strip) - 0.5;
}

/** angle between strip and symmetry axis */
float GEMPartitionGeometry::stripAngle(float strip) const {
  return std::atan( -(strip*thePitch + theOffset)*theYAxOr/theDistToBeam );
}
float GEMPartitionGeometry::localStripLength(const Point2D& lp) const {
  float ltan = -lp.x()/(theYAxOr*lp.y()+theDistToBeam)*theYAxOr;
  float localL = theDetHeight * std::sqrt(1.f+ltan*ltan);
  return localL;
}



float    GEMPartitionGeometry::localPitch(const Point2D& lp) const {
  float x=lp.x();
  float y=theYAxOr*lp.y()+theDistToBeam;
  return thePitch*y/(theDistToBeam*std::sqrt(1.f+x*x/(y*y))); //I think this should be 1/sqrt(...), but I took it from CMSSW...just be carefull
}

Error2D GEMPartitionGeometry::localError(float strip, float stripErr2)const {
  float lt,lc2,ls2,lslc;
  float localL2,localP2;
  float sl2,sp2;
  // angle from strip to local frame (see CMS TN / 95-170)
  lt = -(strip*thePitch + theOffset)*theYAxOr/theDistToBeam;
  lc2 = 1.f/(1.+lt*lt);
  lslc = lt*lc2;
  ls2 = 1.f-lc2;
  localL2 = theDetHeight*theDetHeight / lc2;
  localP2 = thePitch*thePitch*lc2;
  sl2 = localL2/12.;
  sp2 = stripErr2*localP2;
  return Error2D(lc2*sp2+ls2*sl2,
                    lslc*(sp2-sl2),
                    ls2*sp2+lc2*sl2);
}
Error2D  GEMPartitionGeometry::clusterError(float centerStrip, float cluster_size) const {
  return localError(centerStrip, cluster_size*cluster_size/12.);
}
Error2D GEMPartitionGeometry::clusterError(int firstStrip, int nStrips) const {
  float s = static_cast<float>(2*firstStrip+ nStrips -1 )/2 -0.5;
  return clusterError(s,float(nStrips) );
}
Point2D GEMPartitionGeometry::clusterPosition(int firstStrip, int nStrips) const {
  float s = static_cast<float>(2*firstStrip+ nStrips -1 )/2 -0.5;
  return localPosition(s);
}
}
