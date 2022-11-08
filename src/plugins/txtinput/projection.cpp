#include "projection.h"
#include "ellipsoid.h"

Projection::Projection() {
    this->projectLatitude = 0;
    this->projectLatitude = 0;
}

Projection::Projection(Ellipsoid ellipsoid, double projectLongitude, double projectLatitude) {
    this->ellipsoid = ellipsoid;
    this->projectLatitude = projectLatitude;
    this->projectLatitude = projectLatitude;
}

void Projection::setEllipsoid(Ellipsoid ellipsoid) {
  this->ellipsoid = ellipsoid;
}

void Projection::setProjectLongitude(double longitude) {
  this->projectLongitude = longitude;
}

void Projection::setProjectLatitude(double latitude) {
  this->projectLatitude = latitude;
}

/**
 * 球面到平面的正向投影变换
 */
PlaneCoordinate Projection::project(GeodeticCoordinate coord) {
  PlaneCoordinate plane;
  plane.x = coord.lng;
  plane.y = coord.lat;
  return plane;
}

/**
 * 平面到球面的反向投影变换
 */
GeodeticCoordinate Projection::inverse(PlaneCoordinate coord) {
  GeodeticCoordinate geo;
  geo.lng = coord.x;
  geo.lat = coord.y;
  return geo;
}
