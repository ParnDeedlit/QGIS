#include "odatafeature.h"
#include "odatadefine.h"

#include <QString>
#include <QRegularExpression>
#include <QTextCodec>

QString ODataDoubleReg = "(NULL|-?[1-9]\\d*\\.\\d*|0\\.\\d*[1-9]\\d*|0?\\.0+|0)";

QString ODataFeature::toPostgis(QString tablename, vector<ODataAttributeMeta> fields) {
    // INSERT INTO geotable ( the_geom, the_name ) VALUES ( ST_GeomFromEWKT('SRID=312;POINTM(-126.4 45.32 15)'), 'A Place' );
    QString sql = "INSERT INTO " + tablename;
    QString key = " ( ";
    QString value = " ( ";

    // attrbutes
    int count = properties.size();
    int meta_count = fields.size();
    if (count == meta_count) {
        for (int i = 0; i < count; i++) {
            ODataAttribute attr = properties[i];
            key = key + fields[i].fieldname + ", ";
            if (attr.type.compare("string") == 0) {
                value = value + "'" + attr.value.toString() + "', ";
            } else if (attr.type.compare("int") == 0) {
                value = value + QString::number(attr.value.toInt()) + ", ";
            } else if (attr.type.compare("double") == 0) {
                value = value + QString::number(attr.value.toDouble()) + ", ";
            }
        }
    }

    // geometry
    key = key + " geom";
    value = value + "ST_GeometryFromText('" + geometry.toWKT() + "')";

    key += " )";
    value += " )";

    sql = sql + key + " VALUES " + value + ";";
    return sql;
}


ODataFeature* ODataFeature::setProperties(QString linestring, vector<ODataAttributeMeta> fields, int validIndex) {
    QString str;
    const int first_index = 2;
    QString reg("\\s*(\\d+)");
    int count = fields.size();
    for (int i = 0; i < count; i++) {
        ODataAttributeMeta meta = fields[i];
        if (meta.fieldtype.compare("double") == 0) {
            reg += "\\s*(NULL|-?[1-9]\\d*\\.\\d*|0\\.\\d*[1-9]\\d*|0?\\.0+|0)";
        } else if (meta.fieldtype.compare("int") == 0) {
            reg += "\\s*(NULL|-?\\d+)";
        } else if (meta.fieldtype.compare("string") == 0) {
            reg += "\\s*([\\w\u4E00-\u9FA5A-Za-z0-9_、]+)";
        }
    }

    QRegularExpression re_a(reg);
    QRegularExpressionMatch match_a = re_a.match(linestring);
    if(match_a.hasMatch()) {
        isValid = true;
        // printf(QString(match_a.captured(0) + "\r\n").toStdString().c_str());
        QString index = match_a.captured(1);
        // printf("cur index %d,  valid index %d \r\n", index.toInt() ,validIndex);
        if(index.toInt() == validIndex) {
            isValid = true;
            for(int i = 0; i < count; i++) {
                str = match_a.captured(i + first_index);
                ODataAttributeMeta meta = fields[i];
                if (meta.fieldtype == "string") {
                    ODataAttribute attr;
                    attr.type = "string";
                    attr.value = str;
                    properties.push_back(attr);
                } else if (meta.fieldtype == "int") {
                    ODataAttribute attr;
                    attr.type = "int";
                    attr.value = str.toUInt();
                    properties.push_back(attr);
                } else if (meta.fieldtype == "double") {
                    ODataAttribute attr;
                    attr.type = "double";
                    attr.value = str.toDouble();
                    properties.push_back(attr);
                }
            }
        } else {
            isValid = false;
        }
    } else {
        isValid = false;
    }
    return this;
}

ODataFeature* ODataFeature::setGeometryPoint(QString linestring, int validIndex) {
    geometry.type = FeatureType::POINT;
    QString strx;
    QString stry;
    QString strx1;
    QString stry1;
    const int first_index = 2;
    QString reg("\\s*(\\d+)");

    reg += "\\s*" + ODataDoubleReg;
    reg += "\\s*" + ODataDoubleReg;
    reg += "\\s*" + ODataDoubleReg;
    reg += "\\s*" + ODataDoubleReg;

    QRegularExpression re_a(reg);
    QRegularExpressionMatch match_a = re_a.match(linestring);
    if(match_a.hasMatch()) {
        isValid = true;
        // printf(QString(match_a.captured(2) + "  " + match_a.captured(3) + "\r\n").toStdString().c_str());
        QString index = match_a.captured(1);
        // printf("cur index %d,  valid index %d \r\n", index.toInt() ,validIndex);
        if(index.toInt() == validIndex) {
            strx = match_a.captured(0 + first_index);
            stry = match_a.captured(1 + first_index);
            strx1 = match_a.captured(2 + first_index);
            stry1 = match_a.captured(3 + first_index);
            geometry.point.coordinates.x = strx.toDouble();
            geometry.point.coordinates.y = stry.toDouble();
            geometry.point.auxiliary.x = strx1.toDouble();
            geometry.point.auxiliary.y = stry1.toDouble();
            // printf("x %f,  y %f \r\n", strx.toDouble() ,stry.toDouble());
        } else {
            isValid = false;
        }
    } else {
        isValid = false;
    }
    return this;
}
