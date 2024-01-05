#include "vector.h"

// vctr

// std

// gtest
#include <gtest/gtest.h>

namespace arondina
{
namespace vctr
{

TEST(VectorTests, testConstructSize)
{
    Vector<int> v(7);
    EXPECT_EQ(7, v.dimensions());
}

TEST(VectorTests, testConstructInitList)
{
    Vector<int> v{7};
    EXPECT_EQ(1, v.dimensions());
    EXPECT_EQ(7, v[0]);
}

TEST(VectorTests, testDestroyVector)
{
    Vector<int>* v = new Vector<int>(7);

    EXPECT_NO_THROW(
    {
        delete v;
    });
}

} // vctr
} // arondina