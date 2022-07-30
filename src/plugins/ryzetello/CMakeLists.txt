add_definitions(-DTRANSLATION_DOMAIN=\"kirogiryzetelloplugin\")

set(kirogiryzetelloplugin_SRCS
    ryzetelloconnection.cpp
    ryzetelloplugin.cpp
    ryzetellovehicle.cpp
)

ecm_qt_declare_logging_category(kirogiryzetelloplugin_SRCS
    HEADER debug.h
    IDENTIFIER KIROGI_VEHICLESUPPORT_RYZETELLO
    CATEGORY_NAME "kirogi.vehiclesupport.ryzetello"
)

kcoreaddons_add_plugin(kirogiryzetelloplugin
    SOURCES ${kirogiryzetelloplugin_SRCS}
    INSTALL_NAMESPACE "kirogi/vehiclesupport"
)

set_target_properties(kirogiryzetelloplugin
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(kirogiryzetelloplugin PRIVATE
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
        target_compile_options(kirogiryzetelloplugin PRIVATE
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wnull-dereference
            -fsanitize=leak
        )
    endif()
endif()

target_link_libraries(kirogiryzetelloplugin
    PRIVATE
        Qt5::Core
        Qt5::Network
        Qt5::Positioning
        KF5::CoreAddons
        KF5::I18n
        KirogiCore
)

install(FILES kirogiryzetelloplugin.categories DESTINATION ${KDE_INSTALL_CONFDIR})
