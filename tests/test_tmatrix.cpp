#include <gtest/gtest.h>

#include "labmatrix/TMatrix.h"

#include <type_traits>

TEST(TMatrixDeclarationTest, InheritsMathVectorOfRows) {
    static_assert(std::is_base_of_v<
                  TMathVector<TMathVector<int>>,
                  TMatrix<int>>);
    SUCCEED();
}

TEST(TMatrixInheritedTest, StoresRowsAndProvidesIndexing) {
    TMatrix<int> matrix;
    matrix.push_back(TMatrix<int>::Row{1, 2});
    matrix.push_back(TMatrix<int>::Row{3, 4});

    ASSERT_EQ(matrix.get_size(), 2);
    EXPECT_EQ(matrix[0][1], 2);
    EXPECT_EQ(matrix[1][0], 3);
}

TEST(TMatrixInheritedTest, CopiesRowsIndependently) {
    TMatrix<int> matrix;
    matrix.push_back(TMatrix<int>::Row{1, 2});
    matrix.push_back(TMatrix<int>::Row{3, 4});

    TMatrix<int> copy = matrix;
    copy[0][0] = 99;

    EXPECT_EQ(matrix[0][0], 1);
    EXPECT_EQ(copy[0][0], 99);
}

TEST(TMatrixInheritedTest, ShrinksRowStorage) {
    TMatrix<int> matrix;
    matrix.push_back(TMatrix<int>::Row{1, 0});
    matrix.push_back(TMatrix<int>::Row{0, 1});

    ASSERT_EQ(matrix.get_capacity(), 15);
    matrix.shrink_to_fit();

    EXPECT_EQ(matrix.get_size(), 2);
    EXPECT_EQ(matrix.get_capacity(), 2);
}
