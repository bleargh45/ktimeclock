// ----------------------------------------------------------------------------
// Filename:    ktdocker.cpp
// ----------------------------------------------------------------------------
// $Revision: 1.1.1.1 $
// ----------------------------------------------------------------------------

#include "ktdocker.h"
#include <KLocale>
#include <KIconLoader>
#include <QTimer>
#include <QTimerEvent>

// ----------------------------------------------------------------------------
// Function:    KTimeclockDocker *QWidget* parent=0)
// Parameters:  parent      - Handle to our parent widget
// ----------------------------------------------------------------------------
// Constructs a new KTimeclockDocker widget.
// ----------------------------------------------------------------------------
KTimeclockDocker::KTimeclockDocker (QWidget* parent)
    : KSystemTrayIcon( parent ), _timer_id(0), _frame(0)
{
    // Load up the icons that we're going to show in the task bar.
    _icons.insert(1, KSystemTrayIcon::loadIcon("clock"));
    _icons.insert(2, KSystemTrayIcon::loadIcon("dockicon-1"));

    this->setIcon(_icons.at(0));
}

// ----------------------------------------------------------------------------
// Function:    ~KTimeclockDocker ()
// ----------------------------------------------------------------------------
// Destructs the widget.
// ----------------------------------------------------------------------------
KTimeclockDocker::~KTimeclockDocker () {
}

// ----------------------------------------------------------------------------
// Function:    timerStarted ()
// ----------------------------------------------------------------------------
// To be called whenever the timer is started for the timeclock.  When called,
// this method starts the animation sequence in the system tray.
// ----------------------------------------------------------------------------
void KTimeclockDocker::timerStarted () {
    this->setIcon(_icons.at(1));
}

// ----------------------------------------------------------------------------
// Function:    timerStopped ()
// ----------------------------------------------------------------------------
// To be called whenever the timer is stopped for the timeclock.  When called,
// this method stops the animation sequence in the system tray.
// ----------------------------------------------------------------------------
void KTimeclockDocker::timerStopped () {
    this->setIcon(_icons.at(0));
}

// ----------------------------------------------------------------------------
// Function:    timerEvent (QTimerEvent*)
// ----------------------------------------------------------------------------
// Called each time our timer "ticks".  We move onto the next frame in the
// animation sequence and update the icon in the system tray.
// ----------------------------------------------------------------------------
void KTimeclockDocker::timerEvent (QTimerEvent*) {
    // ------------------------------------------------------------------------
    // Redraw the icon in the dock with the next frame in the animation
    // sequence.
    // ------------------------------------------------------------------------
    this->setIcon( _icons.at(_frame) );

    // ------------------------------------------------------------------------
    // Move onto the next frame in the animation sequence.
    // ------------------------------------------------------------------------
    _frame = (_frame + 1) % _icons.size();
}
