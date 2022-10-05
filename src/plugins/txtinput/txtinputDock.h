#ifndef TXTINPUTDOCK_H
#define TXTINPUTDOCK_H

#include "qgsdockwidget.h"

#include "ui_txtinputDock.h"

class QgisInterface;

class txtinputDock : public QgsDockWidget, private Ui::txtinputDock
{
    Q_OBJECT

public:
    explicit txtinputDock( QgisInterface *qIface, QWidget *parent = nullptr);
    ~txtinputDock() override;
private slots:
    void on_hostEdit_textChanged(QString);
    void on_executeButton_clicked();
private:
    QgisInterface *qgsInterface = nullptr;
    QString host;
};

#endif // TXTINPUTDOCK_H
