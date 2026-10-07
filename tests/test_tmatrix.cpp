#include <gtest/gtest.h>

#include "labmatrix/TMatrix.h"

#include <stdexcept>
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

TEST(TMatrixTest, CreatesFilledSquareMatrix) {
    const TMatrix<int> matrix(2, 7);

    EXPECT_EQ(matrix.size(), 2);
    EXPECT_EQ(matrix[0], (TMathVector<int>{7, 7}));
    EXPECT_EQ(matrix[1], (TMathVector<int>{7, 7}));
}

TEST(TMatrixTest, CreatesFromInitializerLists) {
    const TMatrix<int> matrix{{1, 2}, {3, 4}};

    EXPECT_EQ(matrix[0][0], 1);
    EXPECT_EQ(matrix[1][1], 4);
}

TEST(TMatrixTest, RejectsNonSquareInitializer) {
    EXPECT_THROW((TMatrix<int>{{1, 2, 3}, {4, 5, 6}}),
                 std::invalid_argument);
}

TEST(TMatrixTest, AddsAndSubtractsMatrices) {
    const TMatrix<int> left{{1, 2}, {3, 4}};
    const TMatrix<int> right{{4, 3}, {2, 1}};

    EXPECT_EQ(left + right, (TMatrix<int>{{5, 5}, {5, 5}}));
    EXPECT_EQ(left - right, (TMatrix<int>{{-3, -1}, {1, 3}}));
}

TEST(TMatrixTest, MultipliesMatrices) {
    const TMatrix<int> left{{1, 2}, {3, 4}};
    const TMatrix<int> right{{5, 6}, {7, 8}};

    EXPECT_EQ(left * right, (TMatrix<int>{{19, 22}, {43, 50}}));
}

TEST(TMatrixTest, MultipliesByScalar) {
    const TMatrix<int> matrix{{1, 2}, {3, 4}};

    EXPECT_EQ(matrix * 3, (TMatrix<int>{{3, 6}, {9, 12}}));
}

TEST(TMatrixTest, TransposesMatrix) {
    const TMatrix<int> matrix{{1, 2}, {3, 4}};

    EXPECT_EQ(matrix.transposed(), (TMatrix<int>{{1, 3}, {2, 4}}));
}

TEST(TMatrixTest, CalculatesTrace) {
    const TMatrix<int> matrix{{1, 2}, {3, 4}};

    EXPECT_EQ(matrix.trace(), 5);
}

TEST(TMatrixTest, CreatesIdentityMatrix) {
    EXPECT_EQ(TMatrix<int>::identity(3),
              (TMatrix<int>{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}));
}

TEST(TMatrixTest, RejectsDifferentMatrixSizes) {
    const TMatrix<int> small{{1, 0}, {0, 1}};
    const TMatrix<int> large = TMatrix<int>::identity(3);

    EXPECT_THROW(static_cast<void>(small + large), std::invalid_argument);
    EXPECT_THROW(static_cast<void>(small * large), std::invalid_argument);
}

TEST(TMatrixTest, RejectsNonSquareStateCreatedThroughBaseMethods) {
    TMatrix<int> matrix;
    matrix.push_back(TMatrix<int>::Row{1, 2, 3});
    matrix.push_back(TMatrix<int>::Row{4, 5, 6});

    EXPECT_THROW(static_cast<void>(matrix.trace()), std::logic_error);
}
