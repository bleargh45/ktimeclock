// ----------------------------------------------------------------------------
// Filename:    ktaddedit.cpp
// ----------------------------------------------------------------------------
// $Revision: 1.2 $
// ----------------------------------------------------------------------------

#include "ktaddedit.h"
#include "kttime.h"
#include <KLineEdit>
#include <KNumInput>
#include <QLayout>
#include <QLabel>
#include <QRegExp>
#include <KLocale>
#include <KComboBox>

// ----------------------------------------------------------------------------
// Function:    DlgAddEditProject (QWidget* parent=0)
// Parameters:  parent      - Handle to parent widget
// ----------------------------------------------------------------------------
// Constructs a new DlgAddEditProject widget.
// ----------------------------------------------------------------------------
DlgAddEditProject::DlgAddEditProject (QWidget* parent)
    : KDialog(parent)
{
    this->setModal( true );
    this->setButtons( KDialog::Ok | KDialog::Cancel );

    // ------------------------------------------------------------------------
    // Create a widget that we can use to lay everything out on.
    // ------------------------------------------------------------------------
    QWidget* page = new QWidget( this );
    this->setMainWidget( page );

    // ------------------------------------------------------------------------
    // Layout widget so that we can lay out everything nice and neat.
    // ------------------------------------------------------------------------
    _layout = new QGridLayout( page );

    // ------------------------------------------------------------------------
    // Text editing widget; description of the project/task.
    // ------------------------------------------------------------------------
    QLabel* lblDescription = new QLabel( page );
    lblDescription->setText( i18n("Description:") );
    _layout->addWidget( lblDescription, 0, 0, Qt::AlignRight );

    _txtDescription = new KLineEdit( page );
    _txtDescription->setMinimumWidth( this->fontMetrics().maxWidth() * 20 );
    _layout->addWidget( _txtDescription, 0, 1, 1, 3, Qt::AlignLeft );

    // ------------------------------------------------------------------------
    // Drop/Spin box for the rate.
    // ------------------------------------------------------------------------
    QLabel* lblRate = new QLabel( page );
    lblRate->setText( i18n("Rate:") );
    _layout->addWidget( lblRate, 1, 0, Qt::AlignRight );

    _comboRateModifier = new KComboBox( page );
    _comboRateModifier->addItem( i18n("Parent's rate") );
    _comboRateModifier->addItem( "=" );
    _comboRateModifier->addItem( "+" );
    _comboRateModifier->addItem( "-" );
    _comboRateModifier->addItem( "*" );
    _comboRateModifier->addItem( "/" );
    _layout->addWidget( _comboRateModifier, 1, 1, Qt::AlignRight );

    _spinRate = new KDoubleNumInput( page );
    _spinRate->setRange( 0.0, 99999.99, 1.0, false );
    _layout->addWidget( _spinRate, 1, 2, Qt::AlignLeft );

    // ------------------------------------------------------------------------
    // Set our default geometry.
    // ------------------------------------------------------------------------
    this->resize( 450, 140 );

    // ------------------------------------------------------------------------
    // Set the default focus to the project/task description.
    // ------------------------------------------------------------------------
    _txtDescription->setFocus();
}

// ----------------------------------------------------------------------------
// Function:    ~DlgAddEditProject ()
// ----------------------------------------------------------------------------
// Destructs the widget.
// ----------------------------------------------------------------------------
DlgAddEditProject::~DlgAddEditProject () {
}

// ----------------------------------------------------------------------------
// Function:    setDescription (const QString& desc)
// Parameters:  desc        - Description of this project
// ----------------------------------------------------------------------------
// Sets the description of the project to 'desc'.
// ----------------------------------------------------------------------------
void DlgAddEditProject::setDescription (const QString& desc) {
    _txtDescription->setText( desc );
}

// ----------------------------------------------------------------------------
// Function:    description ()
// Returns:     Description of the project
// ----------------------------------------------------------------------------
// Returns the description of the project.
// ----------------------------------------------------------------------------
QString DlgAddEditProject::description () {
    return _txtDescription->text();
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void DlgAddEditProject::setRate (const QString& rate) {
    // ------------------------------------------------------------------------
    // Split the rate string into two pieces; the rate modifier character, and
    // the actual rate itself.  We'll do this by presuming that everything
    // before the first numeric digit is the modifier character (and everything
    // after it is the rate itself).
    // ------------------------------------------------------------------------
    int idx=0;
    while (idx < rate.length()) {
        if (rate[idx].isDigit()) { break; }
        idx++;
    }

    QString modifier = rate.mid( 0, idx );
    QString rateval  = rate.mid( idx );

    // ------------------------------------------------------------------------
    // XXX: hack - hardcode mapping for "Parent's rate"
    // ------------------------------------------------------------------------
    if (modifier.isEmpty()) {
        modifier = i18n("Parent's rate");
    }

    // ------------------------------------------------------------------------
    // Find the modifier character in the combo box and make it the current
    // selection.
    // ------------------------------------------------------------------------
    _comboRateModifier->setCurrentItem(modifier);

    // ------------------------------------------------------------------------
    // Set the rate into the spin box.
    // ------------------------------------------------------------------------
    _spinRate->setValue( rateval.toDouble() );
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
QString DlgAddEditProject::rate () {
    // ------------------------------------------------------------------------
    // If we were told to use the parent's rate, return an empty string; we
    // don't have any explicit rate.
    // ------------------------------------------------------------------------
    if (_comboRateModifier->currentText() == i18n("Parent's rate")) {
        return QString::null;
    }

    // ------------------------------------------------------------------------
    // Build up a rate including both the rate modifier and the rate itself.
    // ------------------------------------------------------------------------
    QString result;
    result.sprintf(
        "%c%0.2f",
        _comboRateModifier->currentText().at(0).toAscii(),
        _spinRate->value()
    );

    // ------------------------------------------------------------------------
    // Done building up the rate, return it to the caller.
    // ------------------------------------------------------------------------
    return result;
}



// ----------------------------------------------------------------------------
// Function:    DlgAddEditTask (QWidget* parent=0)
// Parameters:  parent      - Handle to parent widget
// ----------------------------------------------------------------------------
// Constructs a new DlgAddEditTask widget.
// ----------------------------------------------------------------------------
DlgAddEditTask::DlgAddEditTask (QWidget* parent)
    : DlgAddEditProject(parent)
{
    // ------------------------------------------------------------------------
    // Get a handle to the main widget, so we can create our other widgets as
    // children of this widget.
    // ------------------------------------------------------------------------
    QWidget* page = this->mainWidget();

    // ------------------------------------------------------------------------
    // Spin boxes for the hours/seconds/minutes.
    // ------------------------------------------------------------------------
    QLabel* lblTime = new QLabel( page );
    lblTime->setText( i18n("Time spent:") );
    _layout->addWidget( lblTime, 2, 0, Qt::AlignRight );

    _spinHours = new KIntNumInput( page );
    _layout->addWidget( _spinHours, 2, 1, Qt::AlignLeft );

    _spinMinutes = new KIntNumInput( page );
    _spinMinutes->setRange( 0, 59, 1 );
    _layout->addWidget( _spinMinutes, 2, 2, Qt::AlignLeft );

    _spinSeconds = new KIntNumInput( page );
    _spinSeconds->setRange( 0, 59, 1 );
    _layout->addWidget( _spinSeconds, 2, 3, Qt::AlignLeft );

    // ------------------------------------------------------------------------
    // Set our default geometry.
    // ------------------------------------------------------------------------
    this->resize( 450, 140 );
}

// ----------------------------------------------------------------------------
// Function:    ~DlgAddEditTask ()
// ----------------------------------------------------------------------------
// Destructs the widget.
// ----------------------------------------------------------------------------
DlgAddEditTask::~DlgAddEditTask () {
}

// ----------------------------------------------------------------------------
// Function:    setTimeSpent (const QString& timespent)
// Parameters:  timespent   - Time spent on this task
// ----------------------------------------------------------------------------
// Sets the current time spent on this task to the value provided in
// 'timespent'.
//
// The format of 'timespent' should be HH:MM:SS.
// ----------------------------------------------------------------------------
void DlgAddEditTask::setTimeSpent (const QString& timespent) {
    KTimeclockTime mytime( timespent );
    _spinHours->setValue( mytime.hours() );
    _spinMinutes->setValue( mytime.minutes() );
    _spinSeconds->setValue( mytime.seconds() );
}

// ----------------------------------------------------------------------------
// Function:    timeSpent ()
// Returns:     Time spent on this task.
// ----------------------------------------------------------------------------
// Returns the current time spent on this task, in the format of HH:MM:SS.
// ----------------------------------------------------------------------------
QString DlgAddEditTask::timeSpent () {
    KTimeclockTime mytime;
    mytime.hours( _spinHours->value() );
    mytime.minutes( _spinMinutes->value() );
    mytime.seconds( _spinSeconds->value() );
    return mytime.asString();
}
