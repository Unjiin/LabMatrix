#include <gtest/gtest.h>

#include "labmatrix/TVector.h"

#include <sstream>
#include <stdexcept>

TEST(TMemDataTest, CalculatesCapacityByBlocks) {
    EXPECT_EQ(calculate_capacity(0), 15);
    EXPECT_EQ(calculate_capacity(14), 15);
    EXPECT_EQ(calculate_capacity(15), 30);
}

TEST(TVectorTest, CreatesFromInitializerList) {
    const TVector<int> vector{1, 2, 3};

    EXPECT_EQ(vector.get_size(), 3);
    EXPECT_EQ(vector.get_capacity(), 15);
    EXPECT_EQ(vector[0], 1);
    EXPECT_EQ(vector[2], 3);
}

TEST(TVectorTest, SupportsPushInsertAndErase) {
    TVector<int> vector{2, 4};

    vector.push_front(1);
    vector.insert(3, 2);
    vector.push_back(5);
    vector.erase(1);

    EXPECT_EQ(vector, (TVector<int>{1, 3, 4, 5}));
}

TEST(TVectorTest, GrowsCapacity) {
    TVector<int> vector;
    for (int value = 0; value < 16; ++value) {
        vector.push_back(value);
    }

    EXPECT_EQ(vector.get_size(), 16);
    EXPECT_EQ(vector.get_capacity(), 30);
}

TEST(TVectorTest, ThrowsForInvalidAccess) {
    TVector<int> vector;

    EXPECT_THROW(static_cast<void>(vector.get_front()), std::logic_error);
    EXPECT_THROW(vector.pop_back(), std::logic_error);
    EXPECT_THROW(static_cast<void>(vector.at(0)), std::out_of_range);
}

TEST(TVectorTest, ReadsAndWritesStream) {
    TVector<int> vector;
    std::istringstream input("3 10 20 30");
    input >> vector;

    std::ostringstream output;
    output << vector;

    EXPECT_EQ(output.str(), "{ 10, 20, 30 }");
}

TEST(TVectorTest, CopiesIndependently) {
    TVector<int> original{1, 2, 3};
    TVector<int> copy = original;
    copy[0] = 99;

    EXPECT_EQ(original[0], 1);
    EXPECT_EQ(copy[0], 99);
}
