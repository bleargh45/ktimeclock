// ----------------------------------------------------------------------------
// Filename:    preferences.h
// ----------------------------------------------------------------------------
// $Revision: 1.1.1.1 $
// ----------------------------------------------------------------------------

#ifndef KTIMECLOCK_PREFERENCES_H
#define KTIMECLOCK_PREFERENCES_H

#include <KDialog>
#include <QLabel>
#include <KColorButton>
#include <KIntNumInput>

/**
 * Preferences dialog for KTimeclock.
 *
 * @short   Preferences dialog for KTimeclock.
 * @author  Graham TerMarsch (graham@howlingfrog.com)
 */
class KTimeclockConfig : public KDialog
{
    Q_OBJECT
    public:
        /**
         * Constructor.
         *
         * @param   parent  Handle to parent widget
         */
        KTimeclockConfig (QWidget* parent=0);

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

    private:
        /**
         * Makes all of the widgets for the "General" page in the preferences
         * dialog.
         */
        void makeGeneralPage ();

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
         * Pages within our preferences dialog.
         */
        GeneralWidget       _general;
};

#endif
