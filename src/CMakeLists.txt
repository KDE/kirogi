add_subdirectory(lib)
add_subdirectory(gstreamer)

if(BUILD_PLUGINS)
    add_subdirectory(plugins)
endif()

if(NOT BUILD_APP)
    return()
endif()

if(ANDROID)
    kirigami_package_breeze_icons(ICONS
        arrow-down
        arrow-up
        battery-000
        battery-010
        battery-020
        battery-030
        battery-040
        battery-050
        battery-060
        battery-070
        battery-080
        battery-100
        battery-missing
        chronometer-reset
        clock
        computer-symbolic
        configure
        documentinfo
        edit-redo
        edit-undo
        emblem-default-symbolic
        emblem-important-symbolic
        emblem-photos-symbolic
        emblem-videos-symbolic
        format-align-vertical-bottom
        go-down
        go-home-symbolic
        go-next
        go-previous
        go-up
        gps
        help-about
        input-gamepad-symbolic
        kruler-south
        kruler-west
        map-flat
        media-record-symbolic
        network-wireless-acquiring
        network-wireless-connected-00
        network-wireless-connected-100
        network-wireless-connected-25
        network-wireless-connected-50
        network-wireless-connected-75
        network-wireless-disconnected
        phone-symbolic
        question
        speedometer
        transform-move
        uav
        uav-quadcopter
        window
    )
endif()

if(ANDROID)
    set(ANDROID_HEADERS
        "gstreamer/gstreamer_android.h"
    )

    set(ANDROID_SOURCES
        "gstreamer/gstreamer_android-1.0.c"
        "gstreamer/qt_gstreamer_android-1.0.cpp"
    )
endif()

set(kirogi_SRCS
    main.cpp
    permissions.cpp
    ${ANDROID_HEADERS}
    ${ANDROID_SOURCES}
)

ecm_qt_declare_logging_category(kirogi_SRCS
    HEADER debug.h
    IDENTIFIER KIROGI_APP
    CATEGORY_NAME "kirogi.app"
)

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../data/icons/*apps-kirogi.svg")
ecm_add_app_icon(kirogi_SRCS ICONS ${ICONS_SRCS})

# FIXME In lieu of `ecm_add_app_icon` having Android support.
if(ANDROID)
    install(FILES ${ICONS_SRCS} DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kirigami.2/icons/ RENAME "kirogi.svg")
endif()

if(COMPILE_QML)
    qtquick_compiler_add_resources(RESOURCES resources.qrc)
else()
    qt5_add_resources(RESOURCES resources.qrc)
endif()

add_executable(kirogi ${kirogi_SRCS} ${RESOURCES})

target_compile_definitions(kirogi PRIVATE -DUSE_QRC)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(kirogi PRIVATE
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
        target_compile_options(kirogi PRIVATE
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wnull-dereference
            -fsanitize=leak
        )
    endif()
endif()

target_link_libraries(kirogi
    Qt5::Core
    Qt5::Positioning
    Qt5::Qml
    Qt5::Quick
    KF5::ConfigWidgets
    KF5::CoreAddons
    KF5::I18n
    gstreamer
    ${GSTREAMER_LIBRARIES}
    settings
)

if(NOT ANDROID)
    target_link_libraries(kirogi
        Qt5::Widgets
        KF5::Crash
        ${GSTREAMER_LIBRARIES}
    )
else()
    target_link_libraries(kirogi
        Qt5::AndroidExtras
        # The Material style requires QtSvg.
        Qt5::Svg
        # GStreamer plugins
        gstcoreelements
        gsttypefindfunctions
        gstplayback
        gstvideotestsrc
        gstvideoparsersbad
        gstx264
        gstudp
        gstrtpmanager
        gstrtp
        gstqmlgl
        gstopengl
        gstlibav
        x264
        # Optional
        gstandroidmedia
        gstphotography-1.0
        ${GSTREAMER_STATIC_LIBRARIES}
    )
endif()

install(TARGETS kirogi ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})
