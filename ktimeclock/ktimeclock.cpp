// ----------------------------------------------------------------------------
// Filename:    ktimeclock.cpp
// ----------------------------------------------------------------------------
// $Revision: 1.2 $
// ----------------------------------------------------------------------------

#include "ktimeclock.h"
#include "ktaddedit.h"
#include "kttime.h"
#include "ktlistitem.h"
#include "preferences.h"
#include <Q3Header>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <KConfig>
#include <KGlobal>
#include <KIconLoader>
#include <KLocale>
#include <KMessageBox>
#include <KStandardDirs>
#include <KUrl>

// ----------------------------------------------------------------------------
// Function:    KTimeclock (QWidget* parent=0)
// Parameters:  parent      - Handle to parent widget
// ----------------------------------------------------------------------------
// Constructs a new KTimeclock widget.
// ----------------------------------------------------------------------------
KTimeclock::KTimeclock (QWidget* parent)
    : K3ListView(parent), _seconds_timer(NULL), _autosave_timer(NULL)
{
    // ------------------------------------------------------------------------
    // Create all of the columns that we're going to display.
    // ------------------------------------------------------------------------
    _col_description = this->addColumn( i18n("Description") );
    _col_session     = this->addColumn( i18n("Session") );
    _col_total       = this->addColumn( i18n("Total") );
    _col_rate        = this->addColumn( i18n("Rate") );
    _col_earnings    = this->addColumn( i18n("Earnings") );

    // ------------------------------------------------------------------------
    // Let columns have icon decorations, and full focus when selected.
    // ------------------------------------------------------------------------
    this->setRootIsDecorated( true );
    this->setAllColumnsShowFocus( true );

    // ------------------------------------------------------------------------
    // Set up the dollars columns to be right justified.
    // ------------------------------------------------------------------------
    this->setColumnAlignment( _col_rate, Qt::AlignRight );
    this->setColumnAlignment( _col_earnings, Qt::AlignRight );

    // ------------------------------------------------------------------------
    // Clear the current session time.
    // ------------------------------------------------------------------------
    _session_time = 0;
}

// ----------------------------------------------------------------------------
// Function:    ~KTimeclock ()
// ----------------------------------------------------------------------------
// Destructs the widget.
// ----------------------------------------------------------------------------
KTimeclock::~KTimeclock ()
{
}

// ----------------------------------------------------------------------------
// Function:    loadData ()
// ----------------------------------------------------------------------------
// Loads the timeclock data from the first available data file.  Searches first
// for an XML data file, falling back onto the old-style text data file if no
// XML data file is found.
// ----------------------------------------------------------------------------
void KTimeclock::loadData ()
{
    // ------------------------------------------------------------------------
    // Turn off updates for the display, and clear the list of items in our
    // list.
    // ------------------------------------------------------------------------
    this->setUpdatesEnabled( FALSE );
    this->clear();

    // ------------------------------------------------------------------------
    // Try to load up an XML data file.
    // ------------------------------------------------------------------------
    bool gotdata = this->_loadXMLData();

    // ------------------------------------------------------------------------
    // If we didn't find any XML data to load, see if there is a legacy text
    // file from the KDE-1.x version and try to load that instead.
    // ------------------------------------------------------------------------
    if (!gotdata) {
        gotdata = this->_loadTextData();
    }

    // ------------------------------------------------------------------------
    // Turn updates on again and trigger an update of the visuals.
    // ------------------------------------------------------------------------
    this->setUpdatesEnabled( TRUE );
    this->triggerUpdate();

    // ------------------------------------------------------------------------
    // Emit an appropriate status message.
    // ------------------------------------------------------------------------
    if (gotdata) {
        emit status( i18n("Loaded data file") );
    }
    else {
        emit status( i18n("Unable to load data file") );
    }
}

// ----------------------------------------------------------------------------
// Function:    saveData ()
// ----------------------------------------------------------------------------
// Saves the data file, in XML format.
// ----------------------------------------------------------------------------
void KTimeclock::saveData ()
{
    // ------------------------------------------------------------------------
    // Get the path to the directory where the XML data file is to be stored.
    // If we can't find/create the directory, show an error to the user and
    // abort the save.
    // ------------------------------------------------------------------------
    QString datadir = KGlobal::dirs()->saveLocation(
                        "appdata", QString::null, true
                        );
    if (datadir == QString::null) {
        QString msg = i18n( "Unable to find/create directory to save data to." );
        KMessageBox::error( this, msg );
        return;
    }

    // ------------------------------------------------------------------------
    // Open up a file handle to output the XML data file to; we'll output it to
    // a tmp file first, and then copy over the actual data file.
    // ------------------------------------------------------------------------
    QString fname_xml( datadir + "/ktimeclock.xml" );
    QString fname_bak( fname_xml + ".bak" );
    QString fname_tmp( fname_xml + ".tmp" );
// UNFINISHED - tmp file name should include process ID in it; although we may
//              be running as a unique app, that doesn't mean that whoever else
//              uses this class is.
    QFile file( fname_tmp );
    if (!file.open( QIODevice::WriteOnly )) {
        qWarning( "Can't open temporary data file for output." );
        return;
    }
    QTextStream fout( &file );

    // ------------------------------------------------------------------------
    // Create a new DOM document that we can stuff full of XML.
    // ------------------------------------------------------------------------
    QDomDocument doc( "TIMECLOCK" );
    doc.appendChild( doc.createProcessingInstruction(
                    "xml", "version=\"1.0\" encoding=\"UTF-8\""
                    ) );

    QDomElement root = doc.createElement( "TIMECLOCK" );
// UNFINISHED -> Should include a file format version number.
    doc.appendChild( root );

    // ------------------------------------------------------------------------
    // Get the first element in the list that we're going to output, and build
    // up a complete XML document if the list isn't empty.
    // create XML elements for all of them.
    // ------------------------------------------------------------------------
    KTimeclockListItem* item = (KTimeclockListItem*)this->firstChild();
    if (item != 0) {
        this->_list_to_dom( doc, root, item );
    }

    // ------------------------------------------------------------------------
    // Now that we've got the whole XML document ready, output it to the data
    // file.
    // ------------------------------------------------------------------------
    fout << doc;
    file.close();

    // ------------------------------------------------------------------------
    // Backup the existing XML data file, and move this one into its place.
    // ------------------------------------------------------------------------
    // ... remove old backup
    QFile fileBackup( fname_bak );
    fileBackup.remove();

    // ... move current to backup
    QFile fileXMLData( fname_xml );
    fileXMLData.rename( fname_bak );

    // ... move newly saved temp file to current
    QFile fileTempFile( fname_tmp );
    fileTempFile.rename( fname_xml );

    emit status( i18n("Saved data file") );
}

// ----------------------------------------------------------------------------
// Function:    loadPreferences ()
// ----------------------------------------------------------------------------
// Loads up our column sizes out of our configuration file.
// ----------------------------------------------------------------------------
void KTimeclock::loadPreferences ()
{
    emit status( "Loading data..." );
    KSharedConfigPtr cfg = KGlobal::config();
    KConfigGroup columnGroup( cfg, "Column Sizes" );

    // ------------------------------------------------------------------------
    // Read in the sizes of the columns out of our config file.
    // ------------------------------------------------------------------------
    int w;
    w = columnGroup.readEntry( "Description", 300 );
    setColumnWidth( _col_description, w );
    w = columnGroup.readEntry( "Session", 70 );
    setColumnWidth( _col_session, w );
    w = columnGroup.readEntry( "Total", 70 );
    setColumnWidth( _col_total, w );
    w = columnGroup.readEntry( "Rate", 70 );
    setColumnWidth( _col_rate, w );
    w = columnGroup.readEntry( "Earnings", 70 );
    setColumnWidth( _col_earnings, w );

    // ------------------------------------------------------------------------
    // Start up an autosave timer.
    // ------------------------------------------------------------------------
    this->_setAutosaveTimer();

    // ------------------------------------------------------------------------
    // Done loading in our preferences.
    // ------------------------------------------------------------------------
    emit status( i18n("Loaded preferences") );
}

// ----------------------------------------------------------------------------
// Function:    savePreferences ()
// ----------------------------------------------------------------------------
// Saves out the sizes of our columns.
// ----------------------------------------------------------------------------
void KTimeclock::savePreferences ()
{
    emit status( "Saving data..." );
    KSharedConfigPtr cfg = KGlobal::config();
    KConfigGroup columnGroup( cfg, "Column Sizes" );

    // ------------------------------------------------------------------------
    // Write out the sizes of all of our columns to our config file.
    // ------------------------------------------------------------------------
    columnGroup.writeEntry( "Description",  columnWidth(_col_description) );
    columnGroup.writeEntry( "Session",      columnWidth(_col_session) );
    columnGroup.writeEntry( "Total",        columnWidth(_col_total) );
    columnGroup.writeEntry( "Rate",         columnWidth(_col_rate) );
    columnGroup.writeEntry( "Earnings",     columnWidth(_col_earnings) );
    columnGroup.config()->sync();

    // ------------------------------------------------------------------------
    // All done saving our preferences.
    // ------------------------------------------------------------------------
    emit status( i18n("Saved preferences") );
}

// ----------------------------------------------------------------------------
// Function:    start ()
// ----------------------------------------------------------------------------
// Starts up the timeclock timer, having it go off once a second.
// ----------------------------------------------------------------------------
void KTimeclock::start ()
{
    if (_seconds_timer == NULL) {
        _seconds_timer = new QTimer(this);
        connect(_seconds_timer, SIGNAL(timeout()), this, SLOT(secondTimerEvent()));
    }

/* XXX */
    _last_tick = QTime::currentTime();
    _seconds_timer->start(1000);
    emit timerStarted();
    emit timerState( "Enabled" );
}

// ----------------------------------------------------------------------------
// Function:    stop ()
// ----------------------------------------------------------------------------
// Stops the timeclock timer, if one is running.
// ----------------------------------------------------------------------------
void KTimeclock::stop ()
{
    if (_seconds_timer != NULL) {
        _seconds_timer->stop();
        emit timerStopped();
        emit timerState( "Disabled" );
    }
}

// ----------------------------------------------------------------------------
// Function:    addProject ()
// ----------------------------------------------------------------------------
// Prompts the user for information on a new top-level project to add, and adds
// the project to the list.
// ----------------------------------------------------------------------------
void KTimeclock::addProject ()
{
    DlgAddEditProject* dlg = new DlgAddEditProject( this );
    dlg->setCaption( i18n("Add project") );
    if (dlg->exec()) {
        KTimeclockListItem* item = new KTimeclockListItem( this );
        item->setProject( true );
        item->setDescription( dlg->description() );
        emit status( i18n("New project added") );
    }
    delete dlg;
}

// ----------------------------------------------------------------------------
// Function:    addSubProject ()
// ----------------------------------------------------------------------------
// Prompts the user for information on a new sub-project to add, and adds the
// sub-project to the list as a child of the currently selected project.
// ----------------------------------------------------------------------------
void KTimeclock::addSubProject ()
{
    // ------------------------------------------------------------------------
    // Get the currently selected item and verify that its a project; we can't
    // add sub-projects to tasks or if no project is selected.
    // ------------------------------------------------------------------------
    KTimeclockListItem* selected = (KTimeclockListItem*)this->currentItem();
    if ( (selected == 0) || (!selected->isProject()) ) {
        KMessageBox::sorry( this,
            i18n("You must select a project to add a sub-project to.")
            );
        return;
    }

    // ------------------------------------------------------------------------
    // Display the dialog to the user to get the information on the new
    // sub-project to add.
    // ------------------------------------------------------------------------
    DlgAddEditProject* dlg = new DlgAddEditProject( this );
    dlg->setCaption( i18n("Add sub-project") );
    if (dlg->exec()) {
        KTimeclockListItem* item = new KTimeclockListItem( selected );
        item->setProject( true );
        item->setDescription( dlg->description() );
        emit status( i18n("New sub-project added") );
    }
    delete dlg;
}

// ----------------------------------------------------------------------------
// Function:    addTask ()
// ----------------------------------------------------------------------------
// Prompts the user for information on a new task to add, and adds the task to
// the list as a child of the currently selected project/sub-project.
// ----------------------------------------------------------------------------
void KTimeclock::addTask ()
{
    // ------------------------------------------------------------------------
    // Get the currently selected item and verify that its a project; we can't
    // add tasks to other tasks.  We can, however, add tasks if nothing is
    // selected.
    // ------------------------------------------------------------------------
    KTimeclockListItem* selected = (KTimeclockListItem*)this->currentItem();
    if ( (selected != 0) && (!selected->isProject()) ) {
        KMessageBox::sorry( this,
            i18n("Cannot add tasks underneath other tasks.")
            );
        return;
    }

    // ------------------------------------------------------------------------
    // Show the dialog to the user and get the info on the new task.
    // ------------------------------------------------------------------------
    DlgAddEditTask* dlg = new DlgAddEditTask( this );
    dlg->setCaption( i18n("Add task") );
    if (dlg->exec()) {
        KTimeclockListItem* item;
        if (selected == 0)
            item = new KTimeclockListItem( this );
        else
            item = new KTimeclockListItem( selected );
        item->setDescription( dlg->description() );
        item->setTimeSpent( dlg->timeSpent() );
        emit status( i18n("New task added") );
    }
    delete dlg;
}

// ----------------------------------------------------------------------------
// Function:    editItem ()
// ----------------------------------------------------------------------------
// Prompts the user to edit the information for the currently selected item
// (which can be either a project/sub-project, or a task).  Once edited, the
// item in the list view is updated with the new information.
// ----------------------------------------------------------------------------
void KTimeclock::editItem ()
{
    // ------------------------------------------------------------------------
    // Get the currently selected item that we're supposed to be editing.
    // ------------------------------------------------------------------------
    KTimeclockListItem* selected = (KTimeclockListItem*)this->currentItem();
    if (selected == 0) {
        KMessageBox::sorry( this,
            i18n("You must select an item to edit.")
            );
        return;
    }

    // ------------------------------------------------------------------------
    // Depending on whether the item selected is a project or a task, show an
    // appropriate dialog to the user.
    // ------------------------------------------------------------------------
    if (selected->isProject()) {
        DlgAddEditProject* dlg = new DlgAddEditProject( this );
        dlg->setCaption( i18n("Edit Project") );
        dlg->setDescription( selected->description() );
        dlg->setRate( selected->rate() );
        if (dlg->exec()) {
            selected->setDescription( dlg->description() );
            selected->setRate( dlg->rate() );
        }
        delete dlg;
    }
    else {
        DlgAddEditTask* dlg = new DlgAddEditTask( this );
        dlg->setCaption( i18n("Edit Task") );
        dlg->setDescription( selected->description() );
        dlg->setTimeSpent( selected->timeSpent() );
        dlg->setRate( selected->rate() );
        if (dlg->exec()) {
            selected->setDescription( dlg->description() );
            selected->setRate( dlg->rate() );
            if (dlg->timeSpent() == "00:00:00")
                { selected->setTimeSpent( QString::null ); }
            else
                { selected->setTimeSpent( dlg->timeSpent() ); }
        }
        delete dlg;
    }

    // ------------------------------------------------------------------------
    // Make sure that the rate column is up to date for all child items; any
    // rates which were relative based on this item may require updating.
    // ------------------------------------------------------------------------
    this->_recursiveUpdateRate( selected );

    // ------------------------------------------------------------------------
    // Done editing; emit the signal that shows that we're done.
    // ------------------------------------------------------------------------
    emit status( i18n("Item edit completed") );
}

// ----------------------------------------------------------------------------
// Function:    deleteItem ()
// ----------------------------------------------------------------------------
// Confirms with the user that he/she really wants to delete the currently
// selected item, and then deletes it and all child items from the list view.
// ----------------------------------------------------------------------------
void KTimeclock::deleteItem ()
{
    // ------------------------------------------------------------------------
    // Get the currently selected item that we're supposed to be deleting.
    // ------------------------------------------------------------------------
    KTimeclockListItem* selected = (KTimeclockListItem*)this->currentItem();
    if (selected == 0) {
        KMessageBox::sorry( this,
            i18n("You must select an item to delete.")
            );
        return;
    }

    // ------------------------------------------------------------------------
    // Build up the text for asking the user if they really want to delete this
    // item.
    // ------------------------------------------------------------------------
    QString msg;
    if (selected->isProject()) {
        msg.sprintf(
            i18n("Do you really want to delete '%s' and all sub-projects and tasks?"),
            selected->description().data()
            );
    }
    else {
        msg.sprintf(
            i18n("Do you really want to delete '%s'?"),
            selected->description().data()
            );
    }

    // ------------------------------------------------------------------------
    // Confirm with the user that they really want to delete this item, and
    // then delete it if they confirmed it.
    // ------------------------------------------------------------------------
    int response = KMessageBox::warningYesNo( this, msg );
    if (response == KMessageBox::Yes) {
        delete( selected );
        emit status( i18n("Item deleted") );
    }
}

// ----------------------------------------------------------------------------
// Function:    clearTask ()
// ----------------------------------------------------------------------------
// Clears all session time and total time spent for the currently selected
// task.
// ----------------------------------------------------------------------------
void KTimeclock::clearTask ()
{
    // ------------------------------------------------------------------------
    // Get the currently selected task that we're supposed to be clearing.
    // ------------------------------------------------------------------------
    KTimeclockListItem* selected = (KTimeclockListItem*)this->currentItem();
    if ( (selected == 0) || (selected->isProject()) ) {
        KMessageBox::sorry( this,
            i18n("You must select a task to clear.")
            );
        return;
    }

    // ------------------------------------------------------------------------
    // Clear all of the time associated with this item.
    // ------------------------------------------------------------------------
    if (selected->hasSessionTime()) { selected->setSessionTime( 0 ); }
    if (selected->hasTimeSpent())   { selected->setTimeSpent( 0 ); }
    emit status( i18n("Time cleared for task") );
}

// ----------------------------------------------------------------------------
// Function:    clearAllTasks ()
// ----------------------------------------------------------------------------
// Confirms with the user that he/she really wants to clear the session time
// and total time spent for all of the tasks, and then clears all of the time
// spent.
// ----------------------------------------------------------------------------
void KTimeclock::clearAllTasks ()
{
    // ------------------------------------------------------------------------
    // First verify with the user that we really want to clear the time for all
    // of the items.
    // ------------------------------------------------------------------------
    int response = KMessageBox::questionYesNo( this,
                    i18n("Are you sure you want to clear time for all items?")
                    );

    // ------------------------------------------------------------------------
    // If the user confirmed the clear, clear the time for all of the tasks.
    // ------------------------------------------------------------------------
    if (response == KMessageBox::Yes) {
        this->_recursiveClear( (KTimeclockListItem*)this->firstChild() );
        emit status( i18n("Cleared time for all tasks") );
    }

}

// ----------------------------------------------------------------------------
// Function:    clearSession ()
// ----------------------------------------------------------------------------
// Confirms with the user that he/she really wants to clear the session time
// for all tasks, and then clears the session time.  Note, that this does not
// affect the total time spent for the tasks, only the recording of how much
// time has been spent on the task in this session.
// ----------------------------------------------------------------------------
void KTimeclock::clearSession ()
{
    // ------------------------------------------------------------------------
    // First verify with the user that we really want to clear the session time
    // for all of the items.
    // ------------------------------------------------------------------------
    int response = KMessageBox::questionYesNo( this,
                    i18n("Are you sure you want to clear all session time?")
                    );

    // ------------------------------------------------------------------------
    // If the user confirmed the clear, clear the session time for all of the
    // tasks.
    // ------------------------------------------------------------------------
    if (response == KMessageBox::Yes) {
        // --------------------------------------------------------------------
        // Clear just the session time for all items.
        // --------------------------------------------------------------------
        this->_recursiveClear( (KTimeclockListItem*)this->firstChild(),
                               true, false );

        // --------------------------------------------------------------------
        // Reset our own total session time and update our display.
        // --------------------------------------------------------------------
        _session_time = 0;
        emit sessionTime( KTimeclockTime::asString(_session_time) );
        emit status( i18n("Session time reset") );
    }
}

// ----------------------------------------------------------------------------
// Function:    preferences ()
// ----------------------------------------------------------------------------
// Shows the preferences dialog to the user, and updates our own internal
// values based on the information set in the dialog.
// ----------------------------------------------------------------------------
void KTimeclock::preferences ()
{
    KTimeclockConfig* cfg = new KTimeclockConfig( this );
    if (cfg->exec()) {
        // --------------------------------------------------------------------
        // Reset everything that got information from the preferences dialog.
        // --------------------------------------------------------------------
        this->_setAutosaveTimer();
    }
    delete cfg;
}

void KTimeclock::secondTimerEvent() {
    // Compare the time of the "last timer tick" against the current time,
    // to make sure that we're not losing ticks.
    QTime timeNow = QTime::currentTime();
    int lost_ticks = _last_tick.secsTo( timeNow );
    if (lost_ticks > 1) {
        // Show an error to the user.
        QString msg;
//        QTextOStream(&msg) << "Lost " << lost_ticks << " timer ticks!";
QTextStream(&msg) << "Lost " << lost_ticks << " timer ticks!" << "\n"
  << "_last_tick[" << _last_tick.toString() << "]" << "\n"
  << "currentTime[" << timeNow.toString() << "]" << "\n\n";
_last_tick = QTime::currentTime(); // messagebox is synchronous; update or we show infinitely
        KMessageBox::error( this, msg );
    }

    // Update the last timer tick to be the current time.
    _last_tick = QTime::currentTime();

    // Get the selected item and increment the time spent on it.
    KTimeclockListItem* selected = (KTimeclockListItem*)this->currentItem();
    if (selected != 0) {
        // ----------------------------------------------------------------
        // Only update time for tasks; don't do anything if the item is a
        // project.
        // ----------------------------------------------------------------
        if (!selected->isProject()) {
            // ------------------------------------------------------------
            // Increment the time spent on this one item.
            // ------------------------------------------------------------
            selected->incrementTimeSpent( 1 );
            selected->incrementSessionTime( 1 );
            selected->repaint();

            // ------------------------------------------------------------
            // Increment the total session time that we've marked off
            // against various tasks and update our own display.
            // ------------------------------------------------------------
            _session_time ++;
            emit sessionTime( KTimeclockTime::asString(_session_time) );
        }
    }
}

void KTimeclock::autosaveTimerEvent() {
    this->saveData();
    emit status( i18n("Auto-saved data file") );
}

// ----------------------------------------------------------------------------
// Function:    _dom_to_list (KTimeclockListItem* p_item, QDomElement elem)
// Parameters:  p_item      - Parent list view item to add children to
//              elem        - DOM element to get the information from
// ----------------------------------------------------------------------------
// Recursively loads our timeclock information from an XML DOM tree, into a
// series of list view items.
// ----------------------------------------------------------------------------
void KTimeclock::_dom_to_list (KTimeclockListItem* p_item, QDomElement elem)
{
    // ------------------------------------------------------------------------
    // Get the information that we want out of this node, and create a new list
    // item.
    // ------------------------------------------------------------------------
    KTimeclockListItem* item;
    if (p_item == 0) {
        item = new KTimeclockListItem( this );
    }
    else {
        item = new KTimeclockListItem( p_item );
    }

    // ------------------------------------------------------------------------
    // Figure out whether this node is for a project or not
    // ------------------------------------------------------------------------
    QString name = elem.tagName();
    if (name == "PROJECT") {
        item->setProject( true );
    }

    // ------------------------------------------------------------------------
    // Add in the columns of information into the list item, based on the
    // attributes in the XML element.
    // ------------------------------------------------------------------------
    if (elem.hasAttribute( "NAME" )) {
        item->setDescription( elem.attribute( "NAME" ) );
    }
    if (elem.hasAttribute( "TIMESPENT" )) {
        item->setTimeSpent( elem.attribute( "TIMESPENT" ) );
    }
    if (elem.hasAttribute( "RATE" )) {
        item->setRate( elem.attribute( "RATE" ) );
    }

    // ------------------------------------------------------------------------
    // Add in our child nodes.
    // ------------------------------------------------------------------------
    if (!elem.firstChild().isNull()) {
        this->_dom_to_list( item, elem.firstChild().toElement() );
    }

    // ------------------------------------------------------------------------
    // Then add in our sibling nodes
    // ------------------------------------------------------------------------
    if (!elem.nextSibling().isNull()) {
        this->_dom_to_list( p_item, elem.nextSibling().toElement() );
    }
}

// ----------------------------------------------------------------------------
// Function:    _list_to_dom (QDomDocument doc, QDomElement p_elem,
//                            KTimeclockListItem* item)
// Parameters:  doc         - DOM document to create elements in
//              p_elem      - Parent DOM element to add children to
//              item        - List view item to get the information from
// ----------------------------------------------------------------------------
// Recursively saves out our timeclock information from our list view into an
// XML DOM document.
// ----------------------------------------------------------------------------
void KTimeclock::_list_to_dom (QDomDocument doc, QDomElement p_elem, KTimeclockListItem* item)
{
    // ------------------------------------------------------------------------
    // If we were given a bogus list item, exit early.
    // ------------------------------------------------------------------------
    if (item == 0) { return; }

    // ------------------------------------------------------------------------
    // Create a new XML node for this list item.
    // ------------------------------------------------------------------------
    QDomElement elem;
    if (item->isProject()) {
        elem = doc.createElement( "PROJECT" );
    }
    else {
        elem = doc.createElement( "TASK" );
        if (item->hasTimeSpent()) {
            elem.setAttribute( "TIMESPENT", item->timeSpent() );
        }
    }

    // ------------------------------------------------------------------------
    // Add in some attributes that are common to both projects and tasks.
    // ------------------------------------------------------------------------
    if (item->hasRate()) {
        elem.setAttribute( "RATE", item->rate() );
    }
    if (item->hasDescription()) {
        elem.setAttribute( "NAME", item->description() );
    }

    // ------------------------------------------------------------------------
    // Now that we've got the XML element together, add it to the parent
    // element.
    // ------------------------------------------------------------------------
    p_elem.appendChild( elem );

    // ------------------------------------------------------------------------
    // Make sure we get any child elements taken care of.
    // ------------------------------------------------------------------------
    if (item->firstChild() != 0) {
        this->_list_to_dom( doc, elem,
                            (KTimeclockListItem*)item->firstChild() );
    }

    // ------------------------------------------------------------------------
    // And then take care of any sibling elements we've got
    // ------------------------------------------------------------------------
    if (item->nextSibling() != 0) {
        this->_list_to_dom( doc, p_elem,
                            (KTimeclockListItem*)item->nextSibling() );
    }
}

// ----------------------------------------------------------------------------
// Function:    _recursiveClear (KTimeclockListItem* item, bool session,
//                               bool total)
// Parameters:  item        - List view item to clear time underneath
//              session     - Should we clear the session time?
//              total       - Should we clear the total time spent?
// ----------------------------------------------------------------------------
// Recursively clears the session time and total time spent for the given list
// item and all child items underneath it.  If 'session' is TRUE, session time
// is cleared.  If 'total' is TRUE, total time spent is cleared.
// ----------------------------------------------------------------------------
void KTimeclock::_recursiveClear (KTimeclockListItem* item, bool session, bool total)
{
    // ------------------------------------------------------------------------
    // If we were given a bogus item, exit early.
    // ------------------------------------------------------------------------
    if (item == 0L) { return; }

    // ------------------------------------------------------------------------
    // Clear the time associated with this item.
    // ------------------------------------------------------------------------
    if (!item->isProject()) {
        if (session && item->hasSessionTime()) {
            item->setSessionTime( 0 );
        }
        if (total && item->hasTimeSpent()) {
            item->setTimeSpent( 0 );
        }
    }

    // ------------------------------------------------------------------------
    // Then clear any child items.
    // ------------------------------------------------------------------------
    this->_recursiveClear( (KTimeclockListItem*)item->firstChild(), session, total );

    // ------------------------------------------------------------------------
    // And then clear any sibling items.
    // ------------------------------------------------------------------------
    this->_recursiveClear( (KTimeclockListItem*)item->nextSibling(), session, total );
}

// ----------------------------------------------------------------------------
// Function:    _recursiveUpdateRate (KTimeclockListItem* item)
// Parameters:  item        - List view item to update rates for
// ----------------------------------------------------------------------------
// Recursively updates the rate shown in the list view for the given list view
// item and all child items underneath it.
// ----------------------------------------------------------------------------
void KTimeclock::_recursiveUpdateRate (KTimeclockListItem* item)
{
    // ------------------------------------------------------------------------
    // If we were given a bogus item, exit early.
    // ------------------------------------------------------------------------
    if (item == 0L) { return; }

    // ------------------------------------------------------------------------
    // Force an update on the effective rate for this item.
    // ------------------------------------------------------------------------
    if (item->hasRate()) {
        item->recalculateRate();
    }

    // ------------------------------------------------------------------------
    // Recurse through all child items.
    // ------------------------------------------------------------------------
    this->_recursiveUpdateRate( (KTimeclockListItem*)item->firstChild() );

    // ------------------------------------------------------------------------
    // Recurse through sibling items.
    //
    // This accidentally forces an update on all of the siblings to the
    // original item we were given, but that's not of much concern (it doesn't
    // do anything wrong, it just recalculates something needlessly).
    // ------------------------------------------------------------------------
    this->_recursiveUpdateRate( (KTimeclockListItem*)item->nextSibling() );
}

// ----------------------------------------------------------------------------
// Function:    _setAutosaveTimer ()
// ----------------------------------------------------------------------------
// Resets the autosave timer, based on the information in our configuration
// file for the delay between autosaves.
// ----------------------------------------------------------------------------
void KTimeclock::_setAutosaveTimer ()
{
    if (_autosave_timer == NULL) {
        _autosave_timer = new QTimer(this);
        connect(_autosave_timer, SIGNAL(timeout()), this, SLOT(autosaveTimerEvent()));
    }

    // ------------------------------------------------------------------------
    // Get the number of minutes between auto-saves.
    // ------------------------------------------------------------------------
    KSharedConfigPtr cfg = KGlobal::config();
    KConfigGroup generalGroup(cfg, "General");
    int minutes = generalGroup.readEntry( "autosave" );

    // ------------------------------------------------------------------------
    // If we were given a value to auto-save with, set up a timer to go off
    // every 'n' minutes.
    // ------------------------------------------------------------------------
    _autosave_timer->stop();
    if (minutes > 0) {
        _autosave_timer->start(60000 * minutes);
    }
}

// ----------------------------------------------------------------------------
// Function:    _loadXMLData ()
// Returns:     Did we load information from the data file?
// ----------------------------------------------------------------------------
// Loads our timeclock information from an XML data file.  Returns TRUE if
// we're able to load information from the data file, returning FALSE
// otherwise.
// ----------------------------------------------------------------------------
bool KTimeclock::_loadXMLData ()
{
    // ------------------------------------------------------------------------
    // Get the full path to the XML data file that we're loading
    // ------------------------------------------------------------------------
    QString filename = KGlobal::dirs()->findResource(
                            "appdata", "ktimeclock.xml"
                            );
    QFile file( filename );

    // ------------------------------------------------------------------------
    // If the file doesn't exist or we can't open it, exit early.
    // ------------------------------------------------------------------------
    if (!file.exists()) return false;
    if (!file.open( QIODevice::ReadOnly )) return false;

    // ------------------------------------------------------------------------
    // Create the DOM document, slurp in the XML data file, and get the root
    // element.
    // ------------------------------------------------------------------------
    QDomDocument doc;
    doc.setContent( &file );
    QDomElement root = doc.documentElement();

    // ------------------------------------------------------------------------
    // Do a depth-first traversal through the DOM, creating all of the list
    // items for the timeclock as we go.
    // ------------------------------------------------------------------------
    this->_dom_to_list( 0L, root.firstChild().toElement() );

    // ------------------------------------------------------------------------
    // Done loading up the data, return successfully.
    // ------------------------------------------------------------------------
    return true;
}

// ----------------------------------------------------------------------------
// Function:    _loadTextData ()
// Returns:     Did we load information from the data file?
// ----------------------------------------------------------------------------
// Loads our timeclock information from a text data file (the old KTimeclock
// data file format).  Returns TRUE if we're able to load information from the
// data file, returning FALSE otherwise.
// ----------------------------------------------------------------------------
bool KTimeclock::_loadTextData ()
{
    // ------------------------------------------------------------------------
    // Get the full path to the text data file that we're loading.
    // ------------------------------------------------------------------------
    QString filename = KGlobal::dirs()->findResource(
                            "data", "ktimeclock/ktimeclock.txt"
                            );
    QFile file( filename );

    // ------------------------------------------------------------------------
    // If the file doesn't exist or we can't open it, exit early.
    // ------------------------------------------------------------------------
    if (!file.exists()) return false;
    if (!file.open( QIODevice::ReadOnly )) return false;

    // ------------------------------------------------------------------------
    // Load up the entire file line-by-line into a queue that we can process
    // recursively.
    // ------------------------------------------------------------------------
    QTextStream finstream( &file );
    QQueue<QString> queue;
    queue.setAutoDelete( true );
    while (!finstream.atEnd()) {
        queue.enqueue( new QString( finstream.readLine() ) );
    }
    file.close();

    // ------------------------------------------------------------------------
    // Call off to recursively load the data out of the queue.
    // ------------------------------------------------------------------------
    this->_loadTextDataQueue( queue, NULL, 0 );

    // ------------------------------------------------------------------------
    // Done loading up the data, return successfully.
    // ------------------------------------------------------------------------
    return true;
}

// ----------------------------------------------------------------------------
// Function:    _loadTextDataQueue (QQueue<QString> &queue,
//                                  KTimeclockListItem* parent, int depth)
// Parameters:  queue       - Queue to load the data from
//              parent      - Parent item to create child elements underneath
//              depth       - Depth level of the parent item
// ----------------------------------------------------------------------------
// Recursively loads our timeclock information from a queue of data created by
// '_loadTextData()'.
// ----------------------------------------------------------------------------
void KTimeclock::_loadTextDataQueue (QQueue<QString> &queue, KTimeclockListItem* parent, int depth)
{
    // ------------------------------------------------------------------------
    // Loop forever (we'll know when to kick out later on)
    // ------------------------------------------------------------------------
    while (1) {
        // --------------------------------------------------------------------
        // Get the next item out of the queue.  If there isn't one, kick
        // ourselves out of the loop.
        // --------------------------------------------------------------------
        QString* line = queue.head();
        if (!line) { return; }

        // --------------------------------------------------------------------
        // Get the depth of this item and kick ourselves out if its at a lower
        // depth level than we're at.
        // --------------------------------------------------------------------
        int currdepth = line->contains( '\t' ) - 1;
        if (currdepth <= depth) { return; }

        // --------------------------------------------------------------------
        // Get the info that we need out of this item.
        // --------------------------------------------------------------------
            // ----------------------------------------------------------------
            // Number of seconds spent so far.
            // ----------------------------------------------------------------
        int sec_idx = line->find( '\t' );
        QString tmp = line->mid( 0, sec_idx );
        int seconds = tmp.toInt();
        sec_idx ++;
            // ----------------------------------------------------------------
            // Type; project or task
            // ----------------------------------------------------------------
        int type_idx = line->find( '\t', sec_idx );
        QString type = line->mid( sec_idx, (type_idx - sec_idx) );
            // ----------------------------------------------------------------
            // Description
            // ----------------------------------------------------------------
        int desc_idx = line->findRev( '\t' );
        desc_idx ++;
        QString desc = line->mid( desc_idx, (line->length() - desc_idx) );

        // --------------------------------------------------------------------
        // Free up the memory for this item by removing it from the queue.
        // --------------------------------------------------------------------
        queue.remove();

        // --------------------------------------------------------------------
        // Create a new list item for this project/task.
        // --------------------------------------------------------------------
        KTimeclockListItem* item;
        if (parent == NULL) {
            item = new KTimeclockListItem( this );
        }
        else {
            item = new KTimeclockListItem( parent );
        }

        // --------------------------------------------------------------------
        // Add in all of the data that we read out of this line into the
        // timeclock list item.
        // --------------------------------------------------------------------
        item->setTimeSpent( seconds );
        item->setDescription( desc );
        if (type == "Project") {
            item->setProject( true );
        }

        // --------------------------------------------------------------------
        // Figure out how we're supposed to recurse.
        // --------------------------------------------------------------------
            // ----------------------------------------------------------------
            // If the queue is empty, abort. 
            // ----------------------------------------------------------------
        line = queue.head();
        if (!line) { return; }
            // ----------------------------------------------------------------
            // If its deeper, recurse.  If its shallower, abort.
            // ----------------------------------------------------------------
        int nextdepth = line->contains( '\t' ) - 1;
        if (nextdepth > currdepth) {
            this->_loadTextDataQueue( queue, item, currdepth );
        }
        else if (nextdepth < currdepth) {
            return;
        }
            // ----------------------------------------------------------------
            // Otherwise, its at the same level; loop around and process the
            // next line.
            // ----------------------------------------------------------------
    }
}
