#ifndef GEOPACKAGE_H
#define GEOPACKAGE_H

#include <QString>
#include <sqlite3.h>

#include "odatalayer.h"

class GeoPackageHelper {
public:
    GeoPackageHelper();
    ~GeoPackageHelper();

    sqlite3* setConnection(QString uri);
    sqlite3* getConnection();
    void releaseConnection();

    void create_gpkg(QString uri);
    void create_gpkg_contents(QString sql = "");
    void create_gpkg_spatial_ref_sys(QString sql = "");
    void create_gpkg_feature_table(Layer layer);
    void create_gpkg_geometry_columns(QString sql = "");

    void insert_gpkg_contents(Layer layer);

private:
    sqlite3* _connection;
};

#endif // GEOPACKAGE_H
