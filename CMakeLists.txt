cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 17)

project(tiny_graphics)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set (CMAKE_CXX_FLAGS "-fopenmp -O3")



INCLUDE_DIRECTORIES (${CMAKE_SOURCE_DIR}
                    "external/include"
                    )

subdirs(src)
