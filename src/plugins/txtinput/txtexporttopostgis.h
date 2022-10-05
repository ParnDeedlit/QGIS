#ifndef TXTEXPORTTOPOSTGIS_H
#define TXTEXPORTTOPOSTGIS_H

#include "qgsabstractdatabaseproviderconnection.h"
#include "qgspostgresconn.h"

class TxtExportToPostgis {
    TxtExportToPostgis();
    ~TxtExportToPostgis();

private:
  // std::unique_ptr<QgsPostgresConn> postgisConn;
  std::unique_ptr<QgsAbstractDatabaseProviderConnection> mConn;
  const QString mProviderName = QStringLiteral( "postgres" );
  QString mSchema;
  QString mTable = QString();
  QString mPrimaryKeyField = QStringLiteral( "id" );
  QString mGeomColumn;
  QString mEncoding = QStringLiteral( "UTF-8" );
  bool mCreateIndex = true;
  bool mOverwrite = true;
  QMap< QString, QVariant > mOptions;
};

#endif // TXTEXPORTTOPOSTGIS_H

