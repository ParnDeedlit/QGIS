#include <QMessageBox>

#include "txtexporttopostgis.h"
#include "qgsabstractdatabaseproviderconnection.h"
#include "qgspostgresconn.h"

TxtExportToPostgis::TxtExportToPostgis() {

}

QgsPostgresConn* TxtExportToPostgis::setConnection(QgsDataSourceUri uri) {
    if ( ! _connection )
    {
      _connection = QgsPostgresConn::connectDb( uri, false, true, false );
      assert( _connection );
    }
    return _connection;
}

QgsPostgresConn* TxtExportToPostgis::getConnection() {
    if ( ! _connection )
    {
      return nullptr;
    }
    return _connection;
}

QString TxtExportToPostgis::checkUser() {
    QString user;
    QgsPostgresConn *conn = getConnection();
    const QString sql = QStringLiteral( "SELECT SESSION_USER, CURRENT_USER;" );
    if (conn == nullptr) return user;
    QgsPostgresResult result( conn->PQexec( sql ) );
    // current_user is the same as session_user
    // user = result.PQgetvalue( 0, 0 ) + result.PQgetvalue( 0, 1 );
    return user;
}
