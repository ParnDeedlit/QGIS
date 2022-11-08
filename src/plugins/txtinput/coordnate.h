#ifndef COORDNATE_H
#define COORDNATE_H

class CartesianCoordinate {
public:
  double x;
  double y;
  double z;

  CartesianCoordinate(double x = 0, double y = 0, double z = 0) {
    this->x = x;
    this->y = y;
    this->z = z;
  }
};

/**
 * 大地坐标
 */
class GeodeticCoordinate {
public:
  double lng;
  double lat;
  double height;

  GeodeticCoordinate(double lng = 0, double lat = 0, double height = 0) {
    this->lng = lng;
    this->lat = lat;
    this->height = height;
  }
};

/**
 * 平面坐标
 */
class PlaneCoordinate {
public:
  double x;
  double y;

  PlaneCoordinate(double x = 0, double y = 0) {
    this->x = x;
    this->y = y;
  }
};

#endif // COORDNATE_H
