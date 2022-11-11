#include "odatageometry.h"
#include "ellipsoid.h"
#include "gauss.h"

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
            + QString::number(coordinates.x, 'g', 16)
            + " "
            + QString::number(coordinates.y, 'g', 16)
            + ")";
    return wkt;
}

void ODataPoint::unprojtion(MapMataData *mata) {
    if (mata->version == OdataVersion::ODATA_1_0) {
        if (mata->projType == ProjType::Equal_Angle_Cone && mata->projUnit == ProjUnit::Second) {
            coordinates.x = mata->origin_x + coordinates.x / 3600;
            coordinates.y = mata->origin_y + coordinates.y / 3600;
            auxiliary.x = mata->origin_x + auxiliary.x / 3600;
            auxiliary.y = mata->origin_y + auxiliary.y / 3600;
        } else if (mata->projType == ProjType::Gauss) {
            Ellipsoid xian80 = Ellipsoid("ODATA Custom", mata->a, 0, 1.0 / mata->e, "ODATA Custom");
            GaussProjection gauss;
            gauss.ellipsoid = xian80;
            gauss.falseEasting = mata->gauss_number * 1000000 + 500000.0;
            gauss.falseNorthing = 0.0;
            gauss.latitudeOfNaturalOrigin = 0.0;
            gauss.longitudeOfNaturalOrigin = mata->lon_0;
            coordinates.x += mata->origin_x;
            coordinates.y += mata->origin_y;
            auxiliary.x += mata->origin_x;
            auxiliary.y += mata->origin_y;
            PlaneCoordinate plane = PlaneCoordinate(coordinates.x, coordinates.y);
            GeodeticCoordinate coord = gauss.inverse(plane);
            coordinates.x = coord.lng;
            coordinates.y = coord.lat;
        }
    } else if (mata->version == OdataVersion::ODATA_2_0) {

    } else if (mata->version == OdataVersion::ENTITY_1_0) {

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
                + QString::number(point.x, 'g', 16)
                + " "
                + QString::number(point.y, 'g', 16);
        if (i < count - 1) {
            wkt += ",";
        }
    }

    wkt += ")";
    return wkt;
}

void ODataLine::unprojtion(MapMataData *mata) {
    if (mata->version == OdataVersion::ODATA_1_0) {
        if (mata->projType == ProjType::Equal_Angle_Cone && mata->projUnit == ProjUnit::Second) {
            int count = coordinates.size();
            for (int i = 0; i < count; i++) {
                coordinates[i].x = mata->origin_x + (double)(coordinates[i].x / 3600);
                coordinates[i].y = mata->origin_y + (double)(coordinates[i].y / 3600);
            }
        } else if (mata->projType == ProjType::Gauss) {
            Ellipsoid xian80 = Ellipsoid("ODATA Custom", mata->a, 0, 1.0 / mata->e, "ODATA Custom");
            GaussProjection gauss;
            gauss.ellipsoid = xian80;
            gauss.falseEasting = mata->gauss_number * 1000000 + 500000.0;
            gauss.falseNorthing = 0.0;
            gauss.latitudeOfNaturalOrigin = 0.0;
            gauss.longitudeOfNaturalOrigin = mata->lon_0;
            int count = coordinates.size();
            for (int i = 0; i < count; i++) {
                coordinates[i].x += mata->origin_x;
                coordinates[i].y += mata->origin_y;
                PlaneCoordinate plane = PlaneCoordinate(coordinates[i].x, coordinates[i].y);
                GeodeticCoordinate coord = gauss.inverse(plane);
                // printf("lng lat x y  %f %f %f %f \r\n", coord.lng * 1000000, coord.lat * 1000000, plane.x, plane.y);
                coordinates[i].x  = coord.lng;
                coordinates[i].y= coord.lat;
            }
        }
    } else if (mata->version == OdataVersion::ODATA_2_0) {

    } else if (mata->version == OdataVersion::ENTITY_1_0) {

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
                    + QString::number(point.x, 'g', 16)
                    + " "
                    + QString::number(point.y, 'g', 16);
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

void ODataPolygon::unprojtion(MapMataData *mata) {
    if (mata->version == OdataVersion::ODATA_1_0) {
        if (mata->projType == ProjType::Equal_Angle_Cone && mata->projUnit == ProjUnit::Second) {
            int count = coordinates.size();
            for (int i = 0; i < count; i++) {
                int points = coordinates[i].size();
                for (int j = 0; j < points; j++) {
                    coordinates[i][j].x = mata->origin_x + (double)(coordinates[i][j].x / 3600);
                    coordinates[i][j].y = mata->origin_y + (double)(coordinates[i][j].y / 3600);
                }
            }
        } else if (mata->projType == ProjType::Gauss) {
            Ellipsoid xian80 = Ellipsoid("ODATA Custom", mata->a, 0, 1.0 / mata->e, "ODATA Custom");
            GaussProjection gauss;
            gauss.ellipsoid = xian80;
            gauss.falseEasting = mata->gauss_number * 1000000.0 + 500000.0;
            gauss.falseNorthing = 0.0;
            gauss.latitudeOfNaturalOrigin = 0.0;
            gauss.longitudeOfNaturalOrigin = mata->lon_0;

            int count = coordinates.size();
            for (int i = 0; i < count; i++) {
                int points = coordinates[i].size();
                for (int j = 0; j < points; j++) {
                    coordinates[i][j].x += mata->origin_x;
                    coordinates[i][j].y += mata->origin_y;
                    PlaneCoordinate plane = PlaneCoordinate(coordinates[i][j].x, coordinates[i][j].y);
                    GeodeticCoordinate coord = gauss.inverse(plane);
                    // printf("lng lat x y  %f %f %f %f \r\n", coord.lng * 1000000, coord.lat * 1000000, plane.x, plane.y);
                    coordinates[i][j].x  = coord.lng;
                    coordinates[i][j].y= coord.lat;
                }
            }
        }
    } else if (mata->version == OdataVersion::ODATA_2_0) {

    } else if (mata->version == OdataVersion::ENTITY_1_0) {

    }
}

void ODataGeometry::unprojection(MapMataData *mata){
    if (type == FeatureType::POINT) {
        point.unprojtion(mata);
    } else if (type == FeatureType::LINESTRING) {
        line.unprojtion(mata);
    } else if (type == FeatureType::POLYGON) {
        polygon.unprojtion(mata);
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
