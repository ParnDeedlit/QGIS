#ifndef ODATA_LAYER_H
#define ODATA_LAYER_H
#include <stdio.h>
#include "qgspostgresconn.h"

#include <QString>

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
    void writeToPostgis(PGconn *conn);

    Layer* readFromFile();

    Layer* readFromMsFile();

    Layer* readFromSxFile();
    Layer* readFromSxFilePoint();
    Layer* readFromSxFileLine();
    Layer* readFromSxFileArea();

    Layer* readFromZbFile();
    Layer* readFromZbFilePoint();
    Layer* readFromZbFileLine();
    Layer* readFromZbFileArea();

private:
    Layer* parseField();
    Layer* parseUrl();
    QString fieldsToColumn();
    QString typeToColumn();

};

#endif // ODATA_LAYER_H
