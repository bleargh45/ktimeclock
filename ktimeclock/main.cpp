// ----------------------------------------------------------------------------
// Filename:    main.cpp
// ----------------------------------------------------------------------------
// $Revision: 1.4 $
// ----------------------------------------------------------------------------

#include <KUniqueApplication>
#include <KLocale>
#include <KCmdLineArgs>
#include <KAboutData>
#include "version.h"
#include "mainwindow.h"

static const char *description = I18N_NOOP( "KDE Time clock." );

// ----------------------------------------------------------------------------
// Main routine; starts up whole thing.
// ----------------------------------------------------------------------------
int main (int argc, char* argv[])
{
    // ------------------------------------------------------------------------
    // Set up out "About" box
    // ------------------------------------------------------------------------
    KAboutData aboutData( "ktimeclock",
       I18N_NOOP("KTimeclock"),
       KTIMECLOCK_VERSION,
       description,
// UNFINISHED -> Need useful license text here.
       KAboutData::License_Artistic,
       "(C) 1998-2002 Graham TerMarsch",
       "http://www.howlingfrog.com/opensource/ktimeclock/",
       "ktimeclock-bugs@howlingfrog.com"
       );
    aboutData.addAuthor( "Graham TerMarsch",
        I18N_NOOP("Original Author"),
        "graham@howlingfrog.com" );

    // ------------------------------------------------------------------------
    // Read in and act on any command line args.
    // ------------------------------------------------------------------------
    KCmdLineArgs::init( argc, argv, &aboutData );

    // ------------------------------------------------------------------------
    // Create a new unique application, set up the timeclock window, and show
    // the window to the user.  We make the application "unique", so that we
    // don't run into problems with more than one instance trying to update the
    // data file at once.
    // ------------------------------------------------------------------------
    KUniqueApplication::addCmdLineOptions();
    if (!KUniqueApplication::start()) return 0;

    KUniqueApplication myApp;
    KTimeclockWindow *ktimeclock = new KTimeclockWindow;
    myApp.setMainWidget( ktimeclock );
    ktimeclock->show();

    // ------------------------------------------------------------------------
    // Run the application, giving the return code back out as our exit code.
    // ------------------------------------------------------------------------
    int rc = myApp.exec();
    return rc;
}
