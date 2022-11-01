#ifndef TXTINPUTDOCK_H
#define TXTINPUTDOCK_H

#include <stdio.h>

#include "qgsdockwidget.h"

#include "txtexporttopostgis.h"
#include "odata.h"

#include "ui_txtinputDock.h"

using namespace std;

class QgisInterface;

class txtinputDock : public QgsDockWidget, private Ui::txtinputDock
{
    Q_OBJECT

public:
    explicit txtinputDock( QgisInterface *qIface, QWidget *parent = nullptr);
    ~txtinputDock() override;
private slots:
//    void on_hostEdit_textChanged(QString);
//    void on_portBox_valueChanged(int host);
//    void on_databaseEdit_textChanged(QString databasestr);
//    void on_userEdit_textChanged(QString userstr);
//    void on_passwordEdit_textChanged(QString passwordstr);
    void on_executeButton_clicked();
    void on_selectDirctionButton_clicked();

    void on_deleteButton_clicked();
    void on_createIndexButton_clicked();
private:
    QgisInterface *qgsInterface = nullptr;
    TxtExportToPostgis *txtExport = nullptr;
    QString host;
    QString port;
    QString database;
    QString username;
    QString password;

    vector<Map*> maps;

    PGconn* tryConnectPostgis();
};

#endif // TXTINPUTDOCK_H
