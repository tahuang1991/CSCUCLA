
#include "../include/Points.h"
#include <cmath>

namespace CSCGEMTuples {

void Point2D::set(double newX, double newY) { _x = newX; _y = newY;}
Point2D & Point2D::operator+=(const Point2D &rhs) {
  x() += rhs.x();
  y() += rhs.y();
  return *this;
}
Point2D & Point2D::operator-=(const Point2D &rhs) {
  x() -= rhs.x();
  y() -= rhs.y();
  return *this;
}
Point2D Point2D::operator+(const Point2D &other) const {
  return Point2D(*this) += other;
}
Point2D Point2D::operator-(const Point2D &other) const {
  return Point2D(*this) -= other;
}
Point2D Point2D::operator*(const double scale) const {
  Point2D result = *this;
  result.x() *= scale;
  result.y() *= scale;
  return result;
}
Point2D Point2D::operator/(const double scale) const {
  Point2D result = *this;
  result.x() /= scale;
  result.y() /= scale;
  return result;

}

Point2D Point2D::rotate(double phi) const {
  return rotateCosSin( cos(phi), sin(phi));
}
Point2D Point2D::rotateCosSin( double c, double s) const {
  return Point2D(c*x() -s*y(), c*y() + s*x()  );
}


void Error2D::set(double newXX, double newXY, double newYY) {
  xx() = newXX;
  xy() = newXY;
  yy() = newYY;
}
Error2D& Error2D::operator+=(const Error2D &rhs) {
 xx() += rhs.xx();
 xy() += rhs.xy();
 yy() += rhs.yy();
 return *this;
}

Error2D Error2D::operator+(const Error2D &other) const {
  return Error2D(*this) += other;
}

Error2D Error2D::scale(double s) const {
  double s2 = s*s;
  return Error2D(s2*xx(), s2*xy(), s2*yy());
}


Error2D Error2D::rotate(double phi) const {
  return rotateCosSin( cos(phi), sin(phi));
}
Error2D Error2D::rotateCosSin( double c, double s) const {
  return Error2D( ( (c*c)*xx() + (s*s)*yy() - 2.f*(c*s)*xy()),
      ( (c*s)*(xx() - yy()) +  (c*c-s*s)*xy()) ,
      ( (s*s)*xx() + (c*c)*yy() + 2.f*(c*s)*xy())
                );

}


double  ErrorPoint2D::error_x() const {return _error.xx() < 0 ? 0 : std::sqrt(_error.xx());}
double  ErrorPoint2D::error_y() const {return _error.yy() < 0 ? 0 : std::sqrt(_error.yy());}

void ErrorPoint2D::set(double newX, double newY, double newXX, double newXY, double newYY) {
  _point.set(newX,newY);
  _error.set(newXX,newXY,newYY);
}
void ErrorPoint2D::set(const Point2D& newPoint, const Error2D& newError) {_point = newPoint; _error = newError;}
ErrorPoint2D& ErrorPoint2D::operator+=(const ErrorPoint2D &rhs) {
  _point += rhs.point();
  _error += rhs.error();
  return *this;
}
ErrorPoint2D& ErrorPoint2D::operator-=(const ErrorPoint2D &rhs) {
  _point -= rhs.point();
  _error -= rhs.error();
  return *this;
}
ErrorPoint2D  ErrorPoint2D::operator+(const ErrorPoint2D &other) const {
  return ErrorPoint2D(*this) += other;
}
ErrorPoint2D  ErrorPoint2D::operator-(const ErrorPoint2D &other) const {
  return ErrorPoint2D(*this) -= other;
}
ErrorPoint2D  ErrorPoint2D::operator*(const double scale) const {
  Point2D newPoint = point()*scale;
  Error2D newError = error().scale(scale);
  return ErrorPoint2D(newPoint,newError);
}
ErrorPoint2D  ErrorPoint2D::operator/(const double scale) const {
  Point2D newPoint = point()/scale;
  Error2D newError = error().scale(1.f/scale);
  return ErrorPoint2D(newPoint,newError);
}
ErrorPoint2D  ErrorPoint2D::rotate(double phi) const {
  Point2D newPoint = point().rotate(phi);
  Error2D newError = error().rotate(phi);
  return ErrorPoint2D(newPoint,newError);
}
ErrorPoint2D  ErrorPoint2D::rotateCosSin( double c, double s) const {
  Point2D newPoint = point().rotateCosSin(c,s);
  Error2D newError = error().rotateCosSin(c,s);
  return ErrorPoint2D(newPoint,newError);
}


};
