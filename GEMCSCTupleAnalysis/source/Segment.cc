
#include "../include/Segment.h"
#include <cmath>

namespace CSCGEMTuples {
void Segment::setCoords(float x, float y, float dxdz, float dydz) {
  _coords[X] = x; _coords[Y]=y; _coords[DX] = dxdz; _coords[DY] = dydz;
}
void Segment::setCov(float xx, float xy, float xdxdz, float xdydz, float yy, float ydxdz, float ydydz, float dxdzdxdz, float dxdzdydz, float dydzdydz) {
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

ErrorPoint2D Segment::project(float projX, float projY, float projZ, float projPhi) const {
  //First we project it to the correct Z
  float x = _coords[X] + projZ*_coords[DX];
  float y = _coords[Y] + projZ*_coords[DY];
  float xx = _cov[XX] +projZ*projZ*_cov[DXDX] +2*projZ*_cov[XDX];
  float xy = _cov[XY] +projZ*projZ*_cov[DXDY] +projZ*(_cov[XDY] + _cov[YDX]);
  float yy = _cov[YY] +projZ*projZ*_cov[DYDY] +2*projZ*_cov[YDY];
  //now move it
  ErrorPoint2D point(x+projX,y+projY,xx,xy,yy);
  //and rotate it
  point.rotate(projPhi);
  return point;
}
};
