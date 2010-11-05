// ----------------------------------------------------------------------------
// Filename:    kttime.cpp
// ----------------------------------------------------------------------------
// $Revision: 1.1.1.1 $
// ----------------------------------------------------------------------------

#include "kttime.h"

// ----------------------------------------------------------------------------
// Function:    KTimeclockTime ()
// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
KTimeclockTime::KTimeclockTime ()
    : _hours(0), _minutes(0), _seconds(0), _dirty(true)
{
}

// ----------------------------------------------------------------------------
// Function:    KTimeclockTime (const QString& formatted)
// Parameters:  formatted   - Formatted time string
// ----------------------------------------------------------------------------
// Constructor, with formatted time string value in format of HH:MM:SS.
// ----------------------------------------------------------------------------
KTimeclockTime::KTimeclockTime (const QString& formatted)
    : _dirty(true)
{
    int length = 0;
    int offset = 0;
    QString tmp;

    // ------------------------------------------------------------------------
    // Get the hours.
    // ------------------------------------------------------------------------
    length  = formatted.find( ':', offset ) - offset;
    tmp     = formatted.mid( offset, length );
    _hours  = tmp.toInt();
    offset += length + 1;

    // ------------------------------------------------------------------------
    // Get the minutes.
    // ------------------------------------------------------------------------
    length   = formatted.find( ':', offset ) - offset;
    tmp      = formatted.mid( offset, length );
    _minutes = tmp.toInt();
    offset  += length + 1;

    // ------------------------------------------------------------------------
    // Get the seconds.
    // ------------------------------------------------------------------------
    tmp = formatted.mid( offset );
    _seconds = tmp.toInt();
}

// ----------------------------------------------------------------------------
// Function:    KTimeclockTime (long seconds)
// Parameters:  seconds     - Total number of seconds
// ----------------------------------------------------------------------------
// Constructor, with total number of seconds.
// ----------------------------------------------------------------------------
KTimeclockTime::KTimeclockTime (long seconds)
    : _dirty(true)
{
    // ------------------------------------------------------------------------
    // Get the hours.
    // ------------------------------------------------------------------------
    _hours = seconds / 3600;
    seconds %= 3600;

    // ------------------------------------------------------------------------
    // Get the minutes.
    // ------------------------------------------------------------------------
    _minutes = seconds / 60;
    seconds %= 60;

    // ------------------------------------------------------------------------
    // Get the seconds.
    // ------------------------------------------------------------------------
    _seconds = seconds;
}

// ----------------------------------------------------------------------------
// Function:    ~KTimeClockTime ()
// ----------------------------------------------------------------------------
// Destructor.
// ----------------------------------------------------------------------------
KTimeclockTime::~KTimeclockTime ()
{
}

// ----------------------------------------------------------------------------
// Function:    asSeconds ()
// Returns:     Total number of seconds
// ----------------------------------------------------------------------------
// returns the time as the total number of seconds.
// ----------------------------------------------------------------------------
long KTimeclockTime::asSeconds ()
{
    if (_dirty)
    {
        this->_genCache();
    }
    return _cacheSeconds;
}

// ----------------------------------------------------------------------------
// Function:    asString ()
// Returns:     Formatted time string
// ----------------------------------------------------------------------------
// Returns the time as a string formatted as HH:MM:SS.
// ----------------------------------------------------------------------------
QString KTimeclockTime::asString ()
{
    if (_dirty)
    {
        this->_genCache();
    }
    return _cacheString;
}

// ----------------------------------------------------------------------------
// Function:    hours (int hours)
// Parameters:  hours   - Hours
// ----------------------------------------------------------------------------
// Sets the number of hours to 'hours'
// ----------------------------------------------------------------------------
void KTimeclockTime::hours (int hours)
{
    _hours = hours;
    _dirty = true;
}

// ----------------------------------------------------------------------------
// Function:    hours () const
// Returns:     Hours
// ----------------------------------------------------------------------------
// Returns the number of hours in the given time.
// ----------------------------------------------------------------------------
int KTimeclockTime::hours () const
{
    return _hours;
}

// ----------------------------------------------------------------------------
// Function:    minutes (int minutes)
// Parameters:  minutes     - Minutes
// ----------------------------------------------------------------------------
// Sets the number of minutes to 'minutes'.
// ----------------------------------------------------------------------------
void KTimeclockTime::minutes (int minutes)
{
    _minutes = minutes;
    _dirty = true;
}

// ----------------------------------------------------------------------------
// Function:    minutes () const
// Returns:     Minutes
// ----------------------------------------------------------------------------
// Returns the number of minutes in the given time.
// ----------------------------------------------------------------------------
int KTimeclockTime::minutes () const
{
    return _minutes;
}

// ----------------------------------------------------------------------------
// Function:    seconds (int seconds)
// Parameters:  seconds     - Seconds
// ----------------------------------------------------------------------------
// Sets the number of seconds to 'seconds'.
// ----------------------------------------------------------------------------
void KTimeclockTime::seconds (int seconds)
{
    _seconds = seconds;
    _dirty = true;
}

// ----------------------------------------------------------------------------
// Function:    seconds () const
// Returns:     Seconds
// ----------------------------------------------------------------------------
// Returns the number of seconds in the given time.
// ----------------------------------------------------------------------------
int KTimeclockTime::seconds () const
{
    return _seconds;
}

// ----------------------------------------------------------------------------
// Function:    _genCache ()
// ----------------------------------------------------------------------------
// Regenerates our cached values of "total time in seconds" and "formatted time
// string".
// ----------------------------------------------------------------------------
void KTimeclockTime::_genCache ()
{
    // ------------------------------------------------------------------------
    // First, re-generate the total number of seconds.
    // ------------------------------------------------------------------------
    _cacheSeconds = _seconds + (_minutes * 60) + (_hours * 3600);

    // ------------------------------------------------------------------------
    // Then, re-generate the string representation.
    // ------------------------------------------------------------------------
    _cacheString.sprintf( "%02d:%02d:%02d", _hours, _minutes, _seconds );

    // ------------------------------------------------------------------------
    // All done, we're not "dirty" any more.
    // ------------------------------------------------------------------------
    _dirty = false;
}

// ----------------------------------------------------------------------------
// Function:    KTimeclockTime::asSeconds (const QString& formatted)
// Parameters:  formatted   - Formatted time string
// Returns:     Total number of seconds
// ----------------------------------------------------------------------------
// Helper method to do immediate conversion of a formatted time string to the
// respective total number of seconds that it represents.
// ----------------------------------------------------------------------------
long KTimeclockTime::asSeconds (const QString& formatted)
{
    KTimeclockTime mytime( formatted );
    return mytime.asSeconds();
}

// ----------------------------------------------------------------------------
// Function:    KTimeclockTime::asString (long seconds)
// Parameters:  seconds     - Total number of seconds
// Returns:     Formatted time string
// ----------------------------------------------------------------------------
// Helper method to do immediate conversion from a total number of seconds to
// its respective formatted time string.
// ----------------------------------------------------------------------------
QString KTimeclockTime::asString (long seconds)
{
    KTimeclockTime mytime( seconds );
    return mytime.asString();
}
