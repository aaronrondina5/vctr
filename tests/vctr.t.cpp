#include "vctr.h"

// vctr

// std
#include <stdexcept>
#include <vector>

// gtest
#include <gtest/gtest.h>

namespace arondina
{
namespace vctr
{

TEST(testInit, solveCombinationsThrowsException)
{
    std::vector<std::vector<int>> a{ { 4, 3, 2 } };
    std::vector<int> b{ 1, 2, 3 };
    std::vector<int> x(a.size(), -1);

    EXPECT_THROW(
        {
            try
            {
                solve_combinations(a, b, x);
            }
            catch(const std::runtime_error& e)
            {
                EXPECT_STREQ(e.what(), "not yet implemented");
                throw;
            }
        }
        , std::runtime_error);
}

} // vctr
} // arondina