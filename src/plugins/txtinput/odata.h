#ifndef ODATA_H
#define ODATA_H

#include <stdio.h>

#include <QString>
#include <QFileInfoList>
#include <QList>

#include "odatadefine.h"
#include "odatalayer.h"

using namespace std;

class ODATA {
public:
    OdataVersion version;

    QString getCodeByShortName(QString shortname);
    QString getCnnameByShortName(QString shortname);

public:
    Map* initMap(QFileInfoList list);
    QList<FileGroup*> checkValidGroup(QFileInfoList list);

    void parseMap(Map *map);
    bool parseMapMataData(Map *map);
    void parseFileGroups(Map *map);
    void parseFileGroup(FileGroup *group);
    void parseFileMetadata(File *meta);
    void parseFileAttrbution(File *attr);
    void parseFileGeometry(File *geom);

    QList<Layer*> getLayers(FileGroup group);
    void releaseLayers(QList<Layer*> layers);
private:
    void fixLayer(File *layer);
};

#endif // ODATA_H
