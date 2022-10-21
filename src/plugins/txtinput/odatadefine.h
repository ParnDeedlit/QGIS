#ifndef ODATADEFINE_H
#define ODATADEFINE_H

#include <QString>

enum FileSuffix {
    MS = 0,
    SX = 1,
    TP = 2,
    ZB = 3
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


bool MetadataLineStringMatchReg(QString linestring, QString patch);
double MetadataLineStringToDoubleByReg(QString linestring, QString patch);
int MetadataLineStringToIntByReg(QString linestring, QString patch);
QString MetadataLineStringToStringByReg(QString linestring, QString patch);

int AttributeLineStringTypeCount(QString linestring, LayerType type);
int GeometryLineStringTypeCount(QString linestring, LayerType type);

#endif // ODATADEFINE_H
