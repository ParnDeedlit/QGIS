#ifndef TXTEXPORTTOPOSTGIS_H
#define TXTEXPORTTOPOSTGIS_H
#include <QString>

#include "libpq-fe.h"

//#include "qgsabstractdatabaseproviderconnection.h"
//#include "qgspostgresconn.h"

class TxtExportToPostgis {
public:
    TxtExportToPostgis();
    ~TxtExportToPostgis();

    PGconn* setConnection(QString host, QString port, QString database, QString username, QString password);
    PGconn* getConnection();
    void releaseConnection();

    bool createTable(QString table);
    // bool insertItem(QString table);

    QString checkUser();

private:
    PGconn *_connection;
};

#endif // TXTEXPORTTOPOSTGIS_H

