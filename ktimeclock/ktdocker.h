// ----------------------------------------------------------------------------
// Filename:    ktdocker.h
// ----------------------------------------------------------------------------
// $Revision: 1.1.1.1 $
// ----------------------------------------------------------------------------

#ifndef KTIMECLOCK_DOCKER_H
#define KTIMECLOCK_DOCKER_H

#include <ksystemtray.h>
#include <qvector.h>
class QPixmap;

/**
 * System tray docking widget for KTimeclock.  This widget shows an animated
 * clock in the system tray when the timer is enabled and the clock is running.
 *
 * @short   System tray docking widget for KTimeclock.
 * @author  Graham TerMarsch (graham@howlingfrog.com)
 */
class KTimeclockDocker : public KSystemTray
{
    Q_OBJECT

    public:
        /**
         * Constructs a new KTimeclockDocker widget.
         *
         * @param   parent  Handle to our parent widget
         * @param   name    Name of this widget
         */
        KTimeclockDocker (QWidget* parent=0, const char* name=0);

        /**
         * Destructs the widget.
         */
        virtual ~KTimeclockDocker ();

    public slots:
        /**
         * To be called whenever the timer is started for the timeclock.  When
         * called, this method starts the animation sequence in the system
         * tray.
         */
        void timerStarted ();

        /**
         * To be called whenever the timer is stopped for the timeclock.  When
         * called, this method stops the animation sequence in the system tray.
         */
        void timerStopped ();

    private slots:
        /**
         * Called each time our timer "ticks".  We move onto the next frame in
         * the animation sequence and update the icon in the system tray.
         */
        void timerEvent (QTimerEvent* event);

    private:
        /**
         * Vector of all of the icons that we're using in the animation
         * sequence.
         */
        QVector<QPixmap>* _icons;

        /**
         * Our timer ID that helps keep the animation running.
         */
        int _timer_id;

        /**
         * The frame number of the current frame in the animation sequence.
         */
        int _frame;
};

#endif
