
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )

add_subdirectory(pics)


########### next target ###############

set(ktimeclock_SRCS
    kttime.cpp
    ktdocker.cpp
    ktlistitem.cpp
    ktaddedit.cpp
    ktimeclock.cpp
    mainwindow.cpp
    main.cpp
)

kde4_add_executable(ktimeclock ${ktimeclock_SRCS})

target_link_libraries(ktimeclock QtXml Qt3Support kde3support ${KDE4_KDEUI_LIBS})

install(TARGETS ktimeclock ${INSTALL_TARGETS_DEFAULT_ARGS})


########### install files ###############

install(FILES  ktimeclock.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES  ktimeclockui.rc DESTINATION ${DATA_INSTALL_DIR}/ktimeclock/)

kde4_install_icons(${ICON_INSTALL_DIR})



#original Makefile.am contents follow:

################################################################################
## Autoconf-generated variables.
################################################################################
#AM_CPPFLAGS = -Wall -pedantic -Woverloaded-virtual
#INCLUDES = $(all_includes)
#LDFLAGS = $(all_libraries) $(KDE_RPATH)
#LDADD = $(LIB_KDEUI) $(LIB_KIO)
#
################################################################################
## Specify the name of the program we're using, and the subdirs to build
################################################################################
#bin_PROGRAMS=ktimeclock
#SUBDIRS = pics
#
################################################################################
## Use am_edit to auto-detect which files have meta-sources.
################################################################################
#METASOURCES = AUTO
#
################################################################################
## Specify a list of all of the source files we've got, and which ones don't
## need to be installed.
################################################################################
#ktimeclock_SOURCES = \
#    kttime.cpp \
#    ktdocker.cpp \
#    ktlistitem.cpp \
#    ktaddedit.cpp \
#	preferences.cpp \
#    ktimeclock.cpp \
#	mainwindow.cpp \
#	main.cpp
#
#noinst_HEADERS = \
#    kttime.h \
#    ktdocker.h \
#    ktlistitem.h \
#    ktaddedit.h \
#	preferences.h \
#    ktimeclock.h \
#	mainwindow.h \
#	resource.h version.h
#
################################################################################
## Don't forget a few other files that need to be part of the distribution.
################################################################################
#EXTRA_DIST = \
#    ktimeclock.desktop ktimeclockui.rc \
#    lo16-app-ktimeclock.png lo32-app-ktimeclock.png \
#    hi22-app-ktimeclock.png hi32-app-ktimeclock.png hi48-app-ktimeclock.png
#
################################################################################
## Have the list of icons and their installation auto-generated.
################################################################################
#KDE_ICON = AUTO
#
################################################################################
## Specify the final locations for our installation.
################################################################################
#xdg_apps_DATA = ktimeclock.desktop
#
#rcdir = $(kde_datadir)/ktimeclock
#rc_DATA = ktimeclockui.rc
#
################################################################################
## Specify how our internationalization messages get build up
################################################################################
#pot:
#	$(XGETTEXT) *.cpp -o $(podir)/ktimeclock.pot
#
################################################################################
## Specify how to build up my src docs
################################################################################
#apidocs:
#	kdoc -a -p -d ./api ktimeclock *.h -lkdecore -lkdeui -lqt
