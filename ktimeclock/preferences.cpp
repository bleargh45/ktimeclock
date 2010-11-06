// ----------------------------------------------------------------------------
// Filename:    preferences.cpp
// ----------------------------------------------------------------------------
// $Revision: 1.1.1.1 $
// ----------------------------------------------------------------------------

#include "preferences.h"
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <Q3GridLayout>
#include <Q3Frame>
#include <KColorButton>
#include <KConfig>
#include <KGlobal>
#include <KIconLoader>
#include <KIconTheme>
#include <KLocale>
#include <KNumInput>

// ----------------------------------------------------------------------------
// Function:    KTimeclockConfig (QWidget* parent=0)
// Parameters:  parent      - Handle to parent widget
// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
KTimeclockConfig::KTimeclockConfig (QWidget* parent)
    : KDialog(IconList, i18n("Configure"), Help|Apply|Ok|Cancel, Ok, parent, true, true )
{
// UNFINISHED -> Doesn't set up the default 'help' page.
    this->makeGeneralPage();

    this->setMinimumHeight( 300 );
}

// ----------------------------------------------------------------------------
// Function:    ~KTimeclockConfig ()
// ----------------------------------------------------------------------------
// Destructor.
// ----------------------------------------------------------------------------
KTimeclockConfig::~KTimeclockConfig ()
{
}

// ----------------------------------------------------------------------------
// Function:    slotOk ()
// ----------------------------------------------------------------------------
// Called whenever the user presses the "Ok" button.  Applies all changes made
// in the preferences dialog and closes the dialog.
// ----------------------------------------------------------------------------
void KTimeclockConfig::slotOk ()
{
    this->slotApply();
    accept();
}

// ----------------------------------------------------------------------------
// Function:    slotApply ()
// ----------------------------------------------------------------------------
// Called whenever the user presses the "Cancel" button.  Applies all changes
// made in the preferences dialog.
// ----------------------------------------------------------------------------
void KTimeclockConfig::slotApply ()
{
    // ------------------------------------------------------------------------
    // Get a handle to the config object.
    // ------------------------------------------------------------------------
    KSharedConfigPtr cfg = KGlobal::config();

    // ------------------------------------------------------------------------
    // "General" options.
    // ------------------------------------------------------------------------
    KConfigGroup generalGroup( cfg, "General" );
    generalGroup->writeEntry( "autosave", _general.autosaveNum->value() );

    // ------------------------------------------------------------------------
    // Done saving, sync the changes to disk.
    // ------------------------------------------------------------------------
    cfg->sync();
}

// ----------------------------------------------------------------------------
// Function:    makeGeneralPage ()
// ----------------------------------------------------------------------------
// Makes all of the widgets for the "General" page in the preferences dialog.
// ----------------------------------------------------------------------------
void KTimeclockConfig::makeGeneralPage ()
{
    // ------------------------------------------------------------------------
    // Create the frame for the page and hold onto its index.
    // ------------------------------------------------------------------------
    Q3Frame* page = addPage(
                    i18n("General"),
                    i18n("General options"),
                    KGlobal::iconLoader()->loadIcon( "misc",
                        KIcon::NoGroup, KIcon::SizeMedium )
                    );
    _general.idx = pageIndex( page );

    // ------------------------------------------------------------------------
    // Create a layout within the page to manage all of the widgets we're going
    // to create.
    // ------------------------------------------------------------------------
    Q3GridLayout* grid = new Q3GridLayout( page, 4, 2, 0, -1, "grid" );

    // ------------------------------------------------------------------------
    // Get a handle to the config object we're pulling values out of.
    // ------------------------------------------------------------------------
    KSharedConfigPtr cfg = KGlobal::config();
    KConfigGroup generalGroup(cfg, "General");

    // ------------------------------------------------------------------------
    // Create the widgets.
    // ------------------------------------------------------------------------
        // --------------------------------------------------------------------
        // Auto-saving
        // --------------------------------------------------------------------
    _general.autosaveLbl = new QLabel( page, "_general.autosaveLbl" );
    _general.autosaveLbl->setText( i18n("Autosave every 'n' minutes (0 disables)" ) );
    grid->addWidget( _general.autosaveLbl, 0, 0 );

    _general.autosaveNum = new KIntNumInput( page );
    _general.autosaveNum->setValue( generalGroup->readEntry( "autosave", 5 ) );
    grid->addWidget( _general.autosaveNum, 0, 1 );
}
