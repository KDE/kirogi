set(gstreamer_SRCS
    gstreamerintegration.cpp
    videosurface.cpp
)

ecm_qt_declare_logging_category(gstreamer_SRCS
    HEADER debug.h
    IDENTIFIER GSTREAMER
    CATEGORY_NAME "kirogi.gstreamer"
)

add_library(
    gstreamer
STATIC
    ${gstreamer_SRCS}
)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(gstreamer PRIVATE
        -pedantic
        -Woverloaded-virtual
        -Wunused
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
    )

    if(CMAKE_COMPILER_IS_GNUCXX)
        target_compile_options(gstreamer PRIVATE
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wnull-dereference
            -fsanitize=leak
        )
    endif()
endif()

target_link_libraries(gstreamer
    PRIVATE
        Qt5::Core
        Qt5::Qml
        Qt5::Quick
)
