#ifndef CSCUCLA_GEMCSCTUPLEANALYSIS_POINTS_H
#define CSCUCLA_GEMCSCTUPLEANALYSIS_POINTS_H
namespace CSCGEMTuples {

class Point2D {
public:
  Point2D() : _x(0), _y(0) {}
  Point2D(float _x, float _y) : _x(_x),_y(_y){}
  float x() const {return _x;}
  float y() const {return _y;}
  float& x() {return _x;}
  float& y() {return _y;}

  void set(float newX, float newY);
  Point2D & operator+=(const Point2D &rhs);
  Point2D & operator-=(const Point2D &rhs);
  Point2D operator+(const Point2D &other) const;
  Point2D operator-(const Point2D &other) const;
  Point2D operator*(const float scale) const;
  Point2D operator/(const float scale) const;

  /// Return a new Point2D, rotated by an angle phi
  Point2D rotate(float phi) const;
  /// Return a new Point2D, rotated by an angle defined by it's cosine and sine
  Point2D rotateCosSin( float c, float s) const;



private:
  float _x;
  float _y;
};

class Error2D {
public:
  Error2D() : _xx(0), _xy(0), _yy(0) {}
  Error2D(float _xx, float _xy, float _yy) : _xx(_xx),_xy(_xy),_yy(_yy){}
  float xx() const {return _xx;}
  float xy() const {return _xy;}
  float yy() const {return _yy;}
  float& xx() {return _xx;}
  float& xy() {return _xy;}
  float& yy() {return _yy;}

  void set(float newXX, float newXY, float newYY);
  Error2D & operator+=(const Error2D &rhs);
  //For when adding points...equivalent to a +=
  Error2D & operator-=(const Error2D &rhs) {return (*this) += rhs;}
  Error2D operator+(const Error2D &other) const;
  Error2D operator-(const Error2D &other) const {return (*this) - other;}

  Error2D scale(float s) const;

  /// Return a new Error2D, rotated by an angle phi
  Error2D rotate(float phi) const;
  /// Return a new Error2D, rotated by an angle defined by it's cosine and sine
  Error2D rotateCosSin( float c, float s) const;


private:
  float _xx;
  float _xy;
  float _yy;
};


class ErrorPoint2D {
public:
  ErrorPoint2D() {}
  ErrorPoint2D(float _x, float _y, float _xx, float _xy, float _yy) : _point(_x,_y),_error(_xx,_xy,_yy){}
  ErrorPoint2D(const Point2D& newPoint, const Error2D& newError) : _point(newPoint),_error(newError){}
  float x() const {return _point.x();}
  float y() const {return _point.y();}
  float& x() {return _point.x();}
  float& y() {return _point.y();}

  float  cov_xx() const {return _error.xx();}
  float  cov_xy() const {return _error.xy();}
  float  cov_yy() const {return _error.yy();}
  float& cov_xx() {return _error.xx();}
  float& cov_xy() {return _error.xy();}
  float& cov_yy() {return _error.yy();}

  float  error_x() const;
  float  error_y() const;

  const Point2D& point() const {return _point;}
  const Error2D& error() const {return _error;}
  Point2D& point() {return _point;}
  Error2D& error() {return _error;}

  void set(float newX, float newY, float newXX, float newXY, float newYY);
  void set(const Point2D& newPoint, const Error2D& newError);
  ErrorPoint2D& operator+=(const ErrorPoint2D &rhs);

  ErrorPoint2D& operator-=(const ErrorPoint2D &rhs);
  ErrorPoint2D  operator+(const ErrorPoint2D &other) const;
  ErrorPoint2D  operator-(const ErrorPoint2D &other) const;
  ErrorPoint2D  operator*(const float scale) const;
  ErrorPoint2D  operator/(const float scale) const;

  /// Return a new ErrorPoint2D, rotated by an angle phi
  ErrorPoint2D rotate(float phi) const;
  /// Return a new ErrorPoint2D, rotated by an angle defined by it's cosine and sine
  ErrorPoint2D rotateCosSin( float c, float s) const;



private:
  Point2D _point;
  Error2D _error;
};


};
#endif
