cmake_minimum_required(VERSION 3.12)

file(GLOB_RECURSE LRU_LIB_HEADERS $<$<VERSION_GREATER_EQUAL:${CMAKE_VERSION},3.12>:CONFIGURE_DEPENDS>
    ${CMAKE_SOURCE_DIR}/src/*.h
)

file(GLOB_RECURSE LRU_LIB_SOURCES $<$<VERSION_GREATER_EQUAL:${CMAKE_VERSION},3.12>:CONFIGURE_DEPENDS>
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_library(${LRU_LIB_NAME} STATIC ${LRU_LIB_HEADERS} ${LRU_LIB_SOURCES})
target_include_directories(${LRU_LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src)
#target_link_libraries(${TTA_LIB_NAME} PUBLIC tomlplusplus::tomlplusplus spdlog::spdlog sqlite_orm::sqlite_orm Boost::coroutine Boost::filesystem)

set_property(TARGET ${LRU_LIB_NAME} PROPERTY
             MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
