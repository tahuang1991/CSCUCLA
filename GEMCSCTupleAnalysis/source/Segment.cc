
#include "../include/Segment.h"
#include <cmath>

namespace CSCGEMTuples {
void Segment::setCoords(double x, double y, double dxdz, double dydz) {
  _coords[X] = x; _coords[Y]=y; _coords[DX] = dxdz; _coords[DY] = dydz;
}
void Segment::setCov(double xx, double xy, double xdxdz, double xdydz, double yy, double ydxdz, double ydydz, double dxdzdxdz, double dxdzdydz, double dydzdydz) {
_cov[XX]   = xx;
_cov[XY]   = xy;
_cov[XDX]  = xdxdz;
_cov[XDY]  = xdydz;
_cov[YY]   = yy;
_cov[YDX]  = ydxdz;
_cov[YDY]  = ydydz;
_cov[DXDX] = dxdzdxdz;
_cov[DXDY] = dxdzdydz;
_cov[DYDY] = dydzdydz;
}

ErrorPoint2D Segment::project(double projX, double projY, double projZ, double projPhi) const {
  //First we project it to the correct Z
  double x = _coords[X] + projZ*_coords[DX];
  double y = _coords[Y] + projZ*_coords[DY];
  double xx = _cov[XX] +projZ*projZ*_cov[DXDX] +2*projZ*_cov[XDX];
  double xy = _cov[XY] +projZ*projZ*_cov[DXDY] +projZ*(_cov[XDY] + _cov[YDX]);
  double yy = _cov[YY] +projZ*projZ*_cov[DYDY] +2*projZ*_cov[YDY];
  //now move it
  ErrorPoint2D point(x+projX,y+projY,xx,xy,yy);
  //and rotate it
  if(projPhi) point = point.rotate(projPhi);
  return point;
}
};
