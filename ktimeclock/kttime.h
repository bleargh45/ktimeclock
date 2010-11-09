// ----------------------------------------------------------------------------
// Filename:    kttime.h
// ----------------------------------------------------------------------------
// $Revision: 1.1.1.1 $
// ----------------------------------------------------------------------------

#ifndef KTIMECLOCK_TIME_H
#define KTIMECLOCK_TIME_H

#include <QString>

/**
 * Helper class to do conversions of time formats from HH:MM:SS to the
 * respective number of hours, minutes, and seconds.  Is able to do conversions
 * in both directions.
 *
 * @short   Time format helper class
 * @author  Graham TerMarsch (graham@howlingfrog.com)
 */
class KTimeclockTime {
    public:
        /**
         * Constructor.
         */
        KTimeclockTime ();

        /**
         * Constructor, with formatted time string value in format of HH:MM:SS.
         *
         * @param   formatted   Formatted time string
         */
        KTimeclockTime (const QString& formatted);

        /**
         * Constructor, with total number of seconds.
         *
         * @param   seconds Total number of seconds.
         */
        KTimeclockTime (long seconds);

        /**
         * Destructor.
         */
        ~KTimeclockTime ();

        /**
         * Returns the time as the total number of seconds.
         *
         * @returns Total number of seconds
         */
        long asSeconds ();

        /**
         * Returns the time as a string formatted as HH:MM:SS.
         *
         * @returns Formatted time string
         */
        QString asString ();

        /**
         * Sets the number of hours to @p hours.
         *
         * @param   hours   Hours
         * @see     hours()
         */
        void hours (int hours);

        /**
         * Returns the number of hours in the given time.
         *
         * @returns Hours
         * @see     hours(int)
         */
        int hours () const;

        /**
         * Sets the number of minutes to @p minutes.
         *
         * @param   minutes Minutes
         * @see     minutes()
         */
        void minutes (int minutes);

        /**
         * Returns the number of minutes in the given time.
         *
         * @returns Minutes
         * @see     minutes(int)
         */
        int minutes () const;

        /**
         * Sets the number of seconds to @p seconds.
         *
         * @param   seconds Seconds
         * @see     seconds()
         */
        void seconds (int seconds);

        /**
         * Returns the number of seconds in the given time.
         *
         * @returns Seconds
         * @see     seconds(int)
         */
        int seconds () const;

        /**
         * Helper method to do immediate conversion of a formatted time string
         * to the respective total number of seconds that it represents.
         *
         * @param   formatted   Formatted time string
         * @returns Total number of seconds
         */
        static long asSeconds (const QString& formatted);

        /**
         * Helper method to do immediate conversion from a total number of
         * seconds to its respective formatted time string.
         *
         * @param   seconds Total number of seconds
         * @returns Formatted time string
         */
        static QString asString (long seconds);

    private:
        /**
         * Regenerates our cached values of "total time in seconds" and
         * "formatted time string".
         */
        void _genCache ();

        /**
         * Internal values holding the hours, minutes, and seconds.
         */
        int     _hours;
        int     _minutes;
        int     _seconds;

        /**
         * Cached values of the "formatted time string" and "total time in
         * seconds", as well as a "dirty flag" to state whether or not the
         * cached values need to be recalculated.
         */
        long    _cacheSeconds;
        QString _cacheString;
        bool    _dirty;
};

#endif
