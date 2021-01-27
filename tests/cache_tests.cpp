#include <doctest/doctest.h>
#include "cache.h"

TEST_CASE("create")
{
    cache_t<std::string, std::pmr::string> c(100);
    
    CHECK(c.size() == 0);
}

TEST_CASE("add 1")
{
    cache_t<std::string, std::string> c(1000);
    c.add("key1", "hello #1");
    
    CHECK(c.size() == 1);
}

TEST_CASE("add 2 the same")
{
    cache_t<std::string, std::string> c(1000);
    c.add("key1", "hello #1");
    c.add("key1", "hello #1");

    CHECK(c.size() == 1);
}

TEST_CASE("add 3")
{
    cache_t<std::string, std::string> c(1000);
    c.add("key1", "hello #1");
    c.add("key2", "hello #2");
    
    auto rc = c.get("key1");

    CHECK(rc.value_or("") == "hello #1");
}
