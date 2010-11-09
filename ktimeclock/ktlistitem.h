// ----------------------------------------------------------------------------
// Filename:    ktlistitem.h
// ----------------------------------------------------------------------------
// $Revision: 1.1.1.1 $
// ----------------------------------------------------------------------------

#ifndef KTIMECLOCK_LISTITEM_H
#define KTIMECLOCK_LISTITEM_H

#include <Q3ListView>

/**
 * Derived list view item class, with methods provided to deal with the columns
 * in the list item on a named basis (rather than an index offset).
 *
 * @short   Custom list view class for KTimeclock.
 * @author  Graham TerMarsh (graham@howlingfrog.com)
 */
class KTimeclockListItem : public Q3ListViewItem {
    public:
        /**
         * Constructor with QListView parent.
         *
         * @param   parent  QListView parent
         */
        KTimeclockListItem (Q3ListView* parent)
            : Q3ListViewItem(parent)
        {
            this->setDefaults();
        }

        /**
         * Constructor with KTimeclockListItem parent.
         *
         * @param   parent  KTimeclockListItem parent
         */
        KTimeclockListItem (KTimeclockListItem* parent)
            : Q3ListViewItem(parent)
        {
            this->setDefaults();
        }

        /**
         * Sets some default values; used by our constructors.
         */
        void setDefaults ();

        /**
         * Sets the description associated with this list item.
         *
         * @param   desc    Description of this item
         * @see     description()
         * @see     hasDescription()
         */
        void setDescription (const QString& desc);

        /**
         * Gets the description for this item and returns it.
         *
         * @returns Description of this item
         * @see     setDescription(const QString&)
         * @see     hasDescription()
         */
        QString description () const;

        /**
         * Returns a flag stating whether or not we've been given a value for
         * the description of this item.
         *
         * @returns Has a description been set for this item?
         * @see     setDescription(const QString&)
         * @see     description()
         */
        bool hasDescription () const
        {
            return !this->text(COL_DESCRIPTION).isNull();
        }

        /**
         * Sets the total time spent so far on this list item.  Only works for
         * items which are marked as _NOT_ being "projects"; for projects this
         * method does nothing.
         *
         * @param   timespent   Time spent on this item
         * @see     setTimeSpent(const QString&)
         * @see     timeSpent()
         * @see     hasTimeSpent()
         */
        void setTimeSpent (long timespent);

        /**
         * Same as @ref setTimeSpent() above, but with different parameters.
         *
         * The value @p timespent provided must be formatted as HH:MM::SS.
         *
         * @param   timespent   Time spent on this item
         * @see     setTimeSpent(long)
         * @see     timeSpent()
         * @see     hasTimeSpent()
         */
        void setTimeSpent (const QString& timespent);

        /**
         * Gets the total time spent so far on this item and returns it as a
         * string value.
         *
         * @returns Time spent on this item
         * @see     setTimeSpent(long)
         * @see     setTimeSpent(const QString&)
         * @see     hasTimeSpent()
         */
        QString timeSpent () const;

        /**
         * Returns a flag stating whether or not we've been given a value for
         * the total time spent on this item.
         *
         * @returns Has the time spent been set for this item?
         * @see     setTimeSpent(long)
         * @see     setTimeSpent(const QString&)
         * @see     timeSpent()
         */
        bool hasTimeSpent () const
        {
            return !this->text(COL_TOTAL).isNull();
        }

        /**
         * Sets the earnings rate for the given item to @p rate.
         *
         * The value of @p rate provided is to be formatted as a numeric value;
         * no other character values are allowed within this string.
         *
         * @param   rate    Earnings rate for this item
         * @see     rate()
         * @see     hasRate()
         */
        void setRate (const QString& rate);

        /**
         * Same as @ref setRate() above, but with different parameters.
         *
         * @param   rate    Earnings rate for this item
         */
//        void setRate (double rate);

        /**
         * Gets the rate for this item and returns it.
         *
         * @returns Earnings rate for this item
         * @see     setRate(const QString&)
         * @see     hasRate()
         */
        QString rate () const;

        /**
         * Returns a flag stating whether or not we've been given an earnings
         * rate for this item.
         *
         * @returns Has an earnings rate been set for this item
         * @see     setRate(const QString&)
         * @see     rate()
         */
        bool hasRate () const
        {
            return !this->text(COL_RATE).isNull();
        }

        /**
         * Determines the effective rate for this list item and returns it to
         * the caller.
         *
         * Note that the "effective" rate for a list item is not necessarily
         * any set earnings rate; rates are inherited by child list items.  As
         * such, any child items which do not have a set earnings rate inherit
         * the rate of their parent.  As well, items with relative earnings
         * rates (e.g. "*5", "/2") are calculated against their parents
         * effective earnings rate.
         *
         * @returns Effective earnings rate for this item
         */
        double effectiveRate () const;

        /**
         * Sets the time spent in this session on this list item.  Only works
         * for items which are marked as _NOT_ being "projects"; for projects
         * this method does nothing.
         *
         * @param   sessiontime Session time spent for this item
         * @see     setSessionTime(const QString&)
         * @see     sessionTime()
         * @see     hasSessionTime()
         */
        void setSessionTime (long sessiontime);

        /**
         * Same as @ref setSessionTime() above, but with different parameters.
         *
         * The value of @p sessiontime provided is to be formatted as
         * HH:MM:SS.
         *
         * @param   sessiontime Session time spent for this item.
         * @see     setSessionTime(long)
         * @see     sessionTime()
         * @see     hasSessionTime()
         */
        void setSessionTime (const QString& sessiontime);

        /**
         * Gets the time spent on this item in this session and returns it as a
         * string value.
         *
         * @returns Session time spent for this item.
         * @see     setSessionTime(long)
         * @see     setSessionTime(const QString&)
         * @see     hasSessionTime()
         */
        QString sessionTime () const;

        /**
         * Returns a flag stating whether or not we've been given a value for
         * the time spent on this item in this session.
         *
         * @returns Has the session time been set for this item?
         * @see     setSessionTime(long)
         * @see     setSessionTime(const QString&)
         * @see     sessionTime()
         */
        bool hasSessionTime () const
        {
            return !this->text(COL_SESSION).isNull();
        }

        /**
         * Sets the total earnings so far for this item.
         *
         * @param   earnings    Total earnings for this item
         * @see     earnings()
         * @see     hasEarnings()
         */
        void setEarnings (const QString& earnings);

        /**
         * Gets the total earnings so far for this item and returns it.
         *
         * @returns Total earnings for this item
         * @see     setEarnings(const QString&)
         * @see     hasEarnings()
         */
        QString earnings () const;

        /**
         * Returns a flag stating whether or not we've been given a value for
         * the earnings earned for this item.
         *
         * @returns Has the earnings been set for this item?
         * @see     setEarnings(const QString&)
         * @see     earnings()
         */
        bool hasEarnings () const
        {
            return !this->text(COL_EARNINGS).isNull();
        }

        /**
         * Recalculates the effective earnings rate for this list item.
         */
        void recalculateRate ();

        /**
         * Recalculates the earnings for this list item, based on the items
         * effective earnings rate.
         */
        void recalculateEarnings ();

        /**
         * Sets a flag stating whether this item is a project or not.
         * Depending on the value provided, we set an appropriate icon to
         * represent this item in the tree view.
         *
         * If TRUE, this item is set to be a project, while if FALSE (the
         * default) it is set to be a task.
         *
         * @param   isproject   Is this item a project?
         * @see     isProject()
         */
        void setProject (bool isproject);

        /**
         * Returns a value stating whether or not this item is a project.
         * Returns TRUE if this item is a project, returning FALSE if its a
         * task.
         *
         * @returns Is this item a project?
         * @see     setProject(bool)
         */
        bool isProject () const;

        /**
         * Increments the total time spent on this item by the given number of
         * seconds.
         *
         * @param   seconds Number of seconds to increment total time by
         */
        void incrementTimeSpent (int seconds);

        /**
         * Increments the time spent on this item in this session by the given
         * number of seconds.
         *
         * @param   seconds Number of seconds to increment session time by
         */
        void incrementSessionTime (int seconds);

    private:
        /**
         * Enumerate our columns.
         */
        enum COLUMN { COL_DESCRIPTION=0, COL_TEXTSESSION, COL_TEXTTOTAL, COL_EFFECTIVERATE, COL_EARNINGS, COL_SESSION, COL_RATE, COL_TOTAL };

        /**
         * Flag stating whether or not this item is a project.
         */
        bool _project;
};

#endif
