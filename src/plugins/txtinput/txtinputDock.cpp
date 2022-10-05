#include <QMessageBox>

#include "txtinputDock.h"

#include "qgisinterface.h"

txtinputDock::txtinputDock(QgisInterface *qIface, QWidget *parent)
    :QgsDockWidget(parent), Ui::txtinputDock()
{
    setupUi( this );

    // hide the fix-related stuff, needs more work
    qgsInterface = qIface;
}

txtinputDock::~txtinputDock()
{

}

void txtinputDock::on_hostEdit_textChanged(QString hoststr) {
    host = hoststr;
}

void txtinputDock::on_executeButton_clicked() {
    QMessageBox::information( this, tr( "Topology test" ), host );
}
