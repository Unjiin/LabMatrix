#include "labmatrix/TVector.h"

#include <iostream>

int main() {
    const TVector<int> values{1, 2, 3};
    std::cout << values << '\n';
    return 0;
}

