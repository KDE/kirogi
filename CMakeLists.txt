cmake_minimum_required(VERSION 3.10)
find_package(ECM 5.54.0 REQUIRED NO_MODULE)
set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
set(QT_MIN_VERSION "5.12.0")
set(KF_MIN_VERSION "5.84.0")

project(kirogi VERSION 0.1)

set(CMAKE_AUTOMOC ON)
set(AUTOMOC_MOC_OPTIONS -Muri=org.kde.kirogi)
set(CMAKE_AUTORCC ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
include(WriteBasicConfigVersionFile)
include(FeatureSummary)
include(ECMGenerateHeaders)
include(ECMAddAppIcon)
include(ECMInstallIcons)
include(ECMSetupVersion)
include(ECMQMLModules)
include(ECMQtDeclareLoggingCategory)
include(GNUInstallDirs)
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(KDEClangFormat)
include(FindPkgConfig)

option(BUILD_APP "Build the Kirogi application." TRUE)
add_feature_info(BUILD_APP BUILD_APP "Build the Kirogi application.")

option(BUILD_QT_QUICK_LIB "Build the Kirogi library with Qt Quick support." TRUE)
add_feature_info(BUILD_QT_QUICK_LIB BUILD_QT_QUICK_LIB "Build the Kirogi library with Qt Quick support.")

option(BUILD_PLUGINS "Build the Kirogi plugins." TRUE)
add_feature_info(BUILD_PLUGINS BUILD_PLUGINS "Build the Kirogi plugins.")

option(CLANG_TIDY "Enable clang-tidy during the build process." FALSE)
add_feature_info(CLANG_TIDY CLANG_TIDY "Enable clang-tidy during the build process.")

option(COMPILE_QML "Pre-compile QML files using the Qt Quick compiler." FALSE)
add_feature_info(COMPILE_QML COMPILE_QML "Pre-compile QML files using the Qt Quick compiler.")

option(DEBUG_QML "Build Kirogi with QML debugging/profiling support." FALSE)
add_feature_info(DEBUG_QML DEBUG_QML "Build Kirogi with QML debugging/profiling support.")

if (CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*;-fix")
endif()

if (BUILD_APP)
    if(NOT ANDROID)
        set(qtDesktopComponents "Widgets")
        set(kfDesktopComponents
            "Crash"
            "DNSSD"
        )
    else()
        # The Material style requires QtSvg.
        set(qtAndroidComponents
            "AndroidExtras"
            "Svg"
        )
    endif()

    ecm_find_qmlmodule(QtGamepad 1.12)
    ecm_find_qmlmodule(QtGraphicalEffects 1.12)
    ecm_find_qmlmodule(QtLocation 5.12)
    ecm_find_qmlmodule(org.kde.kirigami 2.6)

    # FIXME TODO: QtGamepad currently causes performance problems on
    # Android (blocking multi-tasking) that need to be investigated.
    if(NOT ANDROID)
        ecm_find_qmlmodule(QtPositioning 5.12)
    endif()
endif()

if(BUILD_APP OR BUILD_QT_QUICK_LIB)
    set(qtQuickComponents
        "Qml"
        "Quick"
    )
endif()

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS
    Core
    Gamepad
    Network
    Positioning
    ${qtQuickComponents}
    ${qtDesktopComponents}
    ${qtAndroidComponents}
)

find_package(KF5 ${KF_MIN_VERSION} REQUIRED
    ConfigWidgets
    CoreAddons
    I18n
    Kirigami2
    ${kfDesktopComponents}
)

find_package(KF${QT_MAJOR_VERSION}KirigamiAddons 0.11 REQUIRED)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GSTREAMER REQUIRED
    gstreamer-1.0
    gstreamer-video-1.0
)

link_directories(${GSTREAMER_LIBRARY_DIRS})
include_directories (${GSTREAMER_INCLUDE_DIRS})

if(ANDROID)
    pkg_check_modules(GSTREAMER REQUIRED
        gstreamer-1.0
        gstreamer-audio-1.0
        gstreamer-codecparsers-1.0
        gstreamer-controller-1.0
        gstreamer-gl-1.0
        gstreamer-net-1.0
        gstreamer-pbutils-1.0
        gstreamer-rtp-1.0
        gstreamer-tag-1.0
        gstreamer-video-1.0
        gio-2.0
        graphene-1.0
        libjpeg
        libpng16
        libavformat
        libavcodec
        libavfilter
        libavutil
        libswresample
        x264
    )

    link_directories(${GSTREAMER_LIBRARY_DIRS}/gstreamer-1.0/)

    find_library(liblog log)
    find_library(libandroid android)
    find_library(libEGL EGL)
    find_library(libGLESv2 GLESv2)
endif()

if(COMPILE_QML)
    find_package(Qt5QuickCompiler)

    set_package_properties(Qt5QuickCompiler PROPERTIES
        DESCRIPTION "Pre-compile QML files using the Qt Quick compiler."
        TYPE OPTIONAL
    )
endif()

if(DEBUG_QML)
    message(STATUS "To enable the QML debugger/profiler, run with: '-qmljsdebugger=port:1234'")
    add_definitions(-DQMLJSDEBUGGER)
    add_definitions(-DQT_DECLARATIVE_DEBUG)
    add_definitions(-DQT_QML_DEBUG)
endif()

add_definitions(
  -DQT_USE_QSTRINGBUILDER
  -DQT_NO_CAST_TO_ASCII
  -DQT_STRICT_ITERATORS
  -DQT_NO_URL_CAST_FROM_STRING
  -DQT_NO_CAST_FROM_BYTEARRAY
  -DQT_NO_SIGNALS_SLOTS_KEYWORDS
  -DQT_USE_FAST_OPERATOR_PLUS
)

# For KConfigXT out-of-source builds.
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(data)
add_subdirectory(src)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

# Add clang-format target for all our real source files.
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.cpp src/*.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
