// ----------------------------------------------------------------------------
// Filename:    ktimeclock.h
// ----------------------------------------------------------------------------
// $Revision: 1.1.1.1 $
// ----------------------------------------------------------------------------

#ifndef KTIMECLOCK_H
#define KTIMECLOCK_H

#include <K3ListView>
#include <QDateTime>
#include <QTimer>
#include <QDomElement>

class KTimeclockListItem;

/**
 * Main view for the timeclock.
 *
 * @short   Main timeclock list view.
 * @author  Graham TerMarsch (graham@howlingfrog.com)
 */
class KTimeclock : public K3ListView {
    Q_OBJECT

    public:
        /**
         * Constructs a new KTimeclock widget.
         *
         * @param   parent  Handle to parent widget.
         */
        KTimeclock (QWidget* parent=0);

        /**
         * Destructs the widget.
         */
        virtual ~KTimeclock ();

    public slots:
        /**
         * Loads the timeclock data from the XML data file.
         *
         * @see     _loadXMLData()
         */
        void loadData ();

        /**
         * Saves the data file, in XML format.
         */
        void saveData ();

        /**
         * Loads up our column sizes out of our configuration file.
         */
        void loadPreferences ();

        /**
         * Saves out the sizes of our columns.
         */
        void savePreferences ();

        /**
         * Starts up the timeclock timer, having it go off once a second.
         *
         * @see     stop()
         */
        void start ();

        /**
         * Stops the timeclock timer, if one is running.
         *
         * @see     start()
         */
        void stop ();

        /**
         * Prompts the user for information on a new top-level project to add,
         * and adds the project to the list.
         */
        void addProject ();

        /**
         * Prompts the user for information on a new sub-project to add, and
         * adds the sub-project to the list as a child of the currently
         * selected project.
         */
        void addSubProject ();

        /**
         * Prompts the user for information on a new task to add, and adds the
         * task to the list as a child of the currently selected
         * project/sub-project.
         */
        void addTask ();

        /**
         * Prompts the user to edit the information for the currently selected
         * item (which can be either a project/sub-project, or a task).  Once
         * edited, the item in the list view is updated with the new
         * information.
         */
        void editItem ();

        /**
         * Confirms with the user that he/she really wants to delete the
         * currently selected item, and then deletes it and all child items
         * from the list view.
         */
        void deleteItem ();

        /**
         * Clears all session time and total time spent for the currently
         * selected task.
         */
        void clearTask ();

        /**
         * Confirms with the user that he/she really wants to clear the session
         * time and total time spent for all of the tasks, and then clears all
         * of the time spent.
         */
        void clearAllTasks ();

        /**
         * Confirms with the user that he/she really wants to clear the session
         * time for all tasks, and then clears the session time.  Note, that
         * this does not affect the total time spent for the tasks, only the
         * recording of how much time has been spent on the task in this
         * session.
         */
        void clearSession ();

    signals:
        /**
         * Emits a status message, which can optionally be shown to the user.
         *
         * @param   msg     Status message to show to the user.
         */
        void status (const QString msg);

        /**
         * Emits an error message, which should be shown to the user.
         *
         * @param   msg     Error message to show to the user.
         */
        void error (const QString msg);

        /**
         * Emitted each time the amount of time recorded in this session
         * changes.  The parameter @p time will be in the format of HH:MM:SS.
         *
         * @param   time    Formatted time string showing session time.
         */
        void sessionTime (const QString time);

        /**
         * Emitted each time the timer state changes.
         *
         * @param   msg     String message showing current timer state.
         */
        void timerState (const QString msg);

        /**
         * Emitted each time the timeclock timer is started.
         */
        void timerStarted ();

        /**
         * Emitted each time the timeclock timer is stopped.
         */
        void timerStopped ();

    protected slots:
        void secondTimerEvent();
        void autosaveTimerEvent();

    private:
        /**
         * List view column indices for the various columns we've got in our
         * display.
         */
        int _col_description;
        int _col_session;
        int _col_total;
        int _col_rate;
        int _col_earnings;

        /**
         * Timer for the timeclock timer.
         */
        QTimer* _seconds_timer;

        /**
         * Time of last timeclock timer tick.  Used to help determine if we're
         * ever missing time due to lost ticks.
         */
        QTime _last_tick;

        /**
         * Timer for the autosave timer.
         */
        QTimer* _autosave_timer;

        /**
         * Current amount of time recorded in this session, in seconds.
         */
        long _session_time;

        /**
         * Recursively loads our timeclock information from an XML DOM tree,
         * into a series of list view items.
         *
         * @param   p_item  Parent list view item to add children to
         * @param   elem    DOM element to get the information from.
         */
        void _dom_to_list (KTimeclockListItem* p_item, QDomElement elem);

        /**
         * Recursively saves out our timeclock information from our list view
         * into an XML DOM document.
         *
         * @param   doc     DOM document to create elements in
         * @param   p_elem  Parent DOM element to add children to
         * @param   item    List view item to get the information from
         */
        void _list_to_dom (QDomDocument doc, QDomElement p_elem, KTimeclockListItem* item);

        /**
         * Recursively clears the session time and total time spent for the
         * given list item and all child items underneath it.  If @p session is
         * TRUE, session time is cleared.  If @p total is TRUE, total time
         * spent is cleared.
         *
         * @param   item    List view item to clear time underneath
         * @param   session Should we clear the session time?
         * @param   total   Should we clear the total time spent?
         */
        void _recursiveClear (KTimeclockListItem* item, bool session=true, bool total=true);

        /**
         * Recursively updates the rate shown in the lsit view for the given
         * list view item and all child items underneath it.
         *
         * @param   item    List view item to update rates for
         */
        void _recursiveUpdateRate (KTimeclockListItem* item);

        /**
         * Resets the autosave timer, based on the information in our
         * configuration file for the delay between autosaves.
         */
        void _setAutosaveTimer ();

        /**
         * Loads our timeclock information from an XML data file.  Returns TRUE
         * if we're able to load information from the data file, returning
         * FALSE otherwise.
         *
         * @returns Did we load information from the data file?
         */
        bool _loadXMLData ();
};

#endif
