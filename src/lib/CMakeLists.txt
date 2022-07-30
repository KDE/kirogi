ecm_setup_version(PROJECT
    VARIABLE_PREFIX KIROGI
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KirogiCoreConfigVersion.cmake"
    SOVERSION ${KIROGI_VERSION_STRING}
)

set(kirogicore_SRCS
    abstractvehicle.cpp
    abstractconnection.cpp
    abstractpluginmodel.cpp
    vehiclesupportplugin.cpp
    vehiclesupportpluginmodel.cpp
    connectionconfiguration.cpp
    udpconfiguration.cpp
)

ecm_qt_declare_logging_category(kirogicore_SRCS
    HEADER debug.h
    IDENTIFIER KIROGI_CORE
    CATEGORY_NAME "kirogi.core"
)

ecm_generate_headers(Kirogi_CamelCase_HEADERS
    HEADER_NAMES
        AbstractVehicle
        AbstractConnection
        AbstractPluginModel
        VehicleSupportPlugin
        VehicleSupportPluginModel
        ConnectionConfiguration
        UdpConfiguration
    REQUIRED_HEADERS Kirogi_HEADERS
    PREFIX kirogi
)

add_subdirectory(positionsource)
add_subdirectory(settings)
add_subdirectory(vehicleparameters)

add_library(kirogicore SHARED ${kirogicore_SRCS} ${Kirogi_HEADERS})
add_library(KirogiCore ALIAS kirogicore)

generate_export_header(kirogicore
    BASE_NAME Kirogi
    EXPORT_FILE_NAME kirogicore_export.h)

target_include_directories(kirogicore INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/kirogi>")

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(kirogicore PRIVATE
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
        target_compile_options(kirogicore PRIVATE
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wnull-dereference
            -fsanitize=leak
        )
    endif()
endif()

target_link_libraries(kirogicore
    PRIVATE
        Qt5::Core
        Qt5::Positioning
        Qt5::Qml
        Qt5::Quick
        KF5::CoreAddons

        positionsource
        settings
        vehicleparameters
)

set_target_properties(kirogicore
    PROPERTIES
    VERSION ${KIROGI_VERSION_STRING}
    SOVERSION ${KIROGI_SOVERSION}
    EXPORT_NAME KirogiCore
    OUTPUT_NAME kirogicore
)

install(TARGETS kirogicore EXPORT kirogicoreLibraryTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
        ${Kirogi_HEADERS}
        ${CMAKE_CURRENT_BINARY_DIR}/kirogicore_export.h
        DESTINATION ${KDE_INSTALL_INCLUDEDIR}/Kirogi/kirogi COMPONENT Devel)

install(FILES
        ${Kirogi_CamelCase_HEADERS}
        DESTINATION ${KDE_INSTALL_INCLUDEDIR}/Kirogi/Kirogi COMPONENT Devel)

write_basic_config_version_file(${CMAKE_CURRENT_BINARY_DIR}/KirogiCoreConfigVersion.cmake
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY AnyNewerVersion)

set(CMAKECONFIG_INSTALL_DIR ${KDE_INSTALL_LIBDIR}/cmake/KirogiCore)

configure_package_config_file(KirogiCoreConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/KirogiCoreConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/KirogiCoreConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/KirogiCoreConfigVersion.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})

install(EXPORT kirogicoreLibraryTargets
        DESTINATION ${CMAKECONFIG_INSTALL_DIR}
        FILE KirogiCoreLibraryTargets.cmake)

install(FILES kirogi.categories DESTINATION ${KDE_INSTALL_CONFDIR})

if(NOT BUILD_QT_QUICK_LIB)
    return()
endif()

add_library(kirogiqtquickplugin SHARED qtquickplugin.cpp)

target_link_libraries(kirogiqtquickplugin
    Qt5::Positioning
    Qt5::Qml
    Qt5::Quick
    KF5::CoreAddons

    KirogiCore
    positionsource
    settings
    vehicleparameters
    )

install(TARGETS kirogiqtquickplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kirogi)
install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kirogi)
