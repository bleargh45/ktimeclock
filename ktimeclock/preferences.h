// ----------------------------------------------------------------------------
// Filename:    preferences.h
// ----------------------------------------------------------------------------
// $Revision: 1.1.1.1 $
// ----------------------------------------------------------------------------

#ifndef KTIMECLOCK_PREFERENCES_H
#define KTIMECLOCK_PREFERENCES_H

#include <kdialogbase.h>
class QLabel;
class QLineEdit;
class KColorButton;
class KIntNumInput;

/**
 * Preferences dialog for KTimeclock.
 *
 * @short   Preferences dialog for KTimeclock.
 * @author  Graham TerMarsch (graham@howlingfrog.com)
 */
class KTimeclockConfig : public KDialogBase
{
    Q_OBJECT
    public:
        /**
         * Constructor.
         *
         * @param   parent  Handle to parent widget
         * @param   name    Name of this widget
         */
        KTimeclockConfig (QWidget* parent=0, const char* name=0);

        /**
         * Destructor.
         */
        virtual ~KTimeclockConfig ();

    private slots:
        /**
         * Called whenever the user presses the "Ok" button.  Applies all
         * changes made in the preferences dialog and closes the dialog.
         */
        void slotOk ();

        /**
         * Called whenever the user presses the "Cancel" button.  Applies all
         * changes made in the preferences dialog.
         */
        void slotApply ();

        /**
         * Called whenever the report background colour is changed in the
         * "Reports" page of the preferences dialog.  Stores the new background
         * colour for the reports.  Needed as we're using a KColorbutton to do
         * the colour selection.
         *
         * @param   col     New report background color
         */
        void slotReportBackgroundChanged (const QColor& col);

        /**
         * Called whenever the report text colour in changed in the "Reports'
         * page of the preferences dialog.  Stores the new text colour for the
         * reports.  Needed as we're using a KColorButton to do the colour
         * selection.
         *
         * @param   col     New report text color
         */
        void slotReportTextChanged (const QColor& col);

    private:
        /**
         * Makes all of the widgets for the "General" page in the preferences
         * dialog.
         */
        void makeGeneralPage ();

        /**
         * Makes all of the widgets for the "Reports" page in the preferences
         * dialog.
         */
        void makeReportPage ();

        /**
         * Structure for the "General" preferences page and the widgets on that
         * page.
         */
        struct GeneralWidget
        {
            int             idx;
            QLabel*         autosaveLbl;
            KIntNumInput*   autosaveNum;
        };

        /**
         * Structure for the "Reports" preferences page and widgets on that
         * page.
         */
        struct ReportWidget
        {
            int             idx;
            QLabel*         backgroundLbl;
            KColorButton*   backgroundBtn;
            QColor          backgroundCol;
            QLabel*         textLbl;
            KColorButton*   textBtn;
            QColor          textCol;
        };

        /**
         * Pages within our preferences dialog.
         */
        GeneralWidget       _general;
        ReportWidget        _report;
};

#endif
