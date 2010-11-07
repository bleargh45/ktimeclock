
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### install files ###############

install(FILES
  dockicon0.png dockicon1.png dockicon2.png dockicon3.png
  dockicon4.png dockicon5.png dockicon6.png dockicon7.png
  DESTINATION ${DATA_INSTALL_DIR}/ktimeclock/pics/
)



#original Makefile.am contents follow:

################################################################################
## Specify the data files we've got here that need to get installed.
################################################################################
#pics_DATA = dockicon0.xpm \
#    dockicon1.xpm \
#    dockicon2.xpm \
#    dockicon3.xpm \
#    dockicon4.xpm \
#    dockicon5.xpm \
#    dockicon6.xpm \
#    dockicon7.xpm
#
#picsdir = $(kde_datadir)/ktimeclock/pics
