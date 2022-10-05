/***************************************************************************
    txtinput.h
 **************************************************************************/
#ifndef TXTINPUT_H
#define TXTINPUT_H

//QT includes
#include <QObject>

//QGIS includes
#include "../qgisplugin.h"

//forward declarations
class QAction;
class QToolBar;

class QgisInterface;
class txtinputDock;

/**
* \class Plugin
* \brief [name] plugin for QGIS
* [description]
*/
class Txtinput: public QObject, public QgisPlugin
{
    Q_OBJECT
  public:

    //////////////////////////////////////////////////////////////////////
    //
    //                MANDATORY PLUGIN METHODS FOLLOW
    //
    //////////////////////////////////////////////////////////////////////

    /**
    * Constructor for a plugin. The QgisInterface pointer is passed by
    * QGIS when it attempts to instantiate the plugin.
    * \param interface Pointer to the QgisInterface object.
     */
    explicit Txtinput( QgisInterface *interface );

  public slots:
    //! init the gui
    void initGui() override;
    //! Create and show the dialog box
    void run();
    //! Show/hide the dialog box
    void showOrHide();
    //! unload the plugin
    void unload() override;
    //! show the help document
    void help();

  private:

    ////////////////////////////////////////////////////////////////////
    //
    // MANDATORY PLUGIN PROPERTY DECLARATIONS  .....
    //
    ////////////////////////////////////////////////////////////////////

    //! Pointer to the QGIS interface object
    QgisInterface *mQGisIface = nullptr;
    //!pointer to the qaction for this plugin
    QAction *mQActionPointer = nullptr;
    txtinputDock *mDock = nullptr;

    ////////////////////////////////////////////////////////////////////
    //
    // ADD YOUR OWN PROPERTY DECLARATIONS AFTER THIS POINT.....
    //
    ////////////////////////////////////////////////////////////////////
};

#endif //TXTINPUT_H
