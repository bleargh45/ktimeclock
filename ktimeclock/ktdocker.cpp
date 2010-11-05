// ----------------------------------------------------------------------------
// Filename:    ktdocker.cpp
// ----------------------------------------------------------------------------
// $Revision: 1.1.1.1 $
// ----------------------------------------------------------------------------

#include "ktdocker.h"
#include <KLocale>
#include <KIconLoader>
#include <QTimer>
#include <QPixmap>
#include <QTimerEvent>

// ----------------------------------------------------------------------------
// Function:    KTimeclockDocker *QWidget* parent=0, const char* name=0)
// Parameters:  parent      - Handle to our parent widget
//              name        - Name of this widget
// ----------------------------------------------------------------------------
// Constructs a new KTimeclockDocker widget.
// ----------------------------------------------------------------------------
KTimeclockDocker::KTimeclockDocker (QWidget* parent, const char* name)
    : KSystemTrayIcon( parent, name ), _timer_id(0), _frame(0)
{
    // ------------------------------------------------------------------------
    // Load up all of the pixmaps that we're going to use when animating the
    // clock in the task bar.
    // ------------------------------------------------------------------------
    _icons = new QVector<QPixmap>(8);
    for (int idx=0; idx<8; idx++)
    {
        QPixmap* icon = new QPixmap;
        QString file;
        file.sprintf( "dockicon%d", idx );
        *icon = UserIcon( file );
        _icons->insert( idx, icon );
    }

    // ------------------------------------------------------------------------
    // Start off showing the first dock icon.
    // ------------------------------------------------------------------------
    this->setPixmap( *(*_icons)[ _frame++ ] );
}

// ----------------------------------------------------------------------------
// Function:    ~KTimeclockDocker ()
// ----------------------------------------------------------------------------
// Destructs the widget.
// ----------------------------------------------------------------------------
KTimeclockDocker::~KTimeclockDocker ()
{
    if (_icons) delete _icons;
}

// ----------------------------------------------------------------------------
// Function:    timerStarted ()
// ----------------------------------------------------------------------------
// To be called whenever the timer is started for the timeclock.  When called,
// this method starts the animation sequence in the system tray.
// ----------------------------------------------------------------------------
void KTimeclockDocker::timerStarted ()
{
    if (_timer_id == 0)
    {
        _timer_id = startTimer( 1000 );
    }
}

// ----------------------------------------------------------------------------
// Function:    timerStopped ()
// ----------------------------------------------------------------------------
// To be called whenever the timer is stopped for the timeclock.  When called,
// this method stops the animation sequence in the system tray.
// ----------------------------------------------------------------------------
void KTimeclockDocker::timerStopped ()
{
    killTimer( _timer_id );
    _timer_id = 0;
}

// ----------------------------------------------------------------------------
// Function:    timerEvent (QTimerEvent*)
// ----------------------------------------------------------------------------
// Called each time our timer "ticks".  We move onto the next frame in the
// animation sequence and update the icon in the system tray.
// ----------------------------------------------------------------------------
void KTimeclockDocker::timerEvent (QTimerEvent*)
{
    // ------------------------------------------------------------------------
    // Redraw the icon in the dock with the next frame in the animation
    // sequence.
    // ------------------------------------------------------------------------
    this->setPixmap( *(*_icons)[ _frame ] );

    // ------------------------------------------------------------------------
    // Move onto the next frame in the animation sequence.
    // ------------------------------------------------------------------------
    _frame = (_frame + 1) % _icons->size();
}
