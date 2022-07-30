add_definitions(-DTRANSLATION_DOMAIN=\"kirogiparrotplugin\")

if(ANDROID)
    find_package(QtZeroConf REQUIRED)
    set_package_properties("qtzeroconf" PROPERTIES
            DESCRIPTION  "Qt wrapper class for ZeroConf libraries across various platforms."
            URL "https://github.com/jbagg/QtZeroConf"
            TYPE REQUIRED
            PURPOSE "Zeroconf discovery on Android.")
endif()

include_directories(${QTZEROCONF_INCLUDE_DIR})

set(kirogiparrotplugin_SRCS
    parrotconnection.cpp
    parrotplugin.cpp
    parrotprotocol.cpp
    parrotvehicle.cpp
)

ecm_qt_declare_logging_category(kirogiparrotplugin_SRCS
    HEADER debug.h
    IDENTIFIER KIROGI_VEHICLESUPPORT_PARROT
    CATEGORY_NAME "kirogi.vehiclesupport.parrot"
)

kcoreaddons_add_plugin(kirogiparrotplugin
    SOURCES ${kirogiparrotplugin_SRCS}
    INSTALL_NAMESPACE "kirogi/vehiclesupport"
)

set_target_properties(kirogiparrotplugin
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(kirogiparrotplugin PRIVATE
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
        target_compile_options(kirogiparrotplugin PRIVATE
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wnull-dereference
            -fsanitize=leak
        )
    endif()
endif()

target_link_libraries(kirogiparrotplugin
    PRIVATE
        Qt5::Core
        Qt5::Network
        Qt5::Positioning
        KF5::CoreAddons
        KF5::I18n
        KirogiCore
)

if(NOT ANDROID)
    target_link_libraries(kirogiparrotplugin PRIVATE KF5::DNSSD)
else()
    target_link_libraries(kirogiparrotplugin PRIVATE ${QTZEROCONF_LIBRARIES})
endif()

# Note: SDP file for a Bebop 2 RTP stream. In case we ever use a video stack that wants one.
# install(FILES bebop.sdp DESTINATION ${DATA_INSTALL_DIR}/kirogi/plugins/vehiclesupport/kirogiparrotplugin)

install(FILES kirogiparrotplugin.categories DESTINATION ${KDE_INSTALL_CONFDIR})
