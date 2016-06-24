#ifndef CSCUCLA_GEMCSCTUPLEANALYSIS_GEMPartitionGEOMETRY_H
#define CSCUCLA_GEMCSCTUPLEANALYSIS_GEMPartitionGEOMETRY_H

#include "../include/Points.h"

namespace CSCGEMTuples {

class GEMPartitionGeometry {
public:

  GEMPartitionGeometry(float bottomEdge,float topEdge,float height,int theNumberOfStrips);

  //Strips go left to right 1.....nStips
  Point2D  centerOfStrip(int strip) const;
  //Fractional strip number from 0....nStrips
  Point2D  localPosition(float strip) const;

  //get fractional strip number from local 2D
  float strip(const Point2D& lp) const;
  float strip(int strip) const;
  /** angle between strip and symmetry axis */
  float stripAngle(float strip) const;
  float localStripLength(const Point2D& lp) const;

  float pitch() const {return thePitch;}
  float bottomEdgeSize() const {return bottomEdge*2;}
  float topEdgeSize() const {return topEdge*2;}
  float height() const {return theDetHeight;}

  float localPitch(const Point2D& lp) const;

  Error2D  localError(float strip, float stripErr2)const;
  Error2D  clusterError(float centerStrip, float cluster_size= 1.) const;
  Error2D  clusterError(int firstStrip, int nStrips)   const;
  Point2D  clusterPosition(int firstStrip, int nStrips) const;


private:
  //From file
  const float bottomEdge;   // half size
  const float topEdge;    // half size
  const int   theNumberOfStrips;

  //For use in trapezoidal code
  const float thePitch;
  const float theDistToBeam;
  const float theDetHeight;
  const float theOffset;
  const float theYAxOr;
};


};
#endif
