#include "geopackage.h"
#include <spatialite/gaiageo.h>
#include <spatialite.h>

GeoPackageHelper::GeoPackageHelper() {
    _connection = nullptr;
}

sqlite3* GeoPackageHelper::setConnection(QString uri) {
    int nResult = sqlite3_open_v2(uri.toStdString().c_str(), &_connection, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (nResult != SQLITE_OK)
    {
        _connection = nullptr;
        return nullptr;
    }
    return _connection;
}

sqlite3* GeoPackageHelper::getConnection() {
    if ( ! _connection )
    {
      return nullptr;
    }
    return _connection;
}

void GeoPackageHelper::releaseConnection() {
    if (_connection )
    {
      sqlite3_close(_connection);
      _connection = nullptr;
    }
}

/**
 * @brief GeoPackageHelper::create_gpkg
 * @param uri
 * http://www.geopackage.org/guidance/getting-started.html#creating-a-geopackage
 */
void GeoPackageHelper::create_gpkg(QString uri) {
    void *cache;
    int nResult = sqlite3_open_v2 (uri.toStdString().c_str(), &_connection,
                               SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (nResult != SQLITE_OK)
    {
        sqlite3_close(_connection);
        _connection = nullptr;
    } else {
        char *err_msg = nullptr;
        QString app("PRAGMA application_id = 0x47504B47");
        QString version("PRAGMA user_version = 0x000027D8");
        sqlite3_exec(_connection, app.toStdString().c_str(), 0, 0, &err_msg);
        sqlite3_exec(_connection, version.toStdString().c_str(), 0, 0, &err_msg);
        cache = spatialite_alloc_connection ();
        spatialite_init_ex (_connection, cache, 0);
        QString init("SELECT InitSpatialMetadata(1)");
        nResult = sqlite3_exec (_connection, init.toStdString().c_str(), NULL, NULL, &err_msg);
        if (nResult != SQLITE_OK)
          {
              /* an error occurred */
              printf ("InitSpatialMetadata() error: %s\n", err_msg);
              sqlite3_free (err_msg);
          }
    }
}

void GeoPackageHelper::create_gpkg_contents(QString sql) {
    QString createdb("CREATE TABLE IF NOT EXISTS gpkg_contents (table_name TEXT NOT NULL PRIMARY KEY,data_type TEXT NOT NULL,identifier TEXT UNIQUE,description TEXT DEFAULT '',last_change DATETIME NOT NULL DEFAULT (strftime('%Y-%m-%dT%H:%M:%fZ','now')),min_x DOUBLE, min_y DOUBLE,max_x DOUBLE, max_y DOUBLE,srs_id INTEGER,CONSTRAINT fk_gc_r_srs_id FOREIGN KEY (srs_id) REFERENCES gpkg_spatial_ref_sys(srs_id))");
    char *err_msg = nullptr;
    sqlite3* sqlite = getConnection();
    int rtn = sqlite3_exec(sqlite, createdb.toStdString().c_str(), 0, 0, &err_msg);
    sqlite3_free (err_msg);
}

void GeoPackageHelper::create_gpkg_spatial_ref_sys(QString sql) {
    QString createdb("CREATE TABLE IF NOT EXISTS gpkg_spatial_ref_sys (srs_name TEXT NOT NULL,srs_id INTEGER NOT NULL PRIMARY KEY,organization TEXT NOT NULL,organization_coordsys_id INTEGER NOT NULL,definition TEXT NOT NULL,description TEXT)");
    char *err_msg = nullptr;
    sqlite3* sqlite = getConnection();
    int rtn = sqlite3_exec(sqlite, createdb.toStdString().c_str(), 0, 0, &err_msg);
    //if (rtn == SQLITE_OK) {
        QString insert_4326("INSERT INTO gpkg_spatial_ref_sys (srs_name, srs_id, organization, organization_coordsys_id, definition, description) ");
        QString values("VALUES(");
        QString v1("'WGS 84 geodetic', 4326, 'EPSG', 4326, ");
        QString v2("\"GEOGCS['WGS 84',DATUM['WGS_1984',SPHEROID['WGS 84',6378137,298.257223563,AUTHORITY['EPSG','7030']],AUTHORITY['EPSG','6326']],PRIMEM['Greenwich',0,AUTHORITY['EPSG','8901']],UNIT['degree',0.0174532925199433,AUTHORITY['EPSG','9122']],AXIS['Latitude',NORTH],AXIS['Longitude',EAST],AUTHORITY['EPSG','4326']]\",");
        QString v3("'longitude/latitude coordinates in decimal degrees on the WGS 84 spheroid');");

        QString e1("'Undefined cartesian SRS', -1, 'NONE', -1, ");
        QString e2("'undefined',");
        QString e3("'undefined cartesian coordinate reference system');");

        QString u1("'Undefined geographic SRS', 0, 'NONE', 0, ");
        QString u2("'undefined',");
        QString u3("'undefined geographic coordinate reference system');");

        QString crs4326 = insert_4326 + values + v1 + v2 + v3;
        QString crs0 = insert_4326 + values + e1 + e2 + e3;
        QString crs_1 = insert_4326 + values + u1 + u2 + u3;

        sqlite3_exec(sqlite, crs0.toStdString().c_str(), 0, 0, &err_msg);
        sqlite3_exec(sqlite, crs_1.toStdString().c_str(), 0, 0, &err_msg);
        sqlite3_exec(sqlite, crs4326.toStdString().c_str(), 0, 0, &err_msg);

        sqlite3_free (err_msg);
}

void GeoPackageHelper::create_gpkg_geometry_columns(QString sql) {
    QString cols("CREATE TABLE gpkg_geometry_columns (table_name TEXT NOT NULL,column_name TEXT NOT NULL,geometry_type_name TEXT NOT NULL,srs_id INTEGER NOT NULL,z TINYINT NOT NULL,m TINYINT NOT NULL,CONSTRAINT pk_geom_cols PRIMARY KEY (table_name, column_name),CONSTRAINT uk_gc_table_name UNIQUE (table_name),CONSTRAINT fk_gc_tn FOREIGN KEY (table_name) REFERENCES gpkg_contents(table_name),CONSTRAINT fk_gc_srs FOREIGN KEY (srs_id) REFERENCES gpkg_spatial_ref_sys (srs_id))");
    char *err_msg = nullptr;
    sqlite3* sqlite = getConnection();
    sqlite3_exec(sqlite, cols.toStdString().c_str(), 0, 0, &err_msg);
    sqlite3_free (err_msg);
}


void GeoPackageHelper::create_gpkg_feature_table(Layer layer) {

}

void GeoPackageHelper::insert_gpkg_contents(Layer layer) {

}
