include_directories(${QT_QTDBUS_INCLUDE_DIR})
set(VGAC_SRCS 
    main.cpp
    ../videoinfo.cpp)
QT4_AUTOMOC(${VGAC_SRCS})
QT4_ADD_DBUS_INTERFACE(VGAC_DBUS ../daemon/vgad.xml VgaCDbus)
add_executable(vgac ${VGAC_SRCS} ${VGAC_DBUS})
target_link_libraries(vgac ${QT_QTCORE_LIBRARY} ${QT_QTDBUS_LIBRARY})