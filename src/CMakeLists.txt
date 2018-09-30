cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 17)

project(tiny_graphics)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set (CMAKE_CXX_FLAGS "-fopenmp -O3")
set (SOURCES
    "main.cpp"
    "image.cpp"
    "color.cpp"
    "rasterizer.cpp"
    )

set (HEADERS
    "image.hpp"
    "framebuffer.hpp"
    "color.hpp"
    "rasterizer.hpp"
    "model.hpp"
    "draw.hpp"
    "math/vector.hpp"
    )

INCLUDE_DIRECTORIES (${CMAKE_SOURCE_DIR}
                    ${PROJECT_SRC_DIR}/external/include
                    )

add_executable(${PROJECT_NAME}
                ${HEADERS}
                ${SOURCES}
                )

target_link_libraries(${PROJECT_NAME} SDL2 gomp)
