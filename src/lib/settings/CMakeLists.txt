set(settings_SRCS
    autosettingshandler.cpp
)

ecm_qt_declare_logging_category(settings_SRCS
    HEADER debug.h
    IDENTIFIER SETTINGS
    CATEGORY_NAME "kirogi.settings"
)

kconfig_add_kcfg_files(settings_SRCS GENERATE_MOC settings.kcfgc)

add_library(
    settings
STATIC
    ${settings_SRCS}
)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(settings PRIVATE
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
        target_compile_options(settings PRIVATE
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wnull-dereference
            -fsanitize=leak
        )
    endif()
endif()

target_link_libraries(settings
    PRIVATE
        Qt5::Core
        KF5::ConfigWidgets
)

install(FILES kirogi.kcfg DESTINATION ${KCFG_INSTALL_DIR})