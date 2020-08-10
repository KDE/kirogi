add_definitions(-DTRANSLATION_DOMAIN=\"kirogimavlinkplugin\")

find_package(MAVLink)

if(MAVLink_FOUND)
    message(STATUS "MAVLink found: Kirogi will build with MAVLink integration.")
else()
    message(WARNING "MAVLink not found.")
    return()
endif()

set(kirogimavlinkplugin_SRCS
    mavlinkplugin.cpp
    mavlinkpluginconfig.cpp
    mavlinkconnection.cpp
    mavlinkudpconnection.cpp
    mavlinkvehicle.cpp
)

ecm_qt_declare_logging_category(kirogimavlinkplugin_SRCS
    HEADER mavlink_plugin_debug.h
    IDENTIFIER KIROGI_VEHICLESUPPORT_MAVLINK_PLUGIN
    CATEGORY_NAME "kirogi.vehiclesupport.mavlink.plugin"
)

ecm_qt_declare_logging_category(kirogimavlinkplugin_SRCS
    HEADER mavlink_connection_debug.h
    IDENTIFIER KIROGI_VEHICLESUPPORT_MAVLINK_CONNECTION
    CATEGORY_NAME "kirogi.vehiclesupport.mavlink.connection"
)

kcoreaddons_add_plugin(kirogimavlinkplugin
    SOURCES ${kirogimavlinkplugin_SRCS}
    JSON "${CMAKE_CURRENT_SOURCE_DIR}/kirogimavlinkplugin.json"
    INSTALL_NAMESPACE "kirogi/vehiclesupport"
)

include_directories(
    ${MAVLINK_INCLUDE_DIR}
    ${MAVLINK_INCLUDE_DIR}/ardupilotmega
)

set_target_properties(kirogimavlinkplugin
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(kirogimavlinkplugin PRIVATE
        -pedantic
        -Woverloaded-virtual
        -Wunused
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
    )

    if(CMAKE_COMPILER_IS_GNUCXX)
        target_compile_options(kirogimavlinkplugin PRIVATE
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wnull-dereference
            -fsanitize=leak
        )
    endif()
endif()

target_link_libraries(kirogimavlinkplugin
    PRIVATE
        Qt5::Core
        Qt5::Network
        Qt5::Positioning
        KF5::CoreAddons
        KF5::I18n
        KirogiCore
        vehicleparameters
)

install(FILES kirogimavlinkplugin.categories DESTINATION ${KDE_INSTALL_CONFDIR})
