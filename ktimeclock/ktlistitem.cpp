// ----------------------------------------------------------------------------
// Filename:    ktlistitem.cpp
// ----------------------------------------------------------------------------
// $Revision: 1.1.1.1 $
// ----------------------------------------------------------------------------

#include "ktlistitem.h"
#include "kttime.h"
#include <KIconLoader>
#include <QPixmap>

// ----------------------------------------------------------------------------
// Function:    setDefaults ()
// ----------------------------------------------------------------------------
// Sets some default values; used by our constructors.
// ----------------------------------------------------------------------------
void KTimeclockListItem::setDefaults ()
{
    this->setProject( false );
}

// ----------------------------------------------------------------------------
// Function:    setDescription (const QString& desc)
// Parameters:  desc    - Description of this item
// ----------------------------------------------------------------------------
// Sets the description associated with this list item.
// ----------------------------------------------------------------------------
void KTimeclockListItem::setDescription (const QString& desc)
{
    this->setText( COL_DESCRIPTION, desc );
}

// ----------------------------------------------------------------------------
// Function:    description () const
// Returns:     Description of this item
// ----------------------------------------------------------------------------
// Gets the description for this item and returns it.
// ----------------------------------------------------------------------------
QString KTimeclockListItem::description () const
{
    return this->text( COL_DESCRIPTION );
}

// ----------------------------------------------------------------------------
// Function:    setTimeSpent (long timespent)
// Parameters:  timespent   - Time spent on this item
// ----------------------------------------------------------------------------
// Sets the total time spent so far on this list item.  Only works for items
// which are marks as _NOT_ being "projects"; for projects this method does
// nothing.
// ----------------------------------------------------------------------------
void KTimeclockListItem::setTimeSpent (long timespent)
{
    // ------------------------------------------------------------------------
    // Exit early if we're a project.
    // ------------------------------------------------------------------------
    if (this->isProject()) return;

    // ------------------------------------------------------------------------
    // Set the numeric version of the column.
    // ------------------------------------------------------------------------
    QString tmp;
    tmp.setNum( timespent );
    this->setText( COL_TOTAL, tmp );

    // ------------------------------------------------------------------------
    // Convert the number of seconds to an hourly format and set the text
    // version of the column.
    // ------------------------------------------------------------------------
    this->setText( COL_TEXTTOTAL, KTimeclockTime::asString( timespent ) );

    // ------------------------------------------------------------------------
    // Recalculate the earnings for this item.
    // ------------------------------------------------------------------------
    this->recalculateEarnings();
}

// ----------------------------------------------------------------------------
// Function:    setTimeSpent (const QString& timespent)
// Parameters:  timespent   - Time spent on this item
// ----------------------------------------------------------------------------
// Same as 'setTimeSpent()' above, but with different parameters.
//
// The format of 'timespent' provided must be formatted as HH:MM:SS.
// ----------------------------------------------------------------------------
void KTimeclockListItem::setTimeSpent (const QString& timespent)
{
    this->setTimeSpent( KTimeclockTime::asSeconds( timespent ) );
}

// ----------------------------------------------------------------------------
// Function:    timeSpent ()
// Returns:     Time spent on this item
// ----------------------------------------------------------------------------
// Gets the total time spent so far on this item and returns it as a string
// value.
// ----------------------------------------------------------------------------
QString KTimeclockListItem::timeSpent () const
{
    return this->text( COL_TEXTTOTAL );
}

// ----------------------------------------------------------------------------
// Function:    setRate (const QString& rate)
// Parameters:  rate        - Earnings rate for this item
// ----------------------------------------------------------------------------
// Sets the earnings rate for this item to 'rate'.
//
// The value of 'rate' provided is to be formatted as a numeric value; no other
// character values are allowed within this string.
// ----------------------------------------------------------------------------
void KTimeclockListItem::setRate (const QString& rate)
{
    this->setText( COL_RATE, rate );

    // ------------------------------------------------------------------------
    // Recalculate the effective rate for this item.
    // ------------------------------------------------------------------------
    this->recalculateRate();

    // ------------------------------------------------------------------------
    // Recalculate the earnings for this item.
    // ------------------------------------------------------------------------
    this->recalculateEarnings();
}

// ----------------------------------------------------------------------------
// Function:    setRate (double rate)
// Parameters:  rate        - Earnings rate for this item
// ----------------------------------------------------------------------------
// Same as 'setRate()' above, but with different parameters.
// ----------------------------------------------------------------------------
/*
void KTimeclockListItem::setRate (double rate)
{
    QString tmp;
    tmp.setNum( rate );
    this->setRate( tmp );
}
*/

// ----------------------------------------------------------------------------
// Function:    rate ()
// Returns:     Earnings rate for this item
// ----------------------------------------------------------------------------
// Gets the rate for this item and returns it.
// ----------------------------------------------------------------------------
QString KTimeclockListItem::rate () const
{
    return this->text( COL_RATE );
}

// ----------------------------------------------------------------------------
// Function:    effectiveRate () const
// Returns:     Effective earnings rate for this item
// ----------------------------------------------------------------------------
// Determines the effective rate for this list item and returns it to the
// caller.
//
// Note that the "effective" rate for a list item is not necessarily any set
// earnings rate; rates are inherited by child list items.  As such, any child
// items which do not have a set earnings rate inherit the rate of their
// parent.  As well, items with relative earnings rates (e.g. "*5", "/2") are
// calculated against their parents effective earnings rate.
// ----------------------------------------------------------------------------
double KTimeclockListItem::effectiveRate () const
{
    // ------------------------------------------------------------------------
    // If we've been given an explicit rate, see if its considered "relative"
    // to our inherited rate.
    // ------------------------------------------------------------------------
    if (this->hasRate())
    {
        QString r = this->rate();
        if ( (r.left( 1 ) == "*") ||
             (r.left( 1 ) == "/") ||
             (r.left( 1 ) == "+") ||
             (r.left( 1 ) == "-")
           )
        {
            // ----------------------------------------------------------------
            // If we don't have a parent node, return zero.
            // ----------------------------------------------------------------
            if (!this->parent()) return 0.0;

            // ----------------------------------------------------------------
            // Get our parents effective rate (the base rate to work with).
            // ----------------------------------------------------------------
            KTimeclockListItem* par = (KTimeclockListItem*)this->parent();
            double base = par->effectiveRate();

            // ----------------------------------------------------------------
            // Get our modifier value for the rate.
            // ----------------------------------------------------------------
            double modifier = r.mid( 1 ).toDouble();

            // ----------------------------------------------------------------
            // Calculate our relative rate, and return it to the caller.
            // ----------------------------------------------------------------
            if (r.left( 1 ) == "*")
            {
                return (base * modifier);
            }
            else if (r.left( 1 ) == "+")
            {
                return (base + modifier);
            }
            else if (r.left( 1 ) == "-")
            {
                return (base - modifier);
            }
            else if (r.left( 1 ) == "/")
            {
                // ------------------------------------------------------------
                // Check for divide-by-zero possibilities first.
                // ------------------------------------------------------------
                if (modifier == 0.0) return 0.0;

                // ------------------------------------------------------------
                // Return the modified rate.
                // ------------------------------------------------------------
                return (base / modifier);
            }
            else
            {
                qWarning( "SHOULDN'T BE HERE! (KTimeclockListItem::effectiveRate())" );
                return 0.0;
            }
        }
        else
        {
            // ----------------------------------------------------------------
            // Remove any leading '=' character that might be present (is
            // optional).
            // ----------------------------------------------------------------
            if (r.left( 1 ) == "=")
            {
                r.remove( 0, 1 );
            }

            // ----------------------------------------------------------------
            // Return the rate to the caller.
            // ----------------------------------------------------------------
            return r.toDouble();
        }
    }

    // ------------------------------------------------------------------------
    // Otherwise, if we've got a parent list item, return its effective rate.
    // ------------------------------------------------------------------------
    if (this->parent())
    {
        KTimeclockListItem* par = (KTimeclockListItem*)this->parent();
        return par->effectiveRate();
    }

    // ------------------------------------------------------------------------
    // Otherwise, we're a top-level item; return zero.
    // ------------------------------------------------------------------------
    return 0.0;
}

// ----------------------------------------------------------------------------
// Function:    setSessionTime (long sessiontime)
// Parameters:  sessiontime     - Session time spent for this item
// ----------------------------------------------------------------------------
// Sets the time spent in this session on this list item.  Only works for items
// which are marked as _NOT_ being "projects"; for projects this method does
// nothing.
// ----------------------------------------------------------------------------
void KTimeclockListItem::setSessionTime (long sessiontime)
{
    // ------------------------------------------------------------------------
    // Exit early if we're a project.
    // ------------------------------------------------------------------------
    if (this->isProject()) return;

    // ------------------------------------------------------------------------
    // Set the numeric version of the column.
    // ------------------------------------------------------------------------
    QString tmp;
    tmp.setNum( sessiontime );
    this->setText( COL_SESSION, tmp );

    // ------------------------------------------------------------------------
    // Convert the number of seconds to an hourly format and set the text
    // version of the column.
    // ------------------------------------------------------------------------
    this->setText( COL_TEXTSESSION, KTimeclockTime::asString( sessiontime ) );
}

// ----------------------------------------------------------------------------
// Function:    setSessionTime (const QString& sessiontime)
// Parameters:  sessiontime     - Session time spent for this item
// ----------------------------------------------------------------------------
// Same as 'setSessionTime()' above, but with different parameters.
//
// The value of 'sessiontime' provided is to be formatted as HH:MM:SS.
// ----------------------------------------------------------------------------
void KTimeclockListItem::setSessionTime (const QString& sessiontime)
{
    this->setSessionTime( KTimeclockTime::asSeconds( sessiontime ) );
}

// ----------------------------------------------------------------------------
// Function:    sessionTime () const
// Returns:     Session time spent for this item.
// ----------------------------------------------------------------------------
// Gets the time spent on this item in this session and returns it as a string
// value.
// ----------------------------------------------------------------------------
QString KTimeclockListItem::sessionTime () const
{
    return this->text( COL_TEXTSESSION );
}

// ----------------------------------------------------------------------------
// Function:    setEarnings (const QString& earnings)
// Parameters:  earnings    - Total earnings for this item
// ----------------------------------------------------------------------------
// Sets the total earnings so far for this item.
// ----------------------------------------------------------------------------
void KTimeclockListItem::setEarnings (const QString& earnings)
{
    // ------------------------------------------------------------------------
    // If we're a project, skip setting this field
    // ------------------------------------------------------------------------
    if (this->isProject()) return;

    // ------------------------------------------------------------------------
    // Store the earnings value provided.
    // ------------------------------------------------------------------------
    this->setText( COL_EARNINGS, earnings );
}

// ----------------------------------------------------------------------------
// Function:    earnings () const
// Returns:     Total earnings for this item
// ----------------------------------------------------------------------------
// Gets the total earnings so far for this item and returns it.
// ----------------------------------------------------------------------------
QString KTimeclockListItem::earnings () const
{
    return this->text( COL_EARNINGS );
}

// ----------------------------------------------------------------------------
// Function:    recalculateRate ()
// ----------------------------------------------------------------------------
// Recalculates the effective earnings rate for this item.
// ----------------------------------------------------------------------------
void KTimeclockListItem::recalculateRate ()
{
    QString tmp;
    tmp.sprintf( "%0.2f", this->effectiveRate() );
    this->setText( COL_EFFECTIVERATE, tmp );
}

// ----------------------------------------------------------------------------
// Function:    relcalculateEarnings ()
// ----------------------------------------------------------------------------
// Recalculates the earnings for this list item, based on the items effective
// earnings rate.
// ----------------------------------------------------------------------------
void KTimeclockListItem::recalculateEarnings ()
{
    // ------------------------------------------------------------------------
    // Get the total time spent on this item.
    // ------------------------------------------------------------------------
    QString tmp = this->text( COL_TOTAL );
    int timespent = tmp.toInt();

    // ------------------------------------------------------------------------
    // Get the effective earnings rate for this item.
    // ------------------------------------------------------------------------
    double rate = this->effectiveRate ();

    // ------------------------------------------------------------------------
    // Calculate the total amount earned and set that value into ourselves.
    // ------------------------------------------------------------------------
    double total = (rate * (double)timespent) / 3600.0;
    tmp.sprintf( "%0.2f", total );
    this->setEarnings( tmp );
}

// ----------------------------------------------------------------------------
// Function:    setProject (bool isproject)
// Parameters:  isproject       - Is this item a project?
// ----------------------------------------------------------------------------
// Sets a flag stating whether this item is a project or not.  Depending on the
// value provided, we set an appropriate icon to represent this item in the
// tree view.
//
// If TRUE, this item is set to be a project, while if FALSE (the default) it
// is set to be a task.
// ----------------------------------------------------------------------------
void KTimeclockListItem::setProject (bool isproject)
{
    this->_project = isproject;
    if (isproject)
    {
        // --------------------------------------------------------------------
        // Projects get a nice little pixmap.
        // --------------------------------------------------------------------
        this->setPixmap( COL_DESCRIPTION, SmallIcon( "folder" ) );

        // --------------------------------------------------------------------
        // Also clear the session time and total time; they're not shown for
        // project items.
        // --------------------------------------------------------------------
        this->setText( COL_TEXTSESSION, QString() );
        this->setText( COL_TEXTTOTAL, QString() );
    }
    else
    {
        // --------------------------------------------------------------------
        // Tasks get empty pixmaps.
        // --------------------------------------------------------------------
        this->setPixmap( COL_DESCRIPTION, QPixmap() );
    }
}

// ----------------------------------------------------------------------------
// Function:    isProject () const
// ----------------------------------------------------------------------------
// Returns a value stating whether or not this item is a project.
// ----------------------------------------------------------------------------
bool KTimeclockListItem::isProject () const
{
    return this->_project;
}

// ----------------------------------------------------------------------------
// Function:    incrementTimespent (int seconds)
// Parameters:  seconds     - Number of seconds to increment total time by
// ----------------------------------------------------------------------------
// Increments the total time spent on this item by the given number of seconds.
// ----------------------------------------------------------------------------
void KTimeclockListItem::incrementTimeSpent (int seconds)
{
    // ------------------------------------------------------------------------
    // Get the current time spent in seconds and increment it by the given
    // value.
    // ------------------------------------------------------------------------
    int current = this->text( COL_TOTAL ).toInt();
    current += seconds;

    // ------------------------------------------------------------------------
    // Stuff the value back into ourselves.
    // ------------------------------------------------------------------------
    this->setTimeSpent( KTimeclockTime::asString( current ) );
}

// ----------------------------------------------------------------------------
// Function:    incrementSessionTime (int seconds)
// Parameters:  seconds     - Number of seconds to increment session time by
// ----------------------------------------------------------------------------
// Increments the time spent on this item in this session by the given number
// of seconds.
// ----------------------------------------------------------------------------
void KTimeclockListItem::incrementSessionTime (int seconds)
{
    // ------------------------------------------------------------------------
    // Get the current session time in seconds and increment it by the given
    // value.
    // ------------------------------------------------------------------------
    int current = this->text( COL_SESSION ).toInt();
    current += seconds;

    // ------------------------------------------------------------------------
    // Stuff the value back into ourselves.
    // ------------------------------------------------------------------------
    this->setSessionTime( KTimeclockTime::asString( current ) );
}
