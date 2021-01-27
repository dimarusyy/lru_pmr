cmake_minimum_required(VERSION 3.12)

set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON")

set(LRU_LIB_VERSION 0.0.1)
set(LRU_LIB_NAME "lru")

project(tta-srv LANGUAGES CXX
                VERSION "${LRU_LIB_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#add_subdirectory(src)
add_subdirectory(tests)

enable_testing()

