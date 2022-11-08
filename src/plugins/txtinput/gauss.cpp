#include "gauss.h"
#include <cmath>

  PlaneCoordinate GaussProjection::project(
      GeodeticCoordinate coord,
      bool isRadian
  ) {
    PlaneCoordinate plane;

    double x = 0;
    double y = 0;
    double lng = coord.lng;
    double lat = coord.lat;
    if (!isRadian) {
      lng = (lng - longitudeOfNaturalOrigin) * radian;
      lat = (lat - latitudeOfNaturalOrigin) * radian;
    } else {
      lng = lng - longitudeOfNaturalOrigin / radian;
      lat = lat - latitudeOfNaturalOrigin / radian;
    }

    double cos1 = cos(lat);
    double X = ellipsoid.meridianArcLength(lat);
    double V = ellipsoid.secondAuxiliary(lat);
    double N = ellipsoid.poleCurvatureRadis / V;
    double m1 = lng * cos1;
    double m2 = m1 * m1;
    double m3 = m2 * m1;
    double m4 = m3 * m1;
    double m5 = m4 * m1;
    double m6 = m5 * m1;
    double t = tan(lat);
    double u = ellipsoid.secondeccentricity * cos1;
    double t2 = t * t;
    double t4 = t2 * t2;
    double u2 = u * u;
    double u4 = u2 * u2;

    double loopX =
      (1.0 / 2) * m2 +
      (1.0 / 24) * (5 - t2 + 9 * u2 + 4 * u4) * m4 +
      (1.0 / 720) * (61 - 58 * t2 + t4) * m6;
    x = X + N * t * loopX + falseNorthing;

    double loopY =
      m1 +
      (1.0 / 6) * (1 - t2 + u2) * m3 +
      (1.0 / 120) * (5 - 18 * t2 + t4 + 14 * u2 - 58 * u2 * t2) * m5;

    y = N * loopY + falseEasting;

    plane.x = y;
    plane.y = x;
    return plane;
  }

  GeodeticCoordinate GaussProjection::inverse(
          PlaneCoordinate coord,
          bool isRadian
  ) {
    GeodeticCoordinate geo;
    double x = coord.x;
    double y = coord.y;

    double Bf = ellipsoid.inverseMeridianArcLength(y - falseNorthing);
    double cos1 = cos(Bf);
    double V = ellipsoid.secondAuxiliary(Bf);
    double N = ellipsoid.poleCurvatureRadis / V;
    double m1 = (x - falseEasting) / N;
    double m2 = m1 * m1;
    double m3 = m2 * m1;
    double m4 = m3 * m1;
    double m5 = m4 * m1;
    double m6 = m5 * m1;
    double t = tan(Bf);
    double u = ellipsoid.secondeccentricity * cos1;
    double t2 = t * t;
    double t4 = t2 * t2;
    double u2 = u * u;
    double u4 = u2 * u2;

    double loopB =
      m2 -
      (1.0 / 12) * (5 + 3 * t2 + u2 - 4 * u4 - 9 * u2 * t2) * m4 +
      (1.0 / 360) * (61 + 90 * t2 + 46 * u2 + 45 * t4 - 252 * t2 * u2) * m6;
    double B = Bf - (1.0 / 2) * V * V * t * loopB;

    double loopL =
      m1 -
      (1.0 / 6) * (1 + 2 * t2 + u2) * m3 +
      (1.0 / 120) * (5 + 6 * u2 + 28 * t2 - 3 * u2 + 8 * u2 * t2) * m5;
    double L = (1.0 / cos1) * loopL;

    if (!isRadian) {
      B = B / radian + latitudeOfNaturalOrigin;
      L = L / radian + longitudeOfNaturalOrigin;
    } else {
      B = B + latitudeOfNaturalOrigin * radian;
      L = L + longitudeOfNaturalOrigin * radian;
    }

    geo.lng = L;
    geo.lat = B;

    return geo;
  }
