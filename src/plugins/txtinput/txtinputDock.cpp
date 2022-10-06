#include <QMessageBox>

#include "txtinputDock.h"

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

//void txtinputDock::on_hostEdit_textChanged(QString hoststr) {
//    host = hoststr;
//}
//void txtinputDock::on_portBox_valueChanged(int portvalue) {
//    port = QString::number(portvalue);
//}
//void txtinputDock::on_databaseEdit_textChanged(QString databasestr) {
//    database = databasestr;
//}
//void txtinputDock::on_userEdit_textChanged(QString userstr) {
//    username = userstr;
//}
//void txtinputDock::on_passwordEdit_textChanged(QString passwordstr) {
//    password = passwordstr;
//}

void txtinputDock::on_executeButton_clicked() {

    QgsDataSourceUri uri;

    host = hostEdit->text();
    port = QString::number(portBox->value());
    database = databaseEdit->text();
    username = userEdit->text();
    password = passwordEdit->text();

    uri.setConnection(host, port, database, username, password);
    txtExport->setConnection(uri);
    QString user = txtExport->checkUser();
    QString info = host + port + database + username + password;
    QMessageBox::information( this, tr( "User Info" ), user, info );

}
