
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
