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
// Function:    KTimeclockConfig (QWidget* parent=0, const char* name=0)
// Parameters:  parent      - Handle to parent widget
//              name        - Name of this widget
// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
KTimeclockConfig::KTimeclockConfig (QWidget* parent, const char* name)
    : KDialog(IconList, i18n("Configure"), Help|Apply|Ok|Cancel, Ok, parent, name, true, true )
{
// UNFINISHED -> Doesn't set up the default 'help' page.
    this->makeGeneralPage();
    this->makeReportPage();

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
    KConfig* cfg = KGlobal::config();

    // ------------------------------------------------------------------------
    // "General" options.
    // ------------------------------------------------------------------------
    cfg->setGroup( "General" );
    cfg->writeEntry( "autosave", _general.autosaveNum->value() );

    // ------------------------------------------------------------------------
    // "Report" options.
    // ------------------------------------------------------------------------
    cfg->setGroup( "Report" );
    cfg->writeEntry( "background", _report.backgroundCol.name() );
    cfg->writeEntry( "text", _report.textCol.name() );

    // ------------------------------------------------------------------------
    // Done saving, sync the changes to disk.
    // ------------------------------------------------------------------------
    cfg->sync();
}

// ----------------------------------------------------------------------------
// Function:    slotReportBackgroundChanged (const QColor& col)
// Parameters:  col         - New report background color
// ----------------------------------------------------------------------------
// Called whenever the report background colour is changed in the "Reports"
// page of the preferences dialog.  Stores the new background colour for the
// reports.  Needed as we're using a KColorbutton to do the colour selection.
// ----------------------------------------------------------------------------
void KTimeclockConfig::slotReportBackgroundChanged (const QColor& col)
{
    _report.backgroundCol = col;
}

// ----------------------------------------------------------------------------
// Function:    slotReportTextChanged (const QColor& col)
// Parameters:  col         - New report text color
// ----------------------------------------------------------------------------
// Called whenever the report text colour in changed in the "Reports' page of
// the preferences dialog.  Stores the new text colour for the reports.  Needed
// as we're using a KColorButton to do the colour selection.
// ----------------------------------------------------------------------------
void KTimeclockConfig::slotReportTextChanged (const QColor& col)
{
    _report.textCol = col;
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
    KConfig* cfg = KGlobal::config();
    cfg->setGroup( "General" );

    // ------------------------------------------------------------------------
    // Create the widgets.
    // ------------------------------------------------------------------------
        // --------------------------------------------------------------------
        // Auto-saving
        // --------------------------------------------------------------------
    _general.autosaveLbl = new QLabel( page, "_general.autosaveLbl" );
    _general.autosaveLbl->setText( i18n("Autosave every 'n' minutes (0 disables)" ) );
    grid->addWidget( _general.autosaveLbl, 0, 0 );

    _general.autosaveNum = new KIntNumInput( page, "_general.autosaveNum" );
    _general.autosaveNum->setValue( cfg->readNumEntry( "autosave" ) );
    grid->addWidget( _general.autosaveNum, 0, 1 );
}

// ----------------------------------------------------------------------------
// Function:    makeReportPage ()
// ----------------------------------------------------------------------------
// Makes all of the widgets for the "Reports" page in the preferences dialog.
// ----------------------------------------------------------------------------
void KTimeclockConfig::makeReportPage ()
{
    // ------------------------------------------------------------------------
    // Create the frame for the page and hold onto its index.
    // ------------------------------------------------------------------------
    Q3Frame* page = addPage(
                    i18n("Reports"),
                    i18n("Report colours"),
                    KGlobal::iconLoader()->loadIcon( "appearance",
                        KIcon::NoGroup, KIcon::SizeMedium )
                    );
    _report.idx = pageIndex( page );

    // ------------------------------------------------------------------------
    // Create a layout within the page to manage all of the widgets we're going
    // to create.
    // ------------------------------------------------------------------------
    Q3GridLayout* grid = new Q3GridLayout( page, 4, 2, 0, -1, "grid" );

    // ------------------------------------------------------------------------
    // Get a handle to the config object we're pulling values out of.
    // ------------------------------------------------------------------------
    KConfig* cfg = KGlobal::config();
    cfg->setGroup( "Report" );

    // ------------------------------------------------------------------------
    // Create the widgets.
    // ------------------------------------------------------------------------
        // --------------------------------------------------------------------
        // Report background colour
        // --------------------------------------------------------------------
    _report.backgroundLbl = new QLabel( page, "_report.backgroundLbl" );
    _report.backgroundLbl->setText( i18n("Background") );
    grid->addWidget( _report.backgroundLbl, 0, 0 );

    _report.backgroundBtn = new KColorButton( page, "_report.backgroundBtn" );
    connect( _report.backgroundBtn, SIGNAL(changed(const QColor&)),
             this, SLOT(slotReportBackgroundChanged(const QColor&)) );
    _report.backgroundBtn->setColor( cfg->readEntry("background", "#FFFFFF") );
    grid->addWidget( _report.backgroundBtn, 0, 1 );
        // --------------------------------------------------------------------
        // Report text colour
        // --------------------------------------------------------------------
    _report.textLbl = new QLabel( page, "_report.textLbl" );
    _report.textLbl->setText( i18n("Text") );
    grid->addWidget( _report.textLbl, 1, 0 );

    _report.textBtn = new KColorButton( page, "_report.textBtn" );
    connect( _report.textBtn, SIGNAL(changed(const QColor&)),
             this, SLOT(slotReportTextChanged(const QColor&)) );
    _report.textBtn->setColor( cfg->readEntry("text", "#000000") );
    grid->addWidget( _report.textBtn, 1, 1 );
}
