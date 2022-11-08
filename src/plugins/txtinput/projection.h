#ifndef PROJECTION_H
#define PROJECTION_H

#include "ellipsoid.h"
#include "coordnate.h"

/**
 * 投影基类
 */
class Projection {
public:
  /**
   * 默认椭球体
   */
  Ellipsoid ellipsoid;

  /**
   * 投影经度，单位弧度
   */
  double projectLongitude = 0;

  /**
   * 投影纬度，单位弧度
   */
  double projectLatitude = 0;

  Projection();

  Projection(Ellipsoid ellipsoid, double projectLongitude, double projectLatitude);

  void setEllipsoid(Ellipsoid ellipsoid);

  void setProjectLongitude(double longitude);

  void setProjectLatitude(double latitude);

  /**
   * 球面到平面的正向投影变换
   */
  PlaneCoordinate project(GeodeticCoordinate coord);

  /**
   * 平面到球面的反向投影变换
   */
  GeodeticCoordinate inverse(PlaneCoordinate coord);
};

#endif // PROJECTION_H
