// ----------------------------------------------------------------------------
// Filename:    mainwindow.h
// ----------------------------------------------------------------------------
// $Revision: 1.1.1.1 $
// ----------------------------------------------------------------------------

#ifndef KTIMECLOCK_MAINWINDOW_H
#define KTIMECLOCK_MAINWINDOW_H

#include <KMainWindow>
#include "ktimeclock.h"

class KTimeclockDocker;

/**
 * Main window for the timeclock application.
 *
 * @short   Main timeclock window.
 * @author  Graham TerMarsch (graham@howlingfrog.com)
 */
class KTimeclockWindow : public KMainWindow
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         */
        KTimeclockWindow ();

        /**
         * Destructor.
         */
        virtual ~KTimeclockWindow ();

    public slots:
        /**
         * Shows the provided session time in the status bar.
         *
         * @param   time    Formatted time string to show in status bar.
         */
        void updateTime (const QString time);

        /**
         * Shows the provided status message in the status bar.
         *
         * @param   text    Status message to show in the status bar.
         */
        void updateStatus (const QString text);

        /**
         * Clears the status bar.
         */
        void clearStatus ();

        /**
         * Shows the provided timer state in the status bar.
         * 
         * @param   state   Timer state to show in the status bar.
         */
        void updateState (const QString state);

    protected:
        /**
         * Saves our window position and geometry into our configuration file.
         */
        void saveGeometry ();

        /**
         * Loads and resets our window position and geometry from the info in
         * our configuration file.
         */
        void loadGeometry ();

    protected slots:
        /**
         * Allows the user to update the key-bindings for our actions.
         */
        void keyBindings ();

        /**
         * Allows the user to update the configuration of the toolbars.
         */
        void cfgToolbars ();

        /**
         * Saves the timeclock data and our wown window geometry.
         */
        void save ();

        /**
         * Saves our application data and quits the application.
         */
        void quit ();

    private:
        /**
         * Timeclock widget.
         */
        KTimeclock      *_timeclock;

        /**
         * Dock widget for the status tray.
         */
        KTimeclockDocker *_docker;

        /**
         * Initializes our menus and creates the GUI.
         */
        void initMenus ();

        /**
         * Initializes our status bar.
         */
        void initStatusbar ();
};

#endif
