#include <gtest/gtest.h>

#include "labmatrix/TTriangleMatrix.h"

#include <sstream>
#include <stdexcept>

TEST(TTriangleMatrixTest, StoresOnlyUpperTriangle) {
    const TTriangleMatrix<int> matrix{{3, 4, 3, 0, 0},
                                      {0, 4, 3, 5, 1},
                                      {0, 0, 3, 5, 1},
                                      {0, 0, 0, 5, 1},
                                      {0, 0, 0, 0, 1}};
    EXPECT_EQ(matrix.size(), 5);
    EXPECT_EQ(matrix.stored_element_count(), 15);
    for (std::size_t row = 0; row < matrix.size(); ++row) {
        EXPECT_EQ(matrix.storage_row_size(row), 5 - row);
    }
    EXPECT_EQ(matrix[4][1], 0);
    EXPECT_EQ(matrix[2][3], 5);
}

TEST(TTriangleMatrixTest, WritesUpperElementsAndPrintsDenseMatrix) {
    TTriangleMatrix<int> matrix{{3, 4, 3}, {0, 4, 3}, {0, 0, 3}};
    matrix[1][2] = -6;
    EXPECT_EQ(matrix[1][2], -6);

    std::ostringstream output;
    output << matrix;
    EXPECT_EQ(output.str(), "[ 3 4 3 ]\n[ 0 4 -6 ]\n[ 0 0 3 ]");
}

TEST(TTriangleMatrixTest, RejectsWritingBelowDiagonal) {
    TTriangleMatrix<int> matrix{{1, 2}, {0, 3}};
    EXPECT_EQ(matrix[1][0], 0);
    EXPECT_THROW(matrix[1][0] = 5, std::logic_error);
    EXPECT_EQ(matrix[1][0], 0);
}

TEST(TTriangleMatrixTest, RejectsInvalidShapesAndIndices) {
    EXPECT_THROW((TTriangleMatrix<int>{{1, 2}, {0}}), std::invalid_argument);
    EXPECT_THROW((TTriangleMatrix<int>{{1, 2}, {3, 4}}), std::invalid_argument);
    TTriangleMatrix<int> matrix(2);
    EXPECT_THROW(matrix[2][0], std::out_of_range);
    EXPECT_THROW(matrix[0][2], std::out_of_range);
    EXPECT_THROW(static_cast<void>(matrix.storage_row_size(2)), std::out_of_range);
}

TEST(TTriangleMatrixTest, CopiesIndependently) {
    TTriangleMatrix<int> original{{1, 2}, {0, 3}};
    TTriangleMatrix<int> copy = original;
    copy[0][1] = 9;
    EXPECT_EQ(original[0][1], 2);
    EXPECT_EQ(copy[0][1], 9);
}

TEST(TTriangleMatrixTest, AddsSubtractsAndScales) {
    const TTriangleMatrix<int> a{{1, 2}, {0, 3}};
    const TTriangleMatrix<int> b{{4, 5}, {0, 6}};
    EXPECT_EQ(a + b, (TTriangleMatrix<int>{{5, 7}, {0, 9}}));
    EXPECT_EQ(b - a, (TTriangleMatrix<int>{{3, 3}, {0, 3}}));
    EXPECT_EQ(a * 2, (TTriangleMatrix<int>{{2, 4}, {0, 6}}));
    EXPECT_THROW(static_cast<void>(a + TTriangleMatrix<int>(3)), std::invalid_argument);
}

TEST(TTriangleMatrixTest, MultipliesWithoutDenseStorage) {
    const TTriangleMatrix<int> a{{1, 2, 3}, {0, 4, 5}, {0, 0, 6}};
    const TTriangleMatrix<int> b{{7, 8, 9}, {0, 10, 11}, {0, 0, 12}};
    EXPECT_EQ(a * b, (TTriangleMatrix<int>{{7, 28, 67},
                                          {0, 40, 104},
                                          {0, 0, 72}}));
    EXPECT_THROW(static_cast<void>(a * TTriangleMatrix<int>(2)), std::invalid_argument);
}
