#ifndef ODATADEFINE_H
#define ODATADEFINE_H

#include <stdio.h>
#include <QString>

using namespace std;

enum FileSuffix {
    MS = 0,
    SX = 1,
    TP = 2,
    ZB = 3
};

enum OdataVersion {
    ODATA_1_0 = 0,
    ODATA_2_0 = 1,
    ENTITY_1_0 = 2
};

class  File {
public:
    QString name;
    QString code;
    QString layername;
    QString filetype;
    QString path;
    QString prefix;
    QString suffix;
    QString cn_name;
    QString uri;
};

class FileGroup {
public:
    QString layername;
    File matadata;
    File attrbution;
    File topylogy;
    File geometry;
};

enum ProjType {
    Equal_Angle_Cone = 0,
    Equal_Arc_Lonlat = 1,
    Gauss = 2
};

enum ProjUnit {
    Miter = 0,
    Degree = 1,
    Minite = 2,
    Second = 3,
};

enum OutputFormat {
    PostGIS = 0,
    GeoPackage = 1
};

class MapMataData {
public:
    OdataVersion version;
    QString name;
    File file;

    QString origin_bbox;
    QString lnglat_polygon;

    double a;
    double e;
    QString datum;

    ProjType projType;
    ProjUnit projUnit;

    double lon_0;
    double lat_1;
    double lat_2;
    QString zone_type;
    int gauss_number;

    double scale;
    double origin_x;
    double origin_y;

    OutputFormat outputFormat;
};

class Map {
public:
    MapMataData metadata;
    vector<FileGroup*> groups;
};

enum LayerType {
    Point = 0,
    Line = 1,
    Area = 2,
    Anno = 3
};

enum FeatureType {
    POINT = 0,
    LINESTRING = 1,
    POLYGON = 2,
};

enum FieldType {
    STRING = 0,
    INT = 1,
    DOUBLE = 2,
};

QString gb2312ToUtf8(QString gb2312);
QString utf8ToGb2312(QString utf8);

ProjType projTypeNameToProjType(QString name);
QString projTypeToProjTypeName(ProjType type);
ProjUnit projUnitNameToProjUnit(QString name);
QString projUnitToProjUnitName(ProjUnit unit);

bool MetadataLineStringMatchReg(QString linestring, QString patch);
double MetadataLineStringToDoubleByReg(QString linestring, QString patch);
int MetadataLineStringToIntByReg(QString linestring, QString patch);
QString MetadataLineStringToStringByReg(QString linestring, QString patch);

int AttributeLineStringTypeCount(QString linestring, LayerType type, OdataVersion version = OdataVersion::ODATA_1_0);
int GeometryLineStringTypeCount(QString linestring, LayerType type, OdataVersion version = OdataVersion::ODATA_1_0);

bool GeometryLineCoordCount(QString linestring, int &index, int &count, OdataVersion version = OdataVersion::ODATA_1_0);
bool GeometryAreaRingCount(QString linestring, int &index, int &count, OdataVersion version = OdataVersion::ODATA_1_0);
bool GeometryAreaCoordCount(QString linestring, int &count, OdataVersion version = OdataVersion::ODATA_1_0);

#endif // ODATADEFINE_H
