#include "odatageometry.h"

void ODataPoint::setCoordinates(double x, double y) {
    coordinates.x = x;
    coordinates.y = y;
}
void ODataPoint::setAuxiliary(double x, double y) {
    auxiliary.x = x;
    auxiliary.y = y;
}

QString ODataPoint::toWKT() {
    QString wkt;
    wkt = "POINT("
            + QString::number(coordinates.x)
            + " "
            + QString::number(coordinates.y)
            + ")";
    printf(QString(wkt + "ODataPoint::toWKT \r\n").toStdString().c_str());
    return wkt;
}

void ODataPoint::unprojtion(ProjType type, ProjUnit unit, double offx, double offy) {
    if (type == ProjType::Equal_Angle_Cone && unit == ProjUnit::Second) {
        coordinates.x = offx + coordinates.x / 3600;
        coordinates.y = offy + coordinates.y / 3600;
        auxiliary.x = offx + auxiliary.x / 3600;
        auxiliary.y = offy + auxiliary.y / 3600;
    }
}

void ODataLine::setCoordinates(BaseLine &line) {

}

void ODataLine::appendCoordinate(double x, double y) {
    BasePoint point(x, y);
    coordinates.push_back(point);
}

QString ODataLine::toWKT() {
    QString wkt("");
    wkt += "LINESTRING(";
    int count = coordinates.size();
    for (int i = 0; i < count; i++) {
        BasePoint point = coordinates[i];
        wkt = wkt
                + QString::number(point.x)
                + " "
                + QString::number(point.y);
        if (i < count - 1) {
            wkt += ",";
        }
    }

    wkt += ")";
    return wkt;
}

void ODataLine::unprojtion(ProjType type, ProjUnit unit, double offx, double offy) {
    if (type == ProjType::Equal_Angle_Cone && unit == ProjUnit::Second) {
        int count = coordinates.size();
        for (int i = 0; i < count; i++) {
            coordinates[i].x = offx + (double)(coordinates[i].x / 3600);
            coordinates[i].y = offy + (double)(coordinates[i].y / 3600);
        }
    }
}

void ODataPolygon::setCoordinates(BaseLine &line, int ring_index) {

}

void ODataPolygon::appendCoordinate(double x, double y, int ring_index) {
    BasePoint point(x, y);
    int size = coordinates.size();
    if (ring_index >= size) {
        BaseLine emptyline;
        emptyline.push_back(point);
        coordinates.push_back(emptyline);
    } else {
        coordinates[ring_index].push_back(point);
    }
}

bool ODataPolygon::isValidPolygon() {
    return true;
}

bool ODataPolygon::fixToValidPolygon() {
    return true;
}

QString ODataPolygon::toWKT() {
    QString wkt;
    wkt += "POLYGON(";
    int ringcount = coordinates.size();
    for (int i = 0; i < ringcount; i++) {
        BaseLine line = coordinates[i];
        int pointcouts = line.size();
        wkt += "(";
        for (int j = 0; j < pointcouts; j++) {
            BasePoint point = line[j];
            wkt = wkt
                    + QString::number(point.x)
                    + " "
                    + QString::number(point.y);
            if (j < pointcouts - 1) {
                wkt += ",";
            }
        }
        wkt += ")";
        if (i < ringcount - 1) {
            wkt += ",";
        }
    }
    wkt += ")";
    return wkt;
}

void ODataPolygon::unprojtion(ProjType type, ProjUnit unit, double offx, double offy) {
    if (type == ProjType::Equal_Angle_Cone && unit == ProjUnit::Second) {
        int count = coordinates.size();
        for (int i = 0; i < count; i++) {
            int points = coordinates[i].size();
            for (int j = 0; j < points; j++) {
                coordinates[i][j].x = offx + (double)(coordinates[i][j].x / 3600);
                coordinates[i][j].y = offy + (double)(coordinates[i][j].y / 3600);
            }
        }
    }
}

void ODataGeometry::unprojection(ProjType ptype, ProjUnit punit, double offx, double offy){
    if (type == FeatureType::POINT) {
        point.unprojtion(ptype, punit, offx, offy);
    } else if (type == FeatureType::LINESTRING) {
        line.unprojtion(ptype, punit, offx, offy);
    } else if (type == FeatureType::POLYGON) {
        polygon.unprojtion(ptype, punit, offx, offy);
    }
}

QString ODataGeometry::toWKT() {
    QString wkt;
    if (type == FeatureType::POINT) {
        wkt = point.toWKT();
    } else if (type == FeatureType::LINESTRING) {
        wkt = line.toWKT();
    } else if (type == FeatureType::POLYGON) {
        wkt = polygon.toWKT();
    }
    // printf(QString(" ODataGeometry::toWKT " + QString::number(type) + " \r\n").toStdString().c_str());
    return wkt;
}
