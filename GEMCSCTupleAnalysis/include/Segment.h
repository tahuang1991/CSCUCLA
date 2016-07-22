#ifndef CSCUCLA_GEMCSCTUPLEANALYSIS_SEGMENT_H
#define CSCUCLA_GEMCSCTUPLEANALYSIS_SEGMENT_H
#include "include/Points.h"
namespace CSCGEMTuples {
class Segment {
public:
  enum Coord {X,Y,DX,DY};
  enum Cov   {XX,XY,XDX,XDY,YY,YDX,YDY,DXDX,DXDY,DYDY};
  Segment() : _coords(new double[4]), _cov(new double[10]) {};
  ~Segment() {delete[] _coords; delete[] _cov;}
  void setCoords(double x, double y, double dxdz, double dydz);
  void setCov(double xx, double xy, double xdxdz, double xdydz, double yy, double ydxdz, double ydydz, double dxdzdxdz, double dxdzdydz, double dydzdydz);
  ErrorPoint2D project(double projX, double projY, double projZ, double projPhi) const;
private:
  double * _coords;
  double * _cov;
};


};
#endif
