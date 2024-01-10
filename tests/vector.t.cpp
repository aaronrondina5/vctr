#include "vector.h"

// vctr

// std
#include <cmath>
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

TEST(VectorTests, copyConstructor)
{
    Vector<int> v1{43,55,79};
    Vector<int> v2(v1);
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 != v2);
}

TEST(VectorTests, moveConstructor)
{
    Vector<int> v1{1,2,3,4};
    Vector<int> v2(std::move(v1));

    EXPECT_EQ(4, v2.dimensions());
    EXPECT_EQ(0, v1.dimensions());
}

TEST(VectorTests, assignment)
{
    Vector<int> v1{1,2,3,4,5};
    Vector<int> v2(7, -1);

    v2 = v1;

    EXPECT_EQ(5, v1.dimensions());
    EXPECT_EQ(1, v1[0]);
    EXPECT_EQ(5, v1[4]);
    EXPECT_EQ(5, v2.dimensions());
    EXPECT_EQ(1, v2[0]);
    EXPECT_EQ(5, v2[4]);
    EXPECT_TRUE(v1 == v2);
}

TEST(VectorTests, moveAssignment)
{
    Vector<int> v1{1,2,3,4,5};
    Vector<int> v2(7, -1);

    v2 = std::move(v1);

    EXPECT_EQ(0, v1.dimensions());
    EXPECT_EQ(5, v2.dimensions());
    EXPECT_EQ(1, v2[0]);
    EXPECT_EQ(5, v2[4]);
    EXPECT_TRUE(v1 != v2);
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

TEST(VectorTests, setValuesAtIndices)
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

TEST(VectorTests, addVectors)
{
    Vector<int> v1{7, 8, 9, 12};
    Vector<int> v2{2, 3, 4, 14};

    Vector<int> result = v1 + v2;
    Vector<int> expected{9, 11, 13, 26};

    EXPECT_TRUE(expected == result);
}

TEST(VectorTests, addVectorsParallel)
{
    Vector<int> v1(VectorConstants::maxDimensionsForSequentialArithmeticOps + 200, 3);
    Vector<int> v2(VectorConstants::maxDimensionsForSequentialArithmeticOps + 200, 1);

    Vector<int> result = v1 + v2;
    Vector<int> expected(VectorConstants::maxDimensionsForSequentialArithmeticOps + 200, 4);

    EXPECT_TRUE(expected == result);
}

TEST(VectorTests, addEmptyVectors)
{
    Vector<int> v1{};
    Vector<int> v2{};

    Vector<int> result = v1 + v2;
    Vector<int> expected{};

    EXPECT_TRUE(expected == result);
}

TEST(VectorTests, addVectorsThrowsUnequalSizes)
{
    Vector<int> v1{7, 8, 9, 12};
    Vector<int> v2{2, 3, 4, 14, 7};

    EXPECT_THROW({
        try
        {
            Vector<int> result = v1 + v2;
        }
        catch(const std::runtime_error& e)
        {
            EXPECT_STREQ("unequal vector sizes.", e.what());
            throw;
        }
    }
    , std::runtime_error);
}

TEST(VectorTests, subtractVectors)
{
    Vector<int> v1{7, 3, 9, 12};
    Vector<int> v2{2, 8, 4, 17};

    Vector<int> result = v1 - v2;
    Vector<int> expected{5, -5, 5, -5};

    EXPECT_TRUE(expected == result);
}

TEST(VectorTests, subtractVectorsParallel)
{
    Vector<int> v1(VectorConstants::maxDimensionsForSequentialArithmeticOps + 200, 3);
    Vector<int> v2(VectorConstants::maxDimensionsForSequentialArithmeticOps + 200, 1);

    Vector<int> result = v1 - v2;
    Vector<int> expected(VectorConstants::maxDimensionsForSequentialArithmeticOps + 200, 2);

    EXPECT_TRUE(expected == result);
}

TEST(VectorTests, subtractEmptyVectors)
{
    Vector<int> v1{};
    Vector<int> v2{};

    Vector<int> result = v1 - v2;
    Vector<int> expected{};

    EXPECT_TRUE(expected == result);
}

TEST(VectorTests, subtractVectorsThrowsUnequalSizes)
{
    Vector<int> v1{7, 8, 9, 12};
    Vector<int> v2{2, 3, 4, 14, 7};

    EXPECT_THROW({
        try
        {
            Vector<int> result = v1 - v2;
        }
        catch(const std::runtime_error& e)
        {
            EXPECT_STREQ("unequal vector sizes.", e.what());
            throw;
        }
    }
    , std::runtime_error);
}

TEST(VectorTests, dotProduct)
{
    Vector<int> v1{7, 3, 9, 12};
    Vector<int> v2{2, 8, 4, 17};

    int result = dot_product(v1, v2);

    EXPECT_EQ(278, result);
}

TEST(VectorTests, dotProductParallel)
{
    Vector<int> v1(VectorConstants::maxDimensionsForSequentialDotProduct + 200, 3);
    Vector<int> v2(VectorConstants::maxDimensionsForSequentialDotProduct + 200, 2);

    int result = dot_product(v1, v2);
    int expected = 3 * 2 *  (VectorConstants::maxDimensionsForSequentialDotProduct + 200);

    EXPECT_EQ(expected, result);
}

TEST(VectorTests, dotProductUnequalVectors)
{
    Vector<int> v1{7, 8, 9, 12};
    Vector<int> v2{2, 3, 4, 14, 7};

    EXPECT_THROW({
        try
        {
            int result = dot_product(v1, v2);
        }
        catch(const std::runtime_error& e)
        {
            EXPECT_STREQ("unequal vector sizes.", e.what());
            throw;
        }
    }
    , std::runtime_error);
}

TEST(VectorTests, dotProductEmptyVectors)
{
    Vector<int> v1{};
    Vector<int> v2{};

    EXPECT_THROW({
        try
        {
            int result = dot_product(v1, v2);
        }
        catch(const std::runtime_error& e)
        {
            EXPECT_STREQ("cannot dot product null vectors.", e.what());
            throw;
        }
    }
    , std::runtime_error);
}

TEST(VectorTests, magnitudeTwoDimensions)
{
    Vector<int> v1{3, 4};
    Vector<int> v2{6, 8};
    EXPECT_EQ(5, v1.magnitude());
    EXPECT_EQ(10, v2.magnitude());
}

TEST(VectorTests, magnitudeThreeDimensions)
{
    Vector<int> v1{1, 2, 2};
    Vector<int> v2{2, 3, 6};
    EXPECT_EQ(3, v1.magnitude());
    EXPECT_EQ(7, v2.magnitude());
}

TEST(VectorTests, magnitudeRandom)
{
    Vector<int> v1{2, 19, 38, 12, 17, 4};
    double fuzzy_delta = std::abs(47.518 - v1.magnitude());
    EXPECT_TRUE(fuzzy_delta < .001);
}

TEST(VectorTests, magnitudeParallel)
{
    Vector<int> v1(6400, 1);
    EXPECT_EQ(80, v1.magnitude());
}

TEST(VectorTests, compareVectors_SameElementsDifferentOrder)
{
    Vector<int> v1{1, 2, 3, 4};
    Vector<int> v2{1, 3, 4, 2};
    EXPECT_FALSE(v1 == v2);
    EXPECT_TRUE(v1 != v2);
}

TEST(VectorTests, compareVectors_DifferentLengthsSameSubsequence)
{
    Vector<int> v1{1, 2, 3, 4};
    Vector<int> v2{1, 2, 3};
    EXPECT_FALSE(v1 == v2);
    EXPECT_TRUE(v1 != v2);
}

TEST(VectorTests, compareVectors_DifferentElementsSameLength)
{
    Vector<int> v1{1, 7, 3, 4};
    Vector<int> v2{8, 2, 9, 7};
    EXPECT_FALSE(v1 == v2);
    EXPECT_TRUE(v1 != v2);
}

TEST(VectorTests, compareVectors_Same)
{
    Vector<int> v1{1, 2, 3, 4};
    Vector<int> v2{1, 2, 3, 4};
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 != v2);
}

TEST(VectorTests, compareVectors_AdjustToSameVector)
{
    Vector<int> v1{1, 2, 3, 4};
    Vector<int> v2{1, 2, 5, 4};
    v2[2] = 3;

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 != v2);
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