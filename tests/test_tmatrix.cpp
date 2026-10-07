#include <gtest/gtest.h>

#include "labmatrix/TMatrix.h"

#include <type_traits>

TEST(TMatrixDeclarationTest, InheritsMathVectorOfRows) {
    static_assert(std::is_base_of_v<
                  TMathVector<TMathVector<int>>,
                  TMatrix<int>>);
    SUCCEED();
}

