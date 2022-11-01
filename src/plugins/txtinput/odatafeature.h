#ifndef ODATA_FEATURE_H
#define ODATA_FEATURE_H
#include <stdio.h>

#include <QString>
#include <QVariant>

#include "odatadefine.h"
#include "odataattributes.h"
#include "odatageometry.h"

using namespace std;

class ODataFeature {
public:
    FeatureType type;
    ODataAttributes properties;
    ODataGeometry geometry;
    bool isValid = true;

    QString toPostgis(QString tablename, vector<ODataAttributeMeta> fields);
    ODataFeature* setGeometryPoint(QString linestring, int validIndex, MapMataData *mapMatadata);
    ODataFeature* setGeometryLine(QString linestring, MapMataData *mapMatadata);
    ODataFeature* setGeometryArea(QString linestring, int ringindex, MapMataData *mapMatadata);
    ODataFeature* setProperties(QString linestring, vector<ODataAttributeMeta> fields, int validIndex);
};

#endif // ODATA_FEATURE_H
