// ----------------------------------------------------------------------------
// Filename:    mainwindow.cpp
// ----------------------------------------------------------------------------
// $Revision: 1.2 $
// ----------------------------------------------------------------------------

#include "mainwindow.h"
#include "ktdocker.h"
#include "ktimeclock.h"
#include "kttime.h"
#include "resource.h"

#include <QString>
#include <KAction>
#include <KConfig>
#include <KEditToolBar>
#include <KGlobal>
#include <KShortcutsDialog>
#include <KLocale>
#include <KMenuBar>
#include <KStandardAction>
#include <KStatusBar>
#include <KApplication>
#include <KActionCollection>

// ----------------------------------------------------------------------------
// Function:    KTimeclockWindow ()
// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
KTimeclockWindow::KTimeclockWindow ()
    : KXmlGuiWindow( 0 )
{
    // ------------------------------------------------------------------------
    // Create a new timeclock and make that our central widget
    // ------------------------------------------------------------------------
    _timeclock = new KTimeclock( this );
    setCentralWidget( _timeclock );

    // ------------------------------------------------------------------------
    // Create a dock widget to go along with the timeclock, and connect it to
    // the timeclock.
    // ------------------------------------------------------------------------
    _docker = new KTimeclockDocker( this );
    _docker->show();
    _docker->connect( _timeclock, SIGNAL(timerStarted()), SLOT(timerStarted()) );
    _docker->connect( _timeclock, SIGNAL(timerStopped()), SLOT(timerStopped()) );

    // ------------------------------------------------------------------------
    // Set up some signals/slots to connect ourselves to the timeclock.
    // ------------------------------------------------------------------------
    connect( _timeclock, SIGNAL(status(const QString)),
             this, SLOT(updateStatus(const QString)) );
    connect( _timeclock, SIGNAL(sessionTime(const QString)),
             this, SLOT(updateTime(const QString)) );
    connect( _timeclock, SIGNAL(timerState(const QString)),
             this, SLOT(updateState(const QString)) );

    // ------------------------------------------------------------------------
    // Create our status bar.
    // ------------------------------------------------------------------------
    this->initStatusbar();
    this->updateState( i18n("Disabled") );
    this->updateTime( KTimeclockTime::asString(0) );

    // ------------------------------------------------------------------------
    // Popup menus.
    // ------------------------------------------------------------------------
    this->initMenus();
    
    // ------------------------------------------------------------------------
    // Load up our geometry.
    // ------------------------------------------------------------------------
    this->loadGeometry();

    // ------------------------------------------------------------------------
    // Load up our timeclock data (if it exists).
    // ------------------------------------------------------------------------
    _timeclock->loadPreferences();
    _timeclock->loadData();
}

// ----------------------------------------------------------------------------
// Function:    ~KTimeclockWindow ()
// ----------------------------------------------------------------------------
// Destructor.
// ----------------------------------------------------------------------------
KTimeclockWindow::~KTimeclockWindow ()
{
    this->quit();
    if (_docker) delete _docker;
}

// ----------------------------------------------------------------------------
// Function:    updateTime (const QString time)
// Parameters:  time        - Formatted time string to show in status bar
// ----------------------------------------------------------------------------
// Shows the provided session time in the status bar.
// ----------------------------------------------------------------------------
void KTimeclockWindow::updateTime (const QString time)
{
    QString formatted = i18n("Session: ") + time;
    statusBar()->changeItem( formatted, ID_STATUS_SESSION );
}

// ----------------------------------------------------------------------------
// Function:    updateStatus (const QString text)
// Parameters:  text        - Status message to show in the status bar
// ----------------------------------------------------------------------------
// Shows the provided status message in the status bar.
// ----------------------------------------------------------------------------
void KTimeclockWindow::updateStatus (const QString text)
{
    statusBar()->message( text, 4000 );
}

// ----------------------------------------------------------------------------
// Function:    clearStatus ()
// ----------------------------------------------------------------------------
// Clears the status bar.
// ----------------------------------------------------------------------------
void KTimeclockWindow::clearStatus ()
{
    statusBar()->clear();
}

// ----------------------------------------------------------------------------
// Function:    updateState (const QString state)
// Parameters:  state       - Timer state to show in the status bar
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void KTimeclockWindow::updateState (const QString state)
{
    statusBar()->changeItem( state, ID_STATUS_ENABLED );
}

// ----------------------------------------------------------------------------
// Function:    saveGeometry ()
// ----------------------------------------------------------------------------
// Saves our window position and geometry into our configuration file.
// ----------------------------------------------------------------------------
void KTimeclockWindow::saveGeometry ()
{
    KSharedConfigPtr cfg = KGlobal::config();
    KConfigGroup windowGroup(cfg, "Main Window Geometry");
    windowGroup.writeEntry( "Width",    this->width() );
    windowGroup.writeEntry( "Height",   this->height() );
    windowGroup.writeEntry( "Xpos",     this->x() );
    windowGroup.writeEntry( "Ypos",     this->y() );
    windowGroup.config()->sync();
}

// ----------------------------------------------------------------------------
// Function:    loadGeometry ()
// ----------------------------------------------------------------------------
// Loads and resets our window position and geometry from the info in our
// configuration file.
// ----------------------------------------------------------------------------
void KTimeclockWindow::loadGeometry ()
{
    KSharedConfigPtr cfg = KGlobal::config();
    KConfigGroup windowGroup(cfg, "Main Window Geometry");
    int w = windowGroup.readEntry( "Width", 620 );
    int h = windowGroup.readEntry( "Height", 400 );
    int x = windowGroup.readEntry( "Xpos", 0 );
    int y = windowGroup.readEntry( "Ypos", 0 );

    // ------------------------------------------------------------------------
    // Adjust the height/width based on our size hint.
    // ------------------------------------------------------------------------
    w = QMAX( w, sizeHint().width() );
    h = QMAX( h, sizeHint().height() );

    // ------------------------------------------------------------------------
    // Set the size of our window.
    // ------------------------------------------------------------------------
    this->setGeometry( x, y, w, h );
}

// ----------------------------------------------------------------------------
// Function:    keyBindings ()
// ----------------------------------------------------------------------------
// Allows the user to update the key-bindings for our actions.
// ----------------------------------------------------------------------------
void KTimeclockWindow::keyBindings ()
{
    KShortcutsDialog::configureKeys( this->actionCollection(), this->xmlFile() );
}

// ----------------------------------------------------------------------------
// Function:    cfgToolbars ()
// ----------------------------------------------------------------------------
// Allows the user to update the configuration of the toolbars.
// ----------------------------------------------------------------------------
void KTimeclockWindow::cfgToolbars ()
{
    KEditToolBar dlg( actionCollection() );
    if (dlg.exec())
        createGUI();
}

// ----------------------------------------------------------------------------
// Function:    save ()
// ----------------------------------------------------------------------------
// Saves the timeclock data and our own window geometry.
// ----------------------------------------------------------------------------
void KTimeclockWindow::save ()
{
    _timeclock->saveData();
    this->saveGeometry();
}

// ----------------------------------------------------------------------------
// Function:    quit ()
// ----------------------------------------------------------------------------
// Saves our application data and quits the application.
// ----------------------------------------------------------------------------
void KTimeclockWindow::quit ()
{
    this->save();
    kapp->quit();
}

// ----------------------------------------------------------------------------
// Function:    initMenus ()
// ----------------------------------------------------------------------------
// Initializes our menus and creates the GUI.
// ----------------------------------------------------------------------------
void KTimeclockWindow::initMenus ()
{
    // ------------------------------------------------------------------------
    // Standard menu fare
    // ------------------------------------------------------------------------
    KStandardAction::save( _timeclock, SLOT(saveData()),
                actionCollection() );
    KStandardAction::quit( this, SLOT(quit()),
                actionCollection() );

    KStandardAction::keyBindings( this, SLOT(keyBindings()),
                actionCollection() );
    KStandardAction::configureToolbars( this, SLOT(cfgToolbars()),
                actionCollection() );

    // ------------------------------------------------------------------------
    // Our own custom menu items
    // ------------------------------------------------------------------------
    KAction* startAction = new KAction(this);
    startAction->setText(i18n("&Start timer"));
    startAction->setIcon(KIcon("1rightarrow"));
    startAction->setShortcut(Qt::CTRL + Qt::Key_T);
    actionCollection()->addAction("start", startAction);
    connect(startAction, SIGNAL(triggered(bool)), _timeclock, SLOT(start()));

    KAction* stopAction = new KAction(this);
    stopAction->setText(i18n("S&top timer"));
    stopAction->setIcon(KIcon("stop"));
    stopAction->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_T);
    actionCollection()->addAction("stop", stopAction);
    connect(stopAction, SIGNAL(triggered(bool)), _timeclock, SLOT(stop()));

    KAction* newProjectAction = new KAction(this);
    newProjectAction->setText(i18n("New &project"));
    newProjectAction->setIcon(KIcon("folder_new"));
    newProjectAction->setShortcut(Qt::CTRL + Qt::Key_P);
    actionCollection()->addAction("addproject", newProjectAction);
    connect(newProjectAction, SIGNAL(triggered(bool)), _timeclock, SLOT(addProject()));

    KAction* newSubProjectAction = new KAction(this);
    newSubProjectAction->setText(i18n("New &sub-project"));
    newSubProjectAction->setIcon(KIcon("folder_new"));
    newSubProjectAction->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_P);
    actionCollection()->addAction("addsubproject", newSubProjectAction);
    connect(newSubProjectAction, SIGNAL(triggered(bool)), _timeclock, SLOT(addSubProject()));

    KAction* newTaskAction = new KAction(this);
    newTaskAction->setText(i18n("&New task"));
    newTaskAction->setIcon(KIcon("filenew"));
    newTaskAction->setShortcut(Qt::CTRL + Qt::Key_N);
    actionCollection()->addAction("addtask", newTaskAction);
    connect(newTaskAction, SIGNAL(triggered(bool)), _timeclock, SLOT(addTask()));

    KAction* editTaskAction = new KAction(this);
    editTaskAction->setText(i18n("&Edit task"));
    editTaskAction->setIcon(KIcon("edit"));
    editTaskAction->setShortcut(Qt::CTRL + Qt::Key_E);
    actionCollection()->addAction("edititem", editTaskAction);
    connect(editTaskAction, SIGNAL(triggered(bool)), _timeclock, SLOT(editItem()));

    KAction* deleteTaskAction = new KAction(this);
    deleteTaskAction->setText(i18n("&Delete task"));
    deleteTaskAction->setIcon(KIcon("editdelete"));
    deleteTaskAction->setShortcut(Qt::CTRL + Qt::Key_D);
    actionCollection()->addAction("deleteitem", deleteTaskAction);
    connect(deleteTaskAction, SIGNAL(triggered(bool)), _timeclock, SLOT(deleteItem()));

    KAction* clearTaskAction = new KAction(this);
    clearTaskAction->setText(i18n("&Clear task"));
    clearTaskAction->setShortcut(Qt::CTRL + Qt::Key_C);
    actionCollection()->addAction("cleartask", clearTaskAction);
    connect(clearTaskAction, SIGNAL(triggered(bool)), _timeclock, SLOT(clearTask()));

    KAction* clearAllTasksAction = new KAction(this);
    clearAllTasksAction->setText(i18n("Clear &all tasks"));
    clearAllTasksAction->setShortcut(Qt::CTRL + Qt::Key_A);
    actionCollection()->addAction("clearalltasks", clearAllTasksAction);
    connect(clearAllTasksAction, SIGNAL(triggered(bool)), _timeclock, SLOT(clearAllTasks()));

    KAction* resetSessionAction = new KAction(this);
    resetSessionAction->setText(i18n("Reset &session"));
    resetSessionAction->setShortcut(Qt::CTRL + Qt::Key_R);
    actionCollection()->addAction("clearsession", resetSessionAction);
    connect(resetSessionAction, SIGNAL(triggered(bool)), _timeclock, SLOT(clearSession()));

    // ------------------------------------------------------------------------
    // Create the GUI from our XML data file.
    // ------------------------------------------------------------------------
    createGUI( "ktimeclockui.rc" );
}

// ----------------------------------------------------------------------------
// Function:    initStatusBar ()
// ----------------------------------------------------------------------------
// Initializes our status bar.
// ----------------------------------------------------------------------------
void KTimeclockWindow::initStatusbar ()
{
    statusBar()->insertItem( QString::null, ID_STATUS_ENABLED, 0, true);
    statusBar()->insertItem( QString::null, ID_STATUS_SESSION, 0, true );
}
