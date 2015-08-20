project(ktimeclock)
set(PROJECT_VERSION "4.0.4")

find_package(KDE4 REQUIRED)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include(KDE4Defaults)

include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )

# XXX - add Qt3 support to assist in porting
add_definitions( -DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS )

add_subdirectory(ktimeclock)
add_subdirectory(doc)

set(ARCHIVE_NAME ${PROJECT_NAME}-${PROJECT_VERSION})
add_custom_target(dist
  COMMAND git archive --prefix=${ARCHIVE_NAME}/ HEAD
    | gzip > ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.gz
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )
