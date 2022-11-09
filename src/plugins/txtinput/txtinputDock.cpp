#include <QMessageBox>
#include <QFileDialog>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QFileInfoList>

#include "txtinputDock.h"
#include "txtreader.h"
#include "odata.h"
#include "odatalayer.h"

#include "gauss.h"
#include "coordnate.h"
#include "ellipsoid.h"

#include "qgisinterface.h"

txtinputDock::txtinputDock(QgisInterface *qIface, QWidget *parent)
    :QgsDockWidget(parent), Ui::txtinputDock()
{
    setupUi( this );

    // hide the fix-related stuff, needs more work
    qgsInterface = qIface;
    txtExport = new TxtExportToPostgis();
}

txtinputDock::~txtinputDock()
{

}

PGconn* txtinputDock::tryConnectPostgis() {
    if (txtExport->getConnection() == nullptr) {
        host = hostEdit->text();
        port = QString::number(portBox->value());
        database = databaseEdit->text();
        username = userEdit->text();
        password = passwordEdit->text();
        txtExport->setConnection(host, port, database, username, password);
    }
    return txtExport->getConnection();
}

void txtinputDock::on_selectDirctionButton_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this, "选择文件夹");
    directoryEdit->setText(dir);
    TxtReader reader;

    std::vector<DirInfo> dirinfos = reader.loopRootDir(dir);
    QStandardItemModel *tablemodel = new QStandardItemModel();
    tablemodel->setHorizontalHeaderItem(0, new QStandardItem("名称"));
    tablemodel->setHorizontalHeaderItem(1, new QStandardItem("路径"));
    tablemodel->setHorizontalHeaderItem(2, new QStandardItem("图层数量"));

    QStandardItemModel *metamodel = new QStandardItemModel();
    metamodel->setHorizontalHeaderItem(0, new QStandardItem("名称"));
    metamodel->setHorizontalHeaderItem(1, new QStandardItem("椭球长半径"));
    metamodel->setHorizontalHeaderItem(2, new QStandardItem("椭球扁率"));
    metamodel->setHorizontalHeaderItem(3, new QStandardItem("大地基准"));
    metamodel->setHorizontalHeaderItem(4, new QStandardItem("地图投影"));
    metamodel->setHorizontalHeaderItem(5, new QStandardItem("中央经线"));
    metamodel->setHorizontalHeaderItem(6, new QStandardItem("标准纬线1"));
    metamodel->setHorizontalHeaderItem(7, new QStandardItem("标准纬线2"));
    metamodel->setHorizontalHeaderItem(8, new QStandardItem("分带方式"));
    metamodel->setHorizontalHeaderItem(9, new QStandardItem("高斯投影带号"));
    metamodel->setHorizontalHeaderItem(10, new QStandardItem("坐标单位"));
    metamodel->setHorizontalHeaderItem(11, new QStandardItem("坐标放大系数"));
    metamodel->setHorizontalHeaderItem(12, new QStandardItem("相对原点横坐标"));
    metamodel->setHorizontalHeaderItem(13, new QStandardItem("相对原点纵坐标"));

    int dircount = dirinfos.size();
    for(int i = 0; i < dircount; i++) {
        //-------------------------------layers-------------------------------------
        tablemodel->setItem(i, 0, new QStandardItem(dirinfos[i].name));
        tablemodel->setItem(i, 1, new QStandardItem(dirinfos[i].path));
        QFileInfoList nodefiles = reader.readNodeDir(dirinfos[i].path);
        ODATA odata;
        Map* map = odata.initMap(nodefiles);
        odata.parseMap(map);
        std::vector<FileGroup*> valids =  map->groups;
        int layercount = valids.size();
        QString count = QString::number(layercount);
        tablemodel->setItem(i, 2, new QStandardItem(count));
        //-------------------------------Metadata-------------------------------------
        metamodel->setItem(i, 0, new QStandardItem(dirinfos[i].name));
        metamodel->setItem(i, 1, new QStandardItem(QString::number(map->metadata.a)));
        metamodel->setItem(i, 2, new QStandardItem(QString::number(map->metadata.e)));
        metamodel->setItem(i, 3, new QStandardItem(map->metadata.datum));
        metamodel->setItem(i, 4, new QStandardItem(projTypeToProjTypeName(map->metadata.projType)));
        metamodel->setItem(i, 5, new QStandardItem(QString::number(map->metadata.lon_0)));
        metamodel->setItem(i, 6, new QStandardItem(QString::number(map->metadata.lat_1)));
        metamodel->setItem(i, 7, new QStandardItem(QString::number(map->metadata.lat_2)));
        metamodel->setItem(i, 8, new QStandardItem(map->metadata.zone_type));
        metamodel->setItem(i, 9, new QStandardItem(QString::number(map->metadata.gauss_number)));
        metamodel->setItem(i, 10, new QStandardItem(projUnitToProjUnitName(map->metadata.projUnit)));
        metamodel->setItem(i, 11, new QStandardItem(QString::number(map->metadata.scale)));
        metamodel->setItem(i, 12, new QStandardItem(QString::number(map->metadata.origin_x)));
        metamodel->setItem(i, 13, new QStandardItem(QString::number(map->metadata.origin_y)));
        maps.push_back(map);
    }

    tablemodel->setRowCount(dirinfos.size());
    metamodel->setRowCount(dirinfos.size());
    layersTableView->setModel(tablemodel);
    metadataView->setModel(metamodel);
}

void txtinputDock::on_createIndexButton_clicked() {
}

void txtinputDock::on_deleteButton_clicked() {
    PGconn *conn = tryConnectPostgis();

    int map_count = maps.size();
    for (int index = 0; index < map_count; index++) {
        Map* map = maps[index];
        int count = map->groups.size();
        for (int i = 0; i < count; i++) {
            FileGroup *filegroup = map->groups[i];

            Layer *layer_point = new Layer;
            Layer *layer_line = new Layer;
            Layer *layer_area = new Layer;
            Layer *layer_anno = new Layer;
            QString m = filegroup->matadata.layername;
            layer_point->setFileGroup(*filegroup)
                    ->setLayerType(LayerType::Point)
                    ->update()
                    ->deletePostgisHasExist(conn);
            layer_line->setFileGroup(*filegroup)
                    ->setLayerType(LayerType::Line)
                    ->update()
                    ->deletePostgisHasExist(conn);
            layer_area->setFileGroup(*filegroup)
                    ->setLayerType(LayerType::Area)
                    ->update()
                    ->deletePostgisHasExist(conn);
            layer_anno->setFileGroup(*filegroup)
                    ->setLayerType(LayerType::Anno)
                    ->update()
                    ->deletePostgisHasExist(conn);
        }
    }
}

void txtinputDock::on_executeButton_clicked() {
    PGconn *conn = tryConnectPostgis();
    int map_count = maps.size();
    int layer_percent = 0;
    int layer_step = 25;
    int total_percent = 0;
    for (int index = 0; index < map_count; index++) {
        Map* map = maps[index];
        int count = map->groups.size();
        for (int i = 0; i < count; i++) {
            FileGroup *filegroup = map->groups[i];
            QString m = filegroup->matadata.layername;

            if(filegroup->matadata.code.compare("280000") == 0) {
                Layer *layer_anno = new Layer;
                layer_anno->setFileGroup(*filegroup)
                        ->setMapMetadata(map->metadata)
                        ->setLayerType(LayerType::Anno)
                        ->update()
                        ->excutePostgis(conn);
                delete layer_anno;
                layer_percent = layer_percent + layer_step > 100 ? 0 : layer_percent + layer_step;
                layerProgressBar->setValue(layer_percent);
            } else {
                Layer *layer_point = new Layer;
                Layer *layer_line = new Layer;
                Layer *layer_area = new Layer;
                layer_point->setFileGroup(*filegroup)
                        ->setMapMetadata(map->metadata)
                        ->setLayerType(LayerType::Point)
                        ->update()
                        ->excutePostgis(conn);

                layer_percent = layer_percent + layer_step > 100 ? 0 : layer_percent + layer_step;
                layerProgressBar->setValue(layer_percent);

                layer_line->setFileGroup(*filegroup)
                        ->setMapMetadata(map->metadata)
                        ->setLayerType(LayerType::Line)
                        ->update()
                        ->excutePostgis(conn);

                layer_percent = layer_percent + layer_step > 100 ? 0 : layer_percent + layer_step;
                layerProgressBar->setValue(layer_percent);

                layer_area->setFileGroup(*filegroup)
                        ->setMapMetadata(map->metadata)
                        ->setLayerType(LayerType::Area)
                        ->update()
                        ->excutePostgis(conn);

                layer_percent = layer_percent + layer_step > 100 ? 0 : layer_percent + layer_step;
                layerProgressBar->setValue(layer_percent);

                delete layer_point;
                delete layer_line;
                delete layer_area;
            }
        }
        total_percent = 100 * (index + 1) / map_count;
        totalProgressBar->setValue(total_percent);
    }
}
