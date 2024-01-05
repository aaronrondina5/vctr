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

TEST_F(VectorFixture, testCreateVectorOfSize)
{
    Vector v(7);
    EXPECT_EQ(7, v.dimensions());
}

TEST_F(VectorFixture, testDestroyVector)
{
    Vector* v = new Vector(7);

    EXPECT_NO_THROW(
    {
        delete v;
    });
}

} // vctr
} // arondina