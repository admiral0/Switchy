
include_directories(${QT_QTDBUS_INCLUDE_DIR})
set(VGAD_SRCS
    vgaswitcher.cpp
    vgadbus.cpp
    main.cpp)
QT4_AUTOMOC(${VGAD_SRCS})
QT4_ADD_DBUS_ADAPTOR(VGAD_ADAPTOR vgad.xml vgadbus.h VgaDbus)
add_executable(vgad ${VGAD_SRCS} ${VGAD_ADAPTOR})
target_link_libraries(vgad ${QT_QTCORE_LIBRARY} ${QT_QTDBUS_LIBRARY})