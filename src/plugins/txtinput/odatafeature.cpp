#include "odatafeature.h"
#include "odatadefine.h"

//#include <spatialite/gaiageo.h>
//#include <spatialite.h>

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
    value = value + "ST_GeometryFromText('" + geometry.toWKT() + "', 4326)";

    key += " )";
    value += " )";

    sql = sql + key + " VALUES " + value + ";";
    return sql;
}

QString ODataFeature::toPostgisTable(QString tablename, vector<ODataAttributeMeta> fields) {
    // INSERT INTO geotable ( the_geom, the_name ) VALUES ( ST_GeomFromEWKT('SRID=312;POINTM(-126.4 45.32 15)'), 'A Place' );
    QString sql = "INSERT INTO " + tablename;
    QString key = " ( ";

    // attrbutes
    int count = properties.size();
    int meta_count = fields.size();
    if (count <= meta_count) {
        for (int i = 0; i < count; i++) {
            ODataAttribute attr = properties[i];
            key = key + fields[i].fieldname + ", ";
        }
    }

    // geometry
    key = key + " geom";
    key += " )";
    sql = sql + key + " VALUES ";
    return sql;
}

QString ODataFeature::toPostgisVaules(QString tablename, vector<ODataAttributeMeta> fields) {
    // INSERT INTO geotable ( the_geom, the_name ) VALUES ( ST_GeomFromEWKT('SRID=312;POINTM(-126.4 45.32 15)'), 'A Place' );
    QString value = " ( ";

    // attrbutes
    int count = properties.size();
    int meta_count = fields.size();
    if (count <= meta_count) {
        for (int i = 0; i < count; i++) {
            ODataAttribute attr = properties[i];
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
    value = value + "ST_GeometryFromText('" + geometry.toWKT() + "', 4326)";
    value += " )";

    return value;
}

QString ODataFeature::toGeoPackage(QString tablename, vector<ODataAttributeMeta> fields) {
    QString sql = "INSERT INTO " + tablename;
    QString key = " ( ";
    QString value = " VALUES (";

    // attrbutes
    int count = properties.size();
    int meta_count = fields.size();
    if (count == meta_count) {
        for (int i = 0; i < count; i++) {
            ODataAttribute attr = properties[i];
            if (i >= count - 1) {
                key = key + fields[i].fieldname + " ";
            } else {
                key = key + fields[i].fieldname + ", ";
            }
        }
        for (int i = 0; i < count; i++) {
            if (i >= count - 1) {
                value = value + " ?);";
            } else {
                value = value + " ?, ";
            }
        }
    }

    // geometry
    // key = key + " geom";
    key += " )";
    sql = sql + key + value;
    return sql;
}

QString ODataFeature::toGeoPackageTable(QString tablename, vector<ODataAttributeMeta> fields) {
    // INSERT INTO geotable ( the_geom, the_name ) VALUES ( ST_GeomFromEWKT('SRID=312;POINTM(-126.4 45.32 15)'), 'A Place' );
    QString sql = "INSERT INTO " + tablename;
    QString key = " ( ";
    QString value = " VALUES (";

    // attrbutes
    int count = properties.size();
    int meta_count = fields.size();
    if (count <= meta_count) {
        for (int i = 0; i < count; i++) {
            ODataAttribute attr = properties[i];
            if (i >= count - 1) {
                key = key + fields[i].fieldname + " ";
            } else {
                key = key + fields[i].fieldname + ", ";
            }
            // key = key + fields[i].fieldname + ", ";
        }
    }

    // geometry
    key = key + ", geom";
    key += " )";
    sql = sql + key + " VALUES ";
    return sql;
}

QString ODataFeature::toGeoPackageVaules(QString tablename, vector<ODataAttributeMeta> fields) {
    QString value = " ( ";

    // attrbutes
    int count = properties.size();
    int meta_count = fields.size();
    if (count <= meta_count) {
        for (int i = 0; i < count; i++) {
            ODataAttribute attr = properties[i];
            if (attr.type.compare("string") == 0) {
                value = value + "'" + attr.value.toString() + "' ";
            } else if (attr.type.compare("int") == 0) {
                value = value + QString::number(attr.value.toInt());
            } else if (attr.type.compare("double") == 0) {
                value = value + QString::number(attr.value.toDouble());
            }
            if (i  < count - 1) {
                value = value + ", ";
            }
        }
    }

    value = value + ", ?";
    value += " );";

    return value;
}

void ODataFeature::createGeoPackageGeometryBlob(unsigned char **blob, int &blob_size) {
//    gaiaGeomCollPtr geo = NULL;
//    /* preparing the geometry to insert */
//    geo = gaiaAllocGeomColl();
//    geo->Srid = 4326;
//    gaiaAddPointToGeomColl (geo, 0, 0);
//    /* transforming this geometry into the SpatiaLite BLOB format */
//    gaiaToSpatiaLiteBlobWkb (geo, &(*blob), blob_size);
//    /* we can now destroy the geometry object */
//    gaiaFreeGeomColl (geo);
}

void ODataFeature::releaseGeoPackageGeometryBlob(unsigned char **blob, int &blob_size) {

}


ODataFeature* ODataFeature::setPropertiesSplit(QString linestring, vector<ODataAttributeMeta> fields, int validIndex) {
    QString str;
    QString reg("\\s+");
    int first_index = 1;
    QRegularExpression re_a(reg);
    QStringList lists = linestring.split(re_a);
    int fieldcount = fields.size();
    int listcount = lists.size();

    bool hasValid = false;
    for (int i = 0; i < listcount; i++) {
        QRegularExpressionMatch match_a = re_a.match(lists[i]);
//        printf("propers reg  %d %s\r\n", i, lists[i].toStdString().c_str());
        if(match_a.hasMatch() || lists[i].compare("") == 0) {
//            printf("propers reg  match %d %c \r\n", i, lists[i].toStdString().c_str());
            if (!hasValid) {
                first_index++;
            } else {
                break;
            }
        } else {
            hasValid = true;
            break;
        }
    }
//    printf("propers offset  %d \r\n", first_index);

//    for (int i = 0; i < listcount; i++) {
//        printf("propers reg  %d %s\r\n", i, lists[i].toStdString().c_str());
//    }

    if (listcount < fieldcount) {
        return this;
    }
    for (int i = 0; i < fieldcount; i++) {
        if (i + first_index < lists.size()) {
            str = lists[i + first_index];
            ODataAttributeMeta meta = fields[i];
            if (meta.fieldtype == "string") {
                ODataAttribute attr;
                attr.type = "string";
                attr.value = str;
                properties.push_back(attr);
            } else if (meta.fieldtype == "int") {
                ODataAttribute attr;
                attr.type = "int";
                attr.value = str.toInt();
                properties.push_back(attr);
            } else if (meta.fieldtype == "double") {
                ODataAttribute attr;
                attr.type = "double";
                attr.value = str.toDouble();
                properties.push_back(attr);
            }
//            printf("%d %s  \r\n", i, str.toStdString().c_str());
        }
    }

    return this;
}

ODataFeature* ODataFeature::setPropertiesReg(QString linestring, vector<ODataAttributeMeta> fields, int validIndex) {
    QString str;
    const int first_index = 2;
    QString reg("\\s*(\\d+)");
    int count = fields.size();
    for (int i = 0; i < count; i++) {
        ODataAttributeMeta meta = fields[i];
        if (meta.fieldtype.compare("double") == 0) {
            reg += "\\s*(NULL|-?[1-9]\\d*\\.\\d*|0\\.\\d*[1-9]\\d*|0?\\.0+|0)";
        } else if (meta.fieldtype.compare("int") == 0) {
            if (meta.fieldname.compare("编码") == 0) {
                // reg += "\\s*(NULL|\\d)";
                reg += "\\s*(NULL|-?\\d+)";
            } else {
                reg += "\\s*(NULL|-?\\d+)";
            }
        } else if (meta.fieldtype.compare("string") == 0) {
            reg += "\\s*([\\w\u4E00-\u9FA5A-Za-z0-9_、]+)";
        }
    }

    printf("reg %s\r\n", reg.toStdString().c_str());

    QRegularExpression re_a(reg);
    QRegularExpressionMatch match_a = re_a.match(linestring);
    if(match_a.hasMatch()) {
        isValid = true;
        //printf(QString(match_a.captured(0) + "\r\n").toStdString().c_str());
        QString index = match_a.captured(1);
        //printf("cur index %d,  valid index %d \r\n", index.toInt() ,validIndex);
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
                    attr.value = str.toInt();
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

ODataFeature* ODataFeature::setGeometryPoint(QString linestring, int validIndex, MapMataData *mapMatadata) {
    geometry.type = FeatureType::POINT;
    QString strx;
    QString stry;
    QString strx1;
    QString stry1;
    const int first_index = 2;
    QString reg("\\s*(\\d+)");

    if (mapMatadata->version == OdataVersion::ODATA_1_0) {
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
                geometry.point.coordinates.x = strx.toDouble();
                geometry.point.coordinates.y = stry.toDouble();
                if (match_a.capturedLength() >= 5) {
                    strx1 = match_a.captured(2 + first_index);
                    stry1 = match_a.captured(3 + first_index);
                    geometry.point.auxiliary.x = strx1.toDouble();
                    geometry.point.auxiliary.y = stry1.toDouble();
                }
                // printf("x %f,  y %f \r\n", strx.toDouble() ,stry.toDouble());
            } else {
                isValid = false;
            }
        } else {
            isValid = false;
        }
    } else if (mapMatadata->version == OdataVersion::ODATA_2_0
        || mapMatadata->version == OdataVersion::ENTITY_1_0) {
        reg += "\\s*" + ODataDoubleReg;
        reg += "\\s*" + ODataDoubleReg;
        int index = 0;

        QRegularExpression re_a(reg);
        QRegularExpressionMatchIterator i = re_a.globalMatch(linestring);
        if(i.hasNext()) {
            isValid = true;
            QRegularExpressionMatch match_a = i.next();
            bool validx = false;
            bool validy = false;
            double px = match_a.captured(2).toDouble(&validx);
            double py = match_a.captured(3).toDouble(&validy);
            if (validx && validy) {
                if (index == 0) {
                    geometry.point.coordinates.x = px;
                    geometry.point.coordinates.y = py;
                } else {
                    geometry.point.auxiliary.x = px;
                    geometry.point.auxiliary.y = py;
                }
            }
            index++;
        } else {
            isValid = false;
        }
    }


    return this;
}

ODataFeature* ODataFeature::setGeometryLine(QString linestring, MapMataData *mapMatadata) {
    geometry.type = FeatureType::LINESTRING;

    QString reg("");
    reg += "\\s*" + ODataDoubleReg;
    reg += "\\s*" + ODataDoubleReg;

    QRegularExpression re_a(reg);
    QRegularExpressionMatchIterator i = re_a.globalMatch(linestring);
    while (i.hasNext()) {
        isValid = true;
        QRegularExpressionMatch match_a = i.next();
        bool validx = false;
        bool validy = false;
        double px = match_a.captured(1).toDouble(&validx);
        double py = match_a.captured(2).toDouble(&validy);
        if (validx && validy) {
            geometry.line.appendCoordinate(px, py);
        }
    }
    return this;
}

ODataFeature* ODataFeature::setGeometryArea(QString linestring, int ringindex, MapMataData *mapMatadata) {
    geometry.type = FeatureType::POLYGON;

    QString reg("");
    reg += "\\s*" + ODataDoubleReg;
    reg += "\\s*" + ODataDoubleReg;

    QRegularExpression re_a(reg);
    QRegularExpressionMatchIterator i = re_a.globalMatch(linestring);
    while (i.hasNext()) {
        isValid = true;
        QRegularExpressionMatch match_a = i.next();
        bool validx = false;
        bool validy = false;
        double px = match_a.captured(1).toDouble(&validx);
        double py = match_a.captured(2).toDouble(&validy);
        if (validx && validy) {
            geometry.polygon.appendCoordinate(px, py, ringindex);
        }
    }
    return this;
}

ODataFeature* ODataFeature::setGeometryAnno(QString linestring, int validIndex, MapMataData *mapMatadata) {
    geometry.type = FeatureType::POINT;
    QString strx;
    QString stry;
    QString strx1;
    QString stry1;
    const int first_index = 2;
    QString reg("\\s*(\\d+)");

    if (mapMatadata->version == OdataVersion::ODATA_1_0) {
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
    } else if (mapMatadata->version == OdataVersion::ODATA_2_0
       || mapMatadata->version == OdataVersion::ENTITY_1_0) {
        reg += "\\s*" + ODataDoubleReg;
        reg += "\\s*" + ODataDoubleReg;
        int index = 0;

        QRegularExpression re_a(reg);
        QRegularExpressionMatchIterator i = re_a.globalMatch(linestring);

        printf("anno %s %b %s \r\n", reg.toStdString().c_str(),
               i.hasNext(), linestring.toStdString().c_str());

        if(i.hasNext()) {
            isValid = true;
            QRegularExpressionMatch match_a = i.next();
            bool validx = false;
            bool validy = false;
            double px = match_a.captured(2).toDouble(&validx);
            double py = match_a.captured(3).toDouble(&validy);
            if (validx && validy) {
                if (index == 0) {
                    geometry.point.coordinates.x = px;
                    geometry.point.coordinates.y = py;
                } else {
                    geometry.point.auxiliary.x = px;
                    geometry.point.auxiliary.y = py;
                }
            }
            index++;
        } else {
            isValid = false;
        }
    }


    return this;
}
