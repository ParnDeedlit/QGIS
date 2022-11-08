#ifndef GAUSS_H
#define GAUSS_H

#include <cmath>

#include "projection.h"
#include "ellipsoid.h"

class GaussProjection: public Projection {
public:
  double radian = M_PI / 180;

  double longitudeOfNaturalOrigin = 0;

  double latitudeOfNaturalOrigin = 0;

  double falseEasting = 500000;

  double falseNorthing = 0;

  GaussProjection() {};

  PlaneCoordinate project(
    GeodeticCoordinate coord,
    bool isRadian = false
  );

  GeodeticCoordinate inverse(
    PlaneCoordinate coord,
    bool isRadian = false
  );

};

#endif // GAUSS_H
