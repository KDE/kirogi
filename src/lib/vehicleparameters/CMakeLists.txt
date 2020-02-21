set(vehicleparameters_SRCS
    parametermodel.cpp
)

ecm_qt_declare_logging_category(vehicleparameters_SRCS
    HEADER debug.h
    IDENTIFIER VEHICLEPARAMETERS
    CATEGORY_NAME "kirogi.vehicleparameters"
)

add_library(
    vehicleparameters
STATIC
    ${vehicleparameters_SRCS}
)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(vehicleparameters PRIVATE
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
        target_compile_options(vehicleparameters PRIVATE
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wnull-dereference
            -fsanitize=leak
        )
    endif()
endif()

target_link_libraries(vehicleparameters
    PRIVATE
        Qt5::Core
)
