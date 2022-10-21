#ifndef ODATA_H
#define ODATA_H

#include <stdio.h>

#include <QString>
#include <QFileInfoList>

#include "odatadefine.h"
#include "odatalayer.h"

using namespace std;

class MapMataData {
public:
    QString name;
    File file;

    QString origin_bbox;
    QString lnglat_polygon;

    double a;
    double e;
    QString datum;
    QString proj;
    double lon_0;
    double lat_1;
    double lat_2;
    QString zone_type;
    int gauss_number;
    QString unit;
    double scale;
    double origin_x;
    double origin_y;
};

class Map {
public:
    MapMataData metadata;
    vector<FileGroup*> groups;
};

class ODATA {
private:
    static ODATA *instance;
    static vector<File> *metadata;

public:
    static ODATA* getInstance();
    static void initMetaData();

    vector<File>* getMetaData();

    QString getCodeByShortName(QString shortname);
    QString getCnnameByShortName(QString shortname);

public:
    Map* initMap(QFileInfoList list);
    vector<FileGroup*> checkValidGroup(QFileInfoList list);

    void parseMap(Map *map);
    bool parseMapMataData(Map *map);
    void parseFileGroups(Map *map);
    void parseFileGroup(FileGroup *group);
    void parseFileMetadata(File *meta);
    void parseFileAttrbution(File *attr);
    void parseFileGeometry(File *geom);

    vector<Layer*> getLayers(FileGroup group);
    void releaseLayers(vector<Layer*> layers);
private:
    void fixLayer(File *layer);
};

#endif // ODATA_H
