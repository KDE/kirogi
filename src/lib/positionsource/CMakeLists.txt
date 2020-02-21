set(positionsource_SRCS
    positionsource.cpp
)

ecm_qt_declare_logging_category(positionsource_SRCS
    HEADER debug.h
    IDENTIFIER POSITIONSOURCE
    CATEGORY_NAME "kirogi.positionsource"
)

add_library(
    positionsource
STATIC
    ${positionsource_SRCS}
)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(positionsource PRIVATE
        -pedantic
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
    )
endif()

target_link_libraries(positionsource
    PRIVATE
        Qt5::Core
        Qt5::Network
        Qt5::Positioning
        Qt5::Quick
)
