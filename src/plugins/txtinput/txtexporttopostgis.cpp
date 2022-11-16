#include <QMessageBox>

#include "libpq-fe.h"

#include "txtexporttopostgis.h"
#include "qgsabstractdatabaseproviderconnection.h"
#include "qgspostgresconn.h"

TxtExportToPostgis::TxtExportToPostgis() {
    _connection = nullptr;
}

PGconn* TxtExportToPostgis::setConnection(QString host, QString port, QString database, QString username, QString password) {
    QString conninfo;
    conninfo = "hostaddr=" + host + " port = " + port + " user = " + username + " password = " + password + " dbname = " + database;
    _connection = PQconnectdb(conninfo.toStdString().c_str());

    /* Check to see that the backend connection was successfully made */
    if (PQstatus(_connection) != CONNECTION_OK)
    {
        _connection = nullptr;
        return nullptr;
    }
    return _connection;
}

PGconn* TxtExportToPostgis::getConnection() {
    if ( ! _connection )
    {
      return nullptr;
    }
    return _connection;
}

void TxtExportToPostgis::releaseConnection() {
    if (_connection )
    {
      PQfinish(_connection);
      _connection = nullptr;
    }
}

QString TxtExportToPostgis::checkUser() {
    QString user;
    PGresult *res;
    PGconn *conn = getConnection();
    const QString sql = QStringLiteral( "SELECT SESSION_USER, CURRENT_USER;" );
    if (conn == nullptr) return user;
    res = PQexec(conn, sql.toStdString().c_str());
    user = PQgetvalue(res, 0, 1);
    return user;
}

bool TxtExportToPostgis::createTable(QString table) {
    bool result = false;
    QString sqlres;
    PGresult *res;
    PGconn *conn = getConnection();

    const QString sql = "CREATE TABLE"
        + table
        + "(id serial primary key, name varchar(20),"
        + "geom geometry())";

    if (conn == nullptr) return result;
    res = PQexec(conn, sql.toStdString().c_str());
    sqlres = PQgetvalue(res, 0, 1);
    printf(sqlres.toStdString().c_str());

    return result;
}
