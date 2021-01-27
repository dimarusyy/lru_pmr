find_package(doctest REQUIRED)

add_executable(unit_tests
    cache_tests.cpp
    main.cpp
    )

include_directories(${CMAKE_SOURCE_DIR}/include)
target_link_libraries(unit_tests doctest::doctest)

 install(
    TARGETS unit_tests
    RUNTIME DESTINATION bin
)

include(doctest)
doctest_discover_tests(unit_tests)
