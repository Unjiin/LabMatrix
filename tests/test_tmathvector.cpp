#include <gtest/gtest.h>

#include "labmatrix/TMathVector.h"

#include <type_traits>

TEST(TMathVectorDeclarationTest, InheritsTVector) {
    static_assert(std::is_base_of_v<TVector<int>, TMathVector<int>>);
    SUCCEED();
}

