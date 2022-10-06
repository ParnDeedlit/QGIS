#ifndef TXTEXPORTTOPOSTGIS_H
#define TXTEXPORTTOPOSTGIS_H

#include "qgsabstractdatabaseproviderconnection.h"
#include "qgspostgresconn.h"

class TxtExportToPostgis {
public:
    TxtExportToPostgis();
    ~TxtExportToPostgis();

    QgsPostgresConn* setConnection(QgsDataSourceUri uri);
    QgsPostgresConn* getConnection();

    QString checkUser();

private:
  QgsPostgresConn *_connection;
};

#endif // TXTEXPORTTOPOSTGIS_H

