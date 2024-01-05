#include "vector.h"

// vctr

// std

// gtest
#include <gtest/gtest.h>

namespace arondina
{
namespace vctr
{

class VectorFixture : public ::testing::Test
{
};

TEST_F(VectorFixture, testConstructSize)
{
    Vector<int> v(7);
    EXPECT_EQ(7, v.dimensions());
}

TEST_F(VectorFixture, testConstructInitList)
{
    Vector<int> v{7};
    EXPECT_EQ(1, v.dimensions());
    EXPECT_EQ(7, v[0]);
}

TEST_F(VectorFixture, testDestroyVector)
{
    Vector<int>* v = new Vector<int>(7);

    EXPECT_NO_THROW(
    {
        delete v;
    });
}

} // vctr
} // arondina