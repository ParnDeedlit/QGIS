/***************************************************************************
  qgsvectortilewriter.cpp
  --------------------------------------
  Date                 : April 2020
  Copyright            : (C) 2020 by Martin Dobias
  Email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsvectortilewriter.h"

#include "qgsdatasourceuri.h"
#include "qgsfeedback.h"
#include "qgsjsonutils.h"
#include "qgslogger.h"
#include "qgsmbtiles.h"
#include "qgstiles.h"
#include "qgsvectorlayer.h"
#include "qgsvectortilemvtencoder.h"
#include "qgsvectortileutils.h"
#include "qgsziputils.h"

#include <nlohmann/json.hpp>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QUrl>


QgsVectorTileWriter::QgsVectorTileWriter()
{
  setRootTileMatrix( QgsTileMatrix::fromWebMercator( 0 ) );
}


bool QgsVectorTileWriter::setRootTileMatrix( const QgsTileMatrix &tileMatrix )
{
  if ( tileMatrix.isRootTileMatrix() )
  {
    mRootTileMatrix = tileMatrix;
    return true;
  }
  return false;
}


bool QgsVectorTileWriter::writeTiles( QgsFeedback *feedback )
{
  if ( mMinZoom < 0 )
  {
    mErrorMessage = tr( "Invalid min. zoom level" );
    return false;
  }
  if ( mMaxZoom > 24 )
  {
    mErrorMessage = tr( "Invalid max. zoom level" );
    return false;
  }

  std::unique_ptr<QgsMbTiles> mbtiles;

  QgsDataSourceUri dsUri;
  dsUri.setEncodedUri( mDestinationUri );

  QString sourceType = dsUri.param( QStringLiteral( "type" ) );
  QString sourcePath = dsUri.param( QStringLiteral( "url" ) );

  QString replaceJson(sourcePath);
  QString replaceMeta(sourcePath);
  QString replaceStyle(sourcePath);

  //-------------------------------------------------------------------------------
  QgsCoordinateTransform ct( mRootTileMatrix.crs(), QgsCoordinateReferenceSystem( "EPSG:4326" ), mTransformContext );
  ct.setBallparkTransformsAreAppropriate( true );
  QgsRectangle tileboundsExtent = mExtent;
  QgsRectangle wgsExtent = ct.transform( tileboundsExtent );

  QRegularExpression re_mbtile("\.mbtiles");
  QString json = replaceJson.replace(re_mbtile, ".json");
  QFile jsonFile(json);
  jsonFile.open(QIODevice::ReadWrite);

  QString x = "{\r\n \"centerX\":" + QString::number(wgsExtent.center().x(), 'g', 12) + ",\r\n";
  jsonFile.write(x.toStdString().c_str());
  QString y = "\"centerY\":" + QString::number(wgsExtent.center().y(), 'g', 12) + ",\r\n";
  jsonFile.write(y.toStdString().c_str());
  QString ox = "\"originX\":" + QString::number(mExtent.center().x(), 'g', 12) + ",\r\n";
  jsonFile.write(ox.toStdString().c_str());
  QString oy = "\"originY\":" + QString::number(mExtent.center().y(), 'g', 12) + ",\r\n";
  jsonFile.write(oy.toStdString().c_str());
  QString min = "\"minZoom\":" + QString::number(mMinZoom) + ",\r\n";
  jsonFile.write(min.toStdString().c_str());
  QString max = "\"maxZoom\":" + QString::number(mMaxZoom) + ",\r\n";
  jsonFile.write(max.toStdString().c_str());
  QString init = "\"initZoom\":" + QString::number((mMinZoom + mMaxZoom) / 2, 'g', 12) + ",\r\n";
  jsonFile.write(init.toStdString().c_str());
  QString styleName("NewMap");
  QString layers = "\"layers\":[ \r\n";
  jsonFile.write(layers.toStdString().c_str());
  for (int i = 0; i < mLayers.size(); i++)
  {
    Layer layer = mLayers[i];
    QgsVectorLayer* vl = layer.layer();
    QString layerid = "{ \r\n \"id\": \" " + layer.layer()->name() + "\",\r\n";
    QString layermin = "\"minzoom\":" + QString::number(mMinZoom) + ",\r\n";
    QString layermax = "\"maxzoom\":" + QString::number(mMaxZoom) + ",\r\n";
    QString layersource = "\"source\": \"" + styleName + "\",  \r\n";
    QString layersourcelayer = "\"source-layer\": \"" + layer.layer()->name() + "\", \r\n";
    jsonFile.write(layerid.toStdString().c_str());
    jsonFile.write(layermin.toStdString().c_str());
    jsonFile.write(layermax.toStdString().c_str());
    jsonFile.write(layersource.toStdString().c_str());
    jsonFile.write(layersourcelayer.toStdString().c_str());
    QString layertype;
    if (vl->geometryType() == QgsWkbTypes::GeometryType::PointGeometry) {
      layertype = "\"type\": \"circle\", \r\n";
      layertype += "\"paint\": {\"circle-color\" : [\"#000000\",\"#FF0000\",\"#00FF00\",\"#0000FF\"] }\r\n";
    }
    else if (vl->geometryType() == QgsWkbTypes::GeometryType::LineGeometry) {
      layertype = "\"type\": \"line\", \r\n";
      layertype += "\"paint\": {\"line-color\" : [\"#000000\",\"#FF0000\",\"#00FF00\",\"#0000FF\"], \r\n \"line-width\" :2 }\r\n";
    }
    else if (vl->geometryType() == QgsWkbTypes::GeometryType::PolygonGeometry) {
      layertype = "\"type\": \"fill\", \r\n";
      layertype += "\"paint\": {\"fill-color\" : [\"#000000\",\"#FF0000\",\"#00FF00\",\"#0000FF\"] }\r\n";
    }
    jsonFile.write(layertype.toStdString().c_str());
    if (i < mLayers.size() - 1) {
      jsonFile.write("}, \r\n");
    }
    else {
      jsonFile.write("} \r\n");
    }
  }
  jsonFile.write("] \r\n");
  jsonFile.write("} \r\n");
  jsonFile.close();
  //-------------------------------------------------------------------------------
  QString meta = replaceMeta.replace(re_mbtile, "_metadata.json");
  QFile meatFile(meta);
  meatFile.open(QIODevice::ReadWrite);
  QString proj = "{\r\n \"projType\": 0 \r\n}";
  meatFile.write(proj.toStdString().c_str());
  meatFile.close();
  //-------------------------------------------------------------------------------
  QString style = replaceStyle.replace(re_mbtile, "_style.json");
  QRegularExpression re_mapname("([\\w\u4E00-\u9FA5A-Za-z0-9_]+)\.mbtiles");
  QRegularExpressionMatch match_a = re_mapname.match(dsUri.param( QStringLiteral( "url" ) ));
  QString mapName("NewMap");
  if(match_a.hasMatch() && match_a.capturedLength() >= 1) {
      mapName = match_a.captured(1);
  }

  QFile styleFile(style);
  styleFile.open(QIODevice::ReadWrite);
  QString version = "{\r\n \"version\": 8, \r\n";
  styleFile.write(version.toStdString().c_str());
  QString glyphs = "\"glyphs\": \"http://localhost:6163/igs/rest/mrms/vtiles/fonts/{fontstack}/{range}.pbf\", \r\n";
  styleFile.write(glyphs.toStdString().c_str());
  QString sprite = "\"sprite\": \"http://localhost:6163/igs/rest/mrms/vtiles/sprite\", \r\n";
  styleFile.write(sprite.toStdString().c_str());
  QString id = "\"id\": \""+ mapName +"\", \r\n";
  styleFile.write(id.toStdString().c_str());
  QString name = "\"name\": \""+ mapName +"\", \r\n";
  styleFile.write(name.toStdString().c_str());

  QString sources = "\"sources\": \r\n {";
  styleFile.write(sources.toStdString().c_str());
  QString mapsource = "\"" +mapName + "\": "+ "{ \r\n \"type\": \"vector\", \r\n";
  styleFile.write(mapsource.toStdString().c_str());
  QString bound = "\"bound\": ["+ QString::number(wgsExtent.xMinimum(), 'g', 12)
          + "," + QString::number(wgsExtent.yMinimum(), 'g', 12)
          + "," + QString::number(wgsExtent.xMaximum(), 'g', 12)
          + "," + QString::number(wgsExtent.yMaximum(), 'g', 12)
          +"], \r\n";
  styleFile.write(bound.toStdString().c_str());
  QString minZoom = "\"minZoom\": "+  QString::number(mMinZoom) +", \r\n";
  styleFile.write(minZoom.toStdString().c_str());
  QString maxZoom = "\"maxZoom\": "+  QString::number(mMaxZoom) +", \r\n";
  styleFile.write(maxZoom.toStdString().c_str());
  QString tiles = "\"tiles\": [\"http://localhost:6163/igs/rest/mrms/tile/"+ mapName +"/{z}/{y}/{x}?type=pbf\"] \r\n } \r\n";
  styleFile.write(tiles.toStdString().c_str());
  styleFile.write("},\r\n ");
  styleFile.write("\"layers\":[");
  //for ( const Layer &layer : std::as_const( mLayers ) )
  for (int i = 0; i < mLayers.size(); i++)
  {
    Layer layer = mLayers[i];
    QgsVectorLayer *vl = layer.layer();
    QString layerid = "{ \r\n \"id\": \" " + layer.layer()->name() + "\",\r\n";
    QString layermin = "\"minzoom\":" + QString::number(mMinZoom) + ",\r\n";
    QString layermax = "\"maxzoom\":" + QString::number(mMaxZoom) + ",\r\n";
    QString layersource = "\"source\": \"" + mapName + "\",  \r\n";
    QString layersourcelayer = "\"source-layer\": \"" + layer.layer()->name() + "\", \r\n";
    styleFile.write(layerid.toStdString().c_str());
    styleFile.write(layermin.toStdString().c_str());
    styleFile.write(layermax.toStdString().c_str());
    styleFile.write(layersource.toStdString().c_str());
    styleFile.write(layersourcelayer.toStdString().c_str());
    QString layertype;
    if (vl->geometryType() == QgsWkbTypes::GeometryType::PointGeometry) {
        layertype = "\"type\": \"circle\" \r\n";
    } else if (vl->geometryType() == QgsWkbTypes::GeometryType::LineGeometry) {
        layertype = "\"type\": \"line\" \r\n";
    } else if (vl->geometryType() == QgsWkbTypes::GeometryType::PolygonGeometry) {
        layertype = "\"type\": \"fill\" \r\n";
    }
    styleFile.write(layertype.toStdString().c_str());
    if (i < mLayers.size() - 1) {
        styleFile.write("}, \r\n");
    } else {
        styleFile.write("} \r\n");
    }
  }
  styleFile.write("]");
  styleFile.write("\r\n }");
  styleFile.close();
  //-------------------------------------------------------------------------------

  if ( sourceType == QLatin1String( "xyz" ) )
  {
    // remove the initial file:// scheme
    sourcePath = QUrl( sourcePath ).toLocalFile();

    if ( !QgsVectorTileUtils::checkXYZUrlTemplate( sourcePath ) )
    {
      mErrorMessage = tr( "Invalid template for XYZ: " ) + sourcePath;
      return false;
    }
  }
  else if ( sourceType == QLatin1String( "mbtiles" ) )
  {
    mbtiles.reset( new QgsMbTiles( sourcePath ) );
  }
  else
  {
    mErrorMessage = tr( "Unsupported source type for writing: " ) + sourceType;
    return false;
  }

  QgsRectangle outputExtent = mExtent;
  if ( outputExtent.isEmpty() )
  {
    outputExtent = fullExtent();
    if ( outputExtent.isEmpty() )
    {
      mErrorMessage = tr( "Failed to calculate output extent" );
      return false;
    }
  }

  // figure out how many tiles we will need to do
  int tilesToCreate = 0;
  for ( int zoomLevel = mMinZoom; zoomLevel <= mMaxZoom; ++zoomLevel )
  {
    const QgsTileMatrix tileMatrix = QgsTileMatrix::fromTileMatrix( zoomLevel, mRootTileMatrix );

    QgsTileRange tileRange = tileMatrix.tileRangeFromExtent( outputExtent );
    tilesToCreate += ( tileRange.endRow() - tileRange.startRow() + 1 ) *
                     ( tileRange.endColumn() - tileRange.startColumn() + 1 );
  }

  if ( tilesToCreate == 0 )
  {
    mErrorMessage = tr( "No tiles to generate" );
    return false;
  }

  if ( mbtiles )
  {
    if ( !mbtiles->create() )
    {
      mErrorMessage = tr( "Failed to create MBTiles file: " ) + sourcePath;
      return false;
    }

    // required metadata
    mbtiles->setMetadataValue( "format", "pbf" );
    mbtiles->setMetadataValue( "json", mbtilesJsonSchema() );

    // metadata specified by the client
    const QStringList metaKeys = mMetadata.keys();
    for ( const QString &key : metaKeys )
    {
      mbtiles->setMetadataValue( key, mMetadata[key].toString() );
    }

    // default metadata that we always write (if not written by the client)
    if ( !mMetadata.contains( "name" ) )
      mbtiles->setMetadataValue( "name",  "unnamed" );  // required by the spec
    if ( !mMetadata.contains( "minzoom" ) )
      mbtiles->setMetadataValue( "minzoom", QString::number( mMinZoom ) );
    if ( !mMetadata.contains( "maxzoom" ) )
      mbtiles->setMetadataValue( "maxzoom", QString::number( mMaxZoom ) );
    if ( !mMetadata.contains( "bounds" ) )
    {
      try
      {
        QgsCoordinateTransform ct( mRootTileMatrix.crs(), QgsCoordinateReferenceSystem( "EPSG:4326" ), mTransformContext );
        ct.setBallparkTransformsAreAppropriate( true );
        QgsRectangle wgsExtent = ct.transform( outputExtent );
        QString boundsStr = QString( "%1,%2,%3,%4" )
                            .arg( wgsExtent.xMinimum() ).arg( wgsExtent.yMinimum() )
                            .arg( wgsExtent.xMaximum() ).arg( wgsExtent.yMaximum() );
        mbtiles->setMetadataValue( "bounds", boundsStr );
      }
      catch ( const QgsCsException & )
      {
        // bounds won't be written (not a problem - it is an optional value)
      }
    }
    if ( !mMetadata.contains( "crs" ) )
      mbtiles->setMetadataValue( "crs",  mRootTileMatrix.crs().authid() );
  }

  int tilesCreated = 0;
  for ( int zoomLevel = mMinZoom; zoomLevel <= mMaxZoom; ++zoomLevel )
  {
    const QgsTileMatrix tileMatrix = QgsTileMatrix::fromTileMatrix( zoomLevel, mRootTileMatrix );

    QgsTileRange tileRange = tileMatrix.tileRangeFromExtent( outputExtent );
    for ( int row = tileRange.startRow(); row <= tileRange.endRow(); ++row )
    {
      for ( int col = tileRange.startColumn(); col <= tileRange.endColumn(); ++col )
      {
        QgsTileXYZ tileID( col, row, zoomLevel );
        QgsVectorTileMVTEncoder encoder( tileID );
        encoder.setTransformContext( mTransformContext );

        for ( const Layer &layer : std::as_const( mLayers ) )
        {
          if ( ( layer.minZoom() >= 0 && zoomLevel < layer.minZoom() ) ||
               ( layer.maxZoom() >= 0 && zoomLevel > layer.maxZoom() ) )
            continue;

          encoder.addLayer( layer.layer(), feedback, layer.filterExpression(), layer.layerName() );
        }

        if ( feedback && feedback->isCanceled() )
        {
          mErrorMessage = tr( "Operation has been canceled" );
          return false;
        }

        QByteArray tileData = encoder.encode();

        ++tilesCreated;
        if ( feedback )
        {
          feedback->setProgress( static_cast<double>( tilesCreated ) / tilesToCreate * 100 );
        }

        if ( tileData.isEmpty() )
        {
          // skipping empty tile - no need to write it
          continue;
        }

        if ( sourceType == QLatin1String( "xyz" ) )
        {
          if ( !writeTileFileXYZ( sourcePath, tileID, tileMatrix, tileData ) )
            return false;  // error message already set
        }
        else  // mbtiles
        {
          QByteArray gzipTileData;
          QgsZipUtils::encodeGzip( tileData, gzipTileData );
          int rowTMS = pow( 2, tileID.zoomLevel() ) - tileID.row() - 1;
          mbtiles->setTileData( tileID.zoomLevel(), tileID.column(), rowTMS, gzipTileData );
        }
      }
    }
  }

  return true;
}

QgsRectangle QgsVectorTileWriter::fullExtent() const
{
  QgsRectangle extent;

  for ( const Layer &layer : mLayers )
  {
    QgsVectorLayer *vl = layer.layer();
    QgsCoordinateTransform ct( vl->crs(), mRootTileMatrix.crs(), mTransformContext );
    ct.setBallparkTransformsAreAppropriate( true );
    try
    {
      QgsRectangle r = ct.transformBoundingBox( vl->extent() );
      extent.combineExtentWith( r );
    }
    catch ( const QgsCsException & )
    {
      QgsDebugMsg( "Failed to reproject layer extent to destination CRS" );
    }
  }
  return extent;
}

bool QgsVectorTileWriter::writeTileFileXYZ( const QString &sourcePath, QgsTileXYZ tileID, const QgsTileMatrix &tileMatrix, const QByteArray &tileData )
{
  QString filePath = QgsVectorTileUtils::formatXYZUrlTemplate( sourcePath, tileID, tileMatrix );

  // make dirs if needed
  QFileInfo fi( filePath );
  QDir fileDir = fi.dir();
  if ( !fileDir.exists() )
  {
    if ( !fileDir.mkpath( "." ) )
    {
      mErrorMessage = tr( "Cannot create directory " ) + fileDir.path();
      return false;
    }
  }

  QFile f( filePath );
  if ( !f.open( QIODevice::WriteOnly ) )
  {
    mErrorMessage = tr( "Cannot open file for writing " ) + filePath;
    return false;
  }

  f.write( tileData );
  f.close();
  return true;
}


QString QgsVectorTileWriter::mbtilesJsonSchema()
{
  QVariantList arrayLayers;
  for ( const Layer &layer : std::as_const( mLayers ) )
  {
    QgsVectorLayer *vl = layer.layer();
    const QgsFields fields = vl->fields();

    QVariantMap fieldsObj;
    for ( const QgsField &field : fields )
    {
      QString fieldTypeStr;
      if ( field.type() == QVariant::Bool )
        fieldTypeStr = QStringLiteral( "Boolean" );
      else if ( field.type() == QVariant::Int || field.type() == QVariant::Double )
        fieldTypeStr = QStringLiteral( "Number" );
      else
        fieldTypeStr = QStringLiteral( "String" );

      fieldsObj[field.name()] = fieldTypeStr;
    }

    QVariantMap layerObj;
    layerObj["id"] = vl->name();
    layerObj["fields"] = fieldsObj;
    arrayLayers.append( layerObj );
  }

  QVariantMap rootObj;
  rootObj["vector_layers"] = arrayLayers;
  return QString::fromStdString( QgsJsonUtils::jsonFromVariant( rootObj ).dump() );
}


QByteArray QgsVectorTileWriter::writeSingleTile( QgsTileXYZ tileID, QgsFeedback *feedback, int buffer, int resolution ) const
{
  int zoomLevel = tileID.zoomLevel();

  QgsVectorTileMVTEncoder encoder( tileID );
  encoder.setTileBuffer( buffer );
  encoder.setResolution( resolution );
  encoder.setTransformContext( mTransformContext );

  for ( const QgsVectorTileWriter::Layer &layer : std::as_const( mLayers ) )
  {
    if ( ( layer.minZoom() >= 0 && zoomLevel < layer.minZoom() ) ||
         ( layer.maxZoom() >= 0 && zoomLevel > layer.maxZoom() ) )
      continue;

    encoder.addLayer( layer.layer(), feedback, layer.filterExpression(), layer.layerName() );
  }

  return encoder.encode();
}

//bool QgsVectorTileWriter::writeJson() {
//    // printf("mapgis json %s \r\n", mDestinationUri.toStdString().c_str());
//    QRegularExpression re_a(".mbtiles");
//    QString json = mDestinationUri.replace(re_a, ".json");
//    QFile jsonFile(json);
//    jsonFile.open(QIODevice::ReadWrite);
//    jsonFile.write("test");
//}
