add_definitions(-DTRANSLATION_DOMAIN=\"kirogimavlinkplugin\")

set(kirogimavlinkplugin_SRCS
    mavlinkconnection.cpp
    mavlinkplugin.cpp
    mavlinkvehicle.cpp
)

ecm_qt_declare_logging_category(kirogimavlinkplugin_SRCS
    HEADER debug.h
    IDENTIFIER KIROGI_VEHICLESUPPORT_MAVLINK
    CATEGORY_NAME "kirogi.vehiclesupport.mavlink"
)

kcoreaddons_add_plugin(kirogimavlinkplugin
    SOURCES ${kirogimavlinkplugin_SRCS}
    JSON "${CMAKE_CURRENT_SOURCE_DIR}/kirogimavlinkplugin.json"
    INSTALL_NAMESPACE "kirogi/vehiclesupport"
)

find_package(MAVLink REQUIRED)

include_directories(
    ${MAVLINK_INCLUDE_DIR}
    ${MAVLINK_INCLUDE_DIR}/ardupilotmega
)

set_target_properties(kirogimavlinkplugin
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

target_link_libraries(kirogimavlinkplugin
    PRIVATE
        Qt5::Core
        Qt5::Network
        Qt5::Positioning
        KF5::CoreAddons
        KF5::I18n
        KirogiCore
)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(kirogimavlinkplugin PRIVATE -pedantic)
endif()

install(FILES kirogimavlinkplugin.categories DESTINATION ${KDE_INSTALL_CONFDIR})
