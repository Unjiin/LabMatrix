#include <gtest/gtest.h>

#include "labmatrix/TMathVector.h"

#include <stdexcept>
#include <type_traits>

TEST(TMathVectorDeclarationTest, InheritsTVector) {
    static_assert(std::is_base_of_v<TVector<int>, TMathVector<int>>);
    SUCCEED();
}

TEST(TMathVectorTest, CreatesFilledVector) {
    const TMathVector<int> vector(3, 7);

    EXPECT_EQ(vector, (TMathVector<int>{7, 7, 7}));
}

TEST(TMathVectorTest, AddsAndSubtractsVectors) {
    const TMathVector<int> left{1, 2, 3};
    const TMathVector<int> right{4, 5, 6};

    EXPECT_EQ(left + right, (TMathVector<int>{5, 7, 9}));
    EXPECT_EQ(right - left, (TMathVector<int>{3, 3, 3}));
}

TEST(TMathVectorTest, MultipliesAndDividesByScalar) {
    const TMathVector<double> vector{2.0, 4.0, 6.0};

    EXPECT_EQ(vector * 2.0, (TMathVector<double>{4.0, 8.0, 12.0}));
    EXPECT_EQ(vector / 2.0, (TMathVector<double>{1.0, 2.0, 3.0}));
}

TEST(TMathVectorTest, CalculatesDotProductAndLength) {
    const TMathVector<int> left{1, 2, 3};
    const TMathVector<int> right{4, 5, 6};
    const TMathVector<int> pythagorean{3, 4};

    EXPECT_EQ(left * right, 32);
    EXPECT_DOUBLE_EQ(pythagorean.length(), 5.0);
}

TEST(TMathVectorTest, RejectsDifferentSizes) {
    const TMathVector<int> shortVector{1, 2};
    const TMathVector<int> longVector{1, 2, 3};

    EXPECT_THROW(static_cast<void>(shortVector + longVector),
                 std::invalid_argument);
    EXPECT_THROW(static_cast<void>(shortVector * longVector),
                 std::invalid_argument);
}

TEST(TMathVectorTest, RejectsDivisionByZero) {
    const TMathVector<double> vector{1.0, 2.0};

    EXPECT_THROW(static_cast<void>(vector / 0.0), std::domain_error);
}
