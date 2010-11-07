
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### install files ###############

install(FILES
  dockicon-1.png  dockicon-2.png  dockicon-3.png
  dockicon-4.png  dockicon-5.png  dockicon-6.png
  dockicon-7.png  dockicon-8.png  dockicon-9.png
  dockicon-10.png dockicon-11.png dockicon-12.png
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
