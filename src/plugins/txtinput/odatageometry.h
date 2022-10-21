#ifndef ODATA_GEOMETRY_H
#define ODATA_GEOMETRY_H

#include <stdio.h>
#include <QString>
#include "odatadefine.h"

using namespace std;

class BasePoint {
public:
    double x;
    double y;
    BasePoint() { x = 0; y = 0; };
    BasePoint(double a, double b) { x= a; y = b; };
};

typedef vector<BasePoint> BaseLine;
typedef vector<BaseLine> BasePolygon;

class ODataPoint {
public:
    BasePoint coordinates;
    BasePoint auxiliary;
public:
    void setCoordinates(double x, double y);
    void setAuxiliary(double x, double y);
    QString toWKT();
};

class ODataLine {
public:
    BaseLine coordinates;
public:
    void setCoordinates(BaseLine &line);
    void appendCoordinate(double x, double y);
    QString toWKT();
};

class ODataPolygon {
public:
    BasePolygon coordinates;
public:
    void setCoordinates(BaseLine &line, int ring_index = 0);
    void appendCoordinate(double x, double y, int ring_index = 0);
    bool isValidPolygon();
    bool fixToValidPolygon();
    QString toWKT();
};

class ODataGeometry {
public:
    FeatureType type;
    ODataPoint point;
    ODataLine line;
    ODataPolygon polygon;

public:
    QString toWKT();
};

#endif // ODATA_GEOMETRY_H
