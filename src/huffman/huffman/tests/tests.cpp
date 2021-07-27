#ifdef TEST_BUILD
/*
* This file is only active in testing build.
* Its purpose is to initialize the testing framework.
* 
* You can safely ignore this file; no changes will have to be made to it.
* Make sure this file remains included in the project though: without it, tests will not be able to run.
*/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "iostream"
#include "util.h"
#include "frequency-table.h"
#include "binary-tree.h"

TEST_CASE("Passing test")
{
    REQUIRE(1 == 1);
}

TEST_CASE("Bits_Needed test")
{
    REQUIRE(bits_needed(2) == 1);
    REQUIRE(bits_needed(3) == 2);
    REQUIRE(bits_needed(4) == 2);
    REQUIRE(bits_needed(5) == 3);
    REQUIRE(bits_needed(6) == 3);
    REQUIRE(bits_needed(7) == 3);
    REQUIRE(bits_needed(8) == 3);
    REQUIRE(bits_needed(9) == 4);
    REQUIRE(bits_needed(128) == 7);
    REQUIRE(bits_needed(129) == 8);
    REQUIRE(bits_needed(1000000) == 20);


}

TEST_CASE("Bytes_needed test")
{
    REQUIRE(bytes_needed(2) == 1);
    REQUIRE(bytes_needed(3) == 1);
    REQUIRE(bytes_needed(128) == 1);
    REQUIRE(bytes_needed(129) == 1);
    REQUIRE(bytes_needed(255) == 1);
    REQUIRE(bytes_needed(256) == 1);
    REQUIRE(bytes_needed(1000000) == 3);
}

TEST_CASE("FrequencyTable testing") {
    data::FrequencyTable<std::string> myfreq;
    myfreq.increment("a");
    REQUIRE(myfreq["a"] == 1);
    myfreq.increment("a");
    REQUIRE(myfreq["a"] == 2);
    REQUIRE(myfreq["b"] == 0);
    myfreq.increment("b");
    REQUIRE(myfreq["b"] == 1);

    REQUIRE(myfreq.values().size() == 2);
    std::cout << myfreq.values().size();


    data::FrequencyTable<std::string> newfreq;
    //data::count_frequencies<std::string>(std::vector({ "a", "b" }));
    newfreq.increment("a");
    REQUIRE(newfreq["a"] == 1);
}

TEST_CASE("Binary Tree testsing") {
    data::Leaf<int> myleaf(0);
    REQUIRE(myleaf.getValue() == 0);


}


#endif
