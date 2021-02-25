// 010-TestCase.cpp
// And write tests in the same file:
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

static int Factorial(int number)
{
    return number <= 1 ? number : Factorial(number - 1) * number; // fail
    // return number <= 1 ? 1      : Factorial( number - 1 ) * number;  // pass
}

TEST_CASE("Factorial of 0 is 1 (fail)", "[single-file]")
{
    REQUIRE(Factorial(0) == 1);
}

TEST_CASE("Factorials of 1 and higher are computed (pass)", "[single-file]")
{
    REQUIRE(Factorial(1) == 1);
    REQUIRE(Factorial(2) == 2);
    REQUIRE(Factorial(3) == 6);
    REQUIRE(Factorial(10) == 3628800);
}

// Compile & run:
// - g++ -std=c++11 -Wall -I$(CATCH_SINGLE_INCLUDE) -o 010-TestCase 010-TestCase.cpp && ./010-TestCase --success

// Expected compact output (all assertions):
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 010-TestCase is a Catch v2.13.3 host application.
// Run with -? for options

// -------------------------------------------------------------------------------
// Factorial of 0 is 1 (fail)
// -------------------------------------------------------------------------------
// 010-TestCase.cpp:12
// ...............................................................................

// 010-TestCase.cpp:14: FAILED:
//   REQUIRE( Factorial(0) == 1 )
// with expansion:
//   0 == 1

// -------------------------------------------------------------------------------
// Factorials of 1 and higher are computed (pass)
// -------------------------------------------------------------------------------
// 010-TestCase.cpp:17
// ...............................................................................

// 010-TestCase.cpp:19: PASSED:
//   REQUIRE( Factorial(1) == 1 )
// with expansion:
//   1 == 1

// 010-TestCase.cpp:20: PASSED:
//   REQUIRE( Factorial(2) == 2 )
// with expansion:
//   2 == 2

// 010-TestCase.cpp:21: PASSED:
//   REQUIRE( Factorial(3) == 6 )
// with expansion:
//   6 == 6

// 010-TestCase.cpp:22: PASSED:
//   REQUIRE( Factorial(10) == 3628800 )
// with expansion:
//   3628800 (0x375f00) == 3628800 (0x375f00)

// ===============================================================================
// test cases: 2 | 1 passed | 1 failed
// assertions: 5 | 4 passed | 1 failed
