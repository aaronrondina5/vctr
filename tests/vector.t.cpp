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
    size_t expected_size(7);
    Vector<int> v(7);
    EXPECT_EQ(expected_size, v.dimensions());
}

TEST_F(VectorFixture, testConstructInitList)
{
    Vector<int> v{7};
    size_t expected_size(1);
    EXPECT_EQ(expected_size, v.dimensions());
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