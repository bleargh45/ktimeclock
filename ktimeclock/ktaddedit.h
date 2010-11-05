// ----------------------------------------------------------------------------
// Filename:    ktaddedit.h
// ----------------------------------------------------------------------------
// $Revision: 1.1.1.1 $
// ----------------------------------------------------------------------------

#ifndef KTIMECLOCK_ADDEDIT_H
#define KTIMECLOCK_ADDEDIT_H

#include <KDialog>
#include <KComboBox>
#include <KLineEdit>
#include <KDoubleNumInput>
#include <Q3GridLayout>
#include <KIntNumInput>

/**
 * Dialog to allow us to add or edit Projects/Sub-projects within KTimeclock.
 *
 * This dialog shows an entry field for the name of the project, as well as
 * combo/spin boxes to allow the user to edit the rate charged for time spent
 * on this project.
 *
 * @short   Dialog to add/edit Projects/Sub-projects.
 * @author  Graham TerMarsch (graham@howlingfrog.com)
 */
class DlgAddEditProject : public KDialog
{
    Q_OBJECT

    public:
        /**
         * Constructs a new DlgAddEditProject widget.
         *
         * @param   parent  Handle to parent widget
         */
        DlgAddEditProject (QWidget* parent=0);

        /**
         * Destructs the widget.
         */
        virtual ~DlgAddEditProject ();

        /**
         * Sets the description of the project to @p desc.
         *
         * @param   desc    Description of this project
         */
        virtual void setDescription (const QString& desc);

        /**
         * Returns the description of the project.
         *
         * @returns Description of the project
         */
        virtual QString description ();

        /**
         * Sets the current rate for this task to the value provided in
         * @p rate.
         *
         * The format of @p rate should be a floating point number, which is
         * optionally prefixed by an arithmetic operator (e.g. "75.00", "*2",
         * "+50.00").  Acceptable arithmetic operators include =, +, -, *, and
         * /.
         *
         * @param   rate    Rate for this task.
         */
        virtual void setRate (const QString& rate);

        /**
         * Returns the rate for this task, in the format of a floating point
         * number, which is optionally prefixed by an arithmetic operator (e.g.
         * "75.00", "*2", "+50.00").
         *
         * @returns Rate for this task.
         */
        virtual QString rate ();

    protected:
        /**
         * Layout holding all of our widgets.
         */
        Q3GridLayout*    _layout;

        /**
         * Description of the project.
         */
        KLineEdit*      _txtDescription;

        /**
         * Combo-box to hold the rate modifier for the rate for this task.
         */
        KComboBox*      _comboRateModifier;

        /**
         * Spinbox holding the rate for this task.
         */
        KDoubleNumInput* _spinRate;
};

/**
 * Dialog to allow us to add or edit Tasks within KTimeclock.
 *
 * Adds to DlgAddEditProject some extra spin-boxes to allow the user to edit
 * the amount of time spent on this task.
 *
 * @short   Dialog to add/edit Tasks.
 * @author  Graham TerMarsch (graham@howlingfrog.com)
 */
class DlgAddEditTask : public DlgAddEditProject
{
    Q_OBJECT

    public:
        /**
         * Constructs a new DlgAddEditTask widget.
         *
         * @param   parent Handle to parent widget.
         */
        DlgAddEditTask (QWidget* parent=0);

        /**
         * Destructs the widget.
         */
        virtual ~DlgAddEditTask ();

        /**
         * Sets the current time spent on this task to the value provided in
         * @p timespent.
         *
         * The format of @p timespent should be HH:MM:SS.
         *
         * @param   timespent   Time spent on this task.
         */
        virtual void setTimeSpent (const QString& timespent);

        /**
         * Returns the current time spent on this task, in the format of
         * HH:HH:SS.
         *
         * @returns Time spent on this task.
         */
        virtual QString timeSpent ();

    private:
        /**
         * Spinbox holding the number of hours spent on this task.
         */
        KIntNumInput*   _spinHours;

        /**
         * Spinbox holding the number of minutes spent on this task.
         */
        KIntNumInput*   _spinMinutes;

        /**
         * Spinbox holding the number of seconds spent on this task.
         */
        KIntNumInput*   _spinSeconds;
};

#endif
