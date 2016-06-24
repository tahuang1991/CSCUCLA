#ifndef CSCUCLA_GEMCSCTUPLEANALYSIS_SEGMENT_H
#define CSCUCLA_GEMCSCTUPLEANALYSIS_SEGMENT_H
#include "include/Points.h"
namespace CSCGEMTuples {
class Segment {
public:
  enum Coord {X,Y,DX,DY};
  enum Cov   {XX,XY,XDX,XDY,YY,YDX,YDY,DXDX,DXDY,DYDY};
  Segment() : _coords(new float[4]), _cov(new float[10]) {};
  ~Segment() {delete[] _coords; delete[] _cov;}
  void setCoords(float x, float y, float dxdz, float dydz);
  void setCov(float xx, float xy, float xdxdz, float xdydz, float yy, float ydxdz, float ydydz, float dxdzdxdz, float dxdzdydz, float dydzdydz);
  ErrorPoint2D project(float projX, float projY, float projZ, float projPhi) const;
private:
  float * _coords;
  float * _cov;
};


};
#endif
