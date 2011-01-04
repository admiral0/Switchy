# Project Needs a name ofcourse
project(plasma-switchy)
 
# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )
 
set(switchy_SRCS videoinfo.cpp switchy.cpp)

kde4_add_ui_files(switchy_SRCS vgaswitcheroo.ui)

kde4_add_plugin(plasma_applet_switchy ${switchy_SRCS})
target_link_libraries(plasma_applet_switchy 
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
 
install(TARGETS plasma_applet_switchy
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-switchy.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})

add_subdirectory(daemon)