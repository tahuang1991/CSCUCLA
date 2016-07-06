#ifndef CSCUCLA_GEMCSCTUPLEANALYSIS_POINTS_H
#define CSCUCLA_GEMCSCTUPLEANALYSIS_POINTS_H
namespace CSCGEMTuples {

class Point2D {
public:
  Point2D() : _x(0), _y(0) {}
  Point2D(double _x, double _y) : _x(_x),_y(_y){}
  double x() const {return _x;}
  double y() const {return _y;}
  double& x() {return _x;}
  double& y() {return _y;}

  void set(double newX, double newY);
  Point2D & operator+=(const Point2D &rhs);
  Point2D & operator-=(const Point2D &rhs);
  Point2D operator+(const Point2D &other) const;
  Point2D operator-(const Point2D &other) const;
  Point2D operator*(const double scale) const;
  Point2D operator/(const double scale) const;

  /// Return a new Point2D, rotated by an angle phi
  Point2D rotate(double phi) const;
  /// Return a new Point2D, rotated by an angle defined by it's cosine and sine
  Point2D rotateCosSin( double c, double s) const;



private:
  double _x;
  double _y;
};

class Error2D {
public:
  Error2D() : _xx(0), _xy(0), _yy(0) {}
  Error2D(double _xx, double _xy, double _yy) : _xx(_xx),_xy(_xy),_yy(_yy){}
  double xx() const {return _xx;}
  double xy() const {return _xy;}
  double yy() const {return _yy;}
  double& xx() {return _xx;}
  double& xy() {return _xy;}
  double& yy() {return _yy;}

  void set(double newXX, double newXY, double newYY);
  Error2D & operator+=(const Error2D &rhs);
  //For when adding points...equivalent to a +=
  Error2D & operator-=(const Error2D &rhs) {return (*this) += rhs;}
  Error2D operator+(const Error2D &other) const;
  Error2D operator-(const Error2D &other) const {return (*this) + other;}

  Error2D scale(double s) const;

  /// Return a new Error2D, rotated by an angle phi
  Error2D rotate(double phi) const;
  /// Return a new Error2D, rotated by an angle defined by it's cosine and sine
  Error2D rotateCosSin( double c, double s) const;


private:
  double _xx;
  double _xy;
  double _yy;
};


class ErrorPoint2D {
public:
  ErrorPoint2D() {}
  ErrorPoint2D(double _x, double _y, double _xx, double _xy, double _yy) : _point(_x,_y),_error(_xx,_xy,_yy){}
  ErrorPoint2D(const Point2D& newPoint, const Error2D& newError) : _point(newPoint),_error(newError){}
  double x() const {return _point.x();}
  double y() const {return _point.y();}
  double& x() {return _point.x();}
  double& y() {return _point.y();}

  double  cov_xx() const {return _error.xx();}
  double  cov_xy() const {return _error.xy();}
  double  cov_yy() const {return _error.yy();}
  double& cov_xx() {return _error.xx();}
  double& cov_xy() {return _error.xy();}
  double& cov_yy() {return _error.yy();}

  double  error_x() const;
  double  error_y() const;

  const Point2D& point() const {return _point;}
  const Error2D& error() const {return _error;}
  Point2D& point() {return _point;}
  Error2D& error() {return _error;}

  void set(double newX, double newY, double newXX, double newXY, double newYY);
  void set(const Point2D& newPoint, const Error2D& newError);
  ErrorPoint2D& operator+=(const ErrorPoint2D &rhs);

  ErrorPoint2D& operator-=(const ErrorPoint2D &rhs);
  ErrorPoint2D  operator+(const ErrorPoint2D &other) const;
  ErrorPoint2D  operator-(const ErrorPoint2D &other) const;
  ErrorPoint2D  operator*(const double scale) const;
  ErrorPoint2D  operator/(const double scale) const;

  /// Return a new ErrorPoint2D, rotated by an angle phi
  ErrorPoint2D rotate(double phi) const;
  /// Return a new ErrorPoint2D, rotated by an angle defined by it's cosine and sine
  ErrorPoint2D rotateCosSin( double c, double s) const;



private:
  Point2D _point;
  Error2D _error;
};


};
#endif
