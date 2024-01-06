#include "vector.h"

// vctr

// std
#include <iostream>
#include <stdexcept>

// gtest
#include <gtest/gtest.h>

namespace arondina
{
namespace vctr
{

TEST(VectorTests, testConstructDefaultValues)
{
    Vector<int> v(7, -1);
    EXPECT_EQ(7, v.dimensions());
    for(int i = 0; i < 7; ++i)
    {
        EXPECT_EQ(-1, v[i]);
    }
}

TEST(VectorTests, testConstructInitList)
{
    Vector<int> v{7};
    EXPECT_EQ(1, v.dimensions());
    EXPECT_EQ(7, v[0]);
}

TEST(VectorTests, testDestroyVector)
{
    Vector<int>* v = new Vector<int>(7, 0);

    EXPECT_NO_THROW(
    {
        delete v;
    });
}

TEST(VectorTests, largerInitListConstructor)
{
    Vector<int> v{9, 3, 4, 1, 4};
    EXPECT_EQ(5, v.dimensions());
    EXPECT_EQ(9, v[0]);
    EXPECT_EQ(3, v[1]);
    EXPECT_EQ(4, v[2]);
    EXPECT_EQ(1, v[3]);
    EXPECT_EQ(4, v[4]);

    EXPECT_THROW({
        try
        {
            v[5];
        }
        catch(const std::runtime_error& e)
        {
            EXPECT_STREQ("index out of bounds.", e.what());
            throw;
        }
    }
    , std::runtime_error);
}

TEST(VectorTests, addValues)
{
    Vector<int> v_length_nine(9, -1);

    v_length_nine[7] = 4;
    v_length_nine[3] = 1;

    EXPECT_EQ(4, v_length_nine[7]);
    EXPECT_EQ(1, v_length_nine[3]);

    for(int i = 0; i < v_length_nine.dimensions(); ++i)
    {
        if(i != 7 && i != 3)
        {
            EXPECT_EQ(-1, v_length_nine[i]);
        }
    }
}

class VectorIteratorTest : public ::testing::Test
{
protected:
    Vector<int> v;

    VectorIteratorTest() : v(5, 0)
    {   
    }

    void SetUp() override
    { 
        for (int i = 0; i < 5; ++i)
        {
            v[i] = i;
        }
    }
};

TEST_F(VectorIteratorTest, IteratorDereference)
{
    auto it = v.begin();
    EXPECT_EQ(*it, 0);
}

TEST_F(VectorIteratorTest, IteratorIncrement)
{
    auto it = v.begin();
    auto pre_increment_val = ++it;
    EXPECT_EQ(1, *it);
    EXPECT_EQ(1, *pre_increment_val);

    auto post_increment_val = it++;
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(1, *post_increment_val);
}

TEST_F(VectorIteratorTest, IteratorEquality)
{
    EXPECT_EQ(v.begin(), v.begin());
    EXPECT_NE(v.begin(), v.end());
}

TEST_F(VectorIteratorTest, IterateThroughVector)
{
    int expected_value = 0;
    for (auto it = v.begin(); it != v.end(); ++it)
    {
        EXPECT_EQ(expected_value, *it);
        ++expected_value;
    }
}

TEST_F(VectorIteratorTest, IteratorRelationalOperators) {
    auto it = v.begin();
    auto mid = v.begin() + 2;

    // Test for less than
    EXPECT_TRUE(it < mid);
    EXPECT_FALSE(mid < it);

    // Test for less than or equal
    EXPECT_TRUE(it <= mid);
    EXPECT_FALSE(mid <= it);
    EXPECT_TRUE(it <= it); // Equal iterators

    // Test for greater than
    EXPECT_TRUE(mid > it);
    EXPECT_FALSE(it > mid);

    // Test for greater than or equal
    EXPECT_TRUE(mid >= it);
    EXPECT_FALSE(it >= mid);
    EXPECT_TRUE(mid >= mid); // Equal iterators

    // Test for equality
    EXPECT_TRUE(it == it);
    EXPECT_FALSE(it == mid);

    // Test for inequality
    EXPECT_TRUE(it != mid);
    EXPECT_FALSE(it != it);
}

TEST_F(VectorIteratorTest, IteratorRandomAccess) {
    auto it = v.begin();

    // Test operator+
    EXPECT_EQ(2, *(it + 2));

    // Test operator-
    auto it2 = it + 3;
    EXPECT_EQ(1, *(it2 - 2));

    // Test operator+=
    it += 2;
    EXPECT_EQ(2, *it);

    // Test operator-=
    it -= 1;
    EXPECT_EQ(1, *it);
}

TEST_F(VectorIteratorTest, IteratorDifference)
{
    auto it = v.begin();
    auto it2 = v.begin() + 3;

    EXPECT_EQ(3, it2 - it);
    EXPECT_EQ(-3, it - it2);
}


} // vctr
} // arondina