#ifndef ODATA_LAYER_H
#define ODATA_LAYER_H
#include <stdio.h>
#include "qgspostgresconn.h"

#include <QString>
#include <sqlite3.h>

#include "odatadefine.h"
#include "odatafeature.h"
#include "odataattributes.h"

using namespace std;

class Layer {
public:
    // map metadata
    MapMataData mapMetadata;

    // file
    QString uri;
    FileGroup filegroup;

    // layer
    LayerType type;
    vector<ODataAttributeMeta> fields;
    vector<ODataFeature> features;

    Layer* setMapMetadata(MapMataData meta);
    Layer* setFileGroup(FileGroup group);
    Layer* setLayerType(LayerType t);
    Layer* update();

    void excutePostgis(PGconn *conn);
    void deletePostgisHasExist(PGconn *conn);
    void checkPostgisHasExist(PGconn *conn);
    void writeToPostgis(PGconn *conn, int batchcount = 10);
    void writeToGeojson(QString uri, int batchcount = 10);

    void excuteGeopackage(sqlite3 *conn);
    void deleteGeopackageHasExist(sqlite3 *conn);
    void checkGeopackageHasExist(sqlite3 *conn);
    void writeToGeopackage(sqlite3 *conn, int batchcount = 10);

    Layer* readFromFile();

    Layer* readFromMsFile();

    Layer* readFromSxFile();
    Layer* readFromSxFilePoint();
    Layer* readFromSxFileLine();
    Layer* readFromSxFileArea();
    Layer* readFromSxFileAnno();

    Layer* readFromZbFile();
    Layer* readFromZbFilePoint();
    Layer* readFromZbFileLine();
    Layer* readFromZbFileArea();
    Layer* readFromZbFileAnno();

private:
    Layer* parseField();
    Layer* parseUrl();
    QString fieldsToColumn();
    QString fieldsToColumnBySqlite();
    QString typeToColumn();

};

#endif // ODATA_LAYER_H
