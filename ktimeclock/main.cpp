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

// ----------------------------------------------------------------------------
// Main routine; starts up whole thing.
// ----------------------------------------------------------------------------
int main (int argc, char* argv[]) {
    // ------------------------------------------------------------------------
    // Set up out "About" box
    // ------------------------------------------------------------------------
    KAboutData aboutData(
        // Program name, used internally
        "ktimeclock",
        // Message catalog name (if null, program name is used instead)
        NULL,
        // Displayable program name string,
        ki18n("KTimeclock"),
        // Program version
        KTIMECLOCK_VERSION,
        // Short description of what the app does,
        ki18n("KDE Time Clock"),
        // License the code is released under,
        KAboutData::License_Artistic,
        // Copyright Statement
        ki18n("(C) 1998-2010 Graham TerMarsch"),
        // Optional text shown in About box
        ki18n("w00t!"),
        // Program homepage
        "http://www.howlingfrog.com/",
        // Bug report e-mail address
        "ktimeclock@howlingfrog.com"
    );
    aboutData.addAuthor(
        ki18n("Graham TerMarsch"),
        ki18n("Author"),
        "graham@howlingfrog.com"
    );

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
