#include "labmatrix/TMatrix.h"
#include "labmatrix/TTriangleMatrix.h"

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace {

template <typename Matrix>
Matrix read_matrix(std::istream& input, std::size_t size) {
    Matrix matrix(size);
    for (std::size_t row = 0; row < size; ++row) {
        for (std::size_t column = 0; column < size; ++column) {
            double value{};
            if (!(input >> value)) {
                throw std::runtime_error("not enough matrix elements");
            }
            if constexpr (std::is_same_v<Matrix, TTriangleMatrix<double>>) {
                if (column < row) {
                    if (value != 0.0) {
                        throw std::invalid_argument("triangle matrix must be zero below diagonal");
                    }
                    continue;
                }
            }
            matrix[row][column] = value;
        }
    }
    return matrix;
}

template <typename Matrix>
void print_matrix(std::ostream& output, const Matrix& matrix) {
    for (std::size_t row = 0; row < matrix.size(); ++row) {
        output << "[ ";
        for (std::size_t column = 0; column < matrix.size(); ++column) {
            output << matrix[row][column] << ' ';
        }
        output << "]\n";
    }
}

template <typename Matrix>
void run_calculation(std::istream& input, std::ostream& output,
                     const std::string& operation, std::size_t size) {
    output << "Enter matrix A, " << size * size << " values row by row:\n";
    Matrix a = read_matrix<Matrix>(input, size);

    if (operation == "add" || operation == "subtract" || operation == "multiply") {
        output << "Enter matrix B, " << size * size << " values row by row:\n";
        Matrix b = read_matrix<Matrix>(input, size);
        if (operation == "add") {
            print_matrix(output, a + b);
        } else if (operation == "subtract") {
            print_matrix(output, a - b);
        } else {
            print_matrix(output, a * b);
        }
    } else if (operation == "scale") {
        output << "Scalar: ";
        double scalar{};
        if (!(input >> scalar)) {
            throw std::runtime_error("invalid scalar");
        }
        print_matrix(output, a * scalar);
    } else if (operation == "trace") {
        double trace{};
        for (std::size_t index = 0; index < size; ++index) {
            trace += a[index][index];
        }
        output << trace << '\n';
    } else if (operation == "transpose") {
        if constexpr (std::is_same_v<Matrix, TMatrix<double>>) {
            print_matrix(output, a.transposed());
        } else {
            throw std::invalid_argument("transpose is available only for square matrices");
        }
    } else {
        throw std::invalid_argument("unknown operation");
    }
}

} // namespace

int main() {
    try {
        std::cout << "Matrix calculator\n"
                  << "Type (square/triangle): ";
        std::string type;
        if (!(std::cin >> type) || (type != "square" && type != "triangle")) {
            throw std::invalid_argument("choose square or triangle");
        }

        std::cout << "Operation (add/subtract/multiply/scale/trace/transpose): ";
        std::string operation;
        if (!(std::cin >> operation)) {
            throw std::runtime_error("missing operation");
        }
        if (operation != "add" && operation != "subtract" &&
            operation != "multiply" && operation != "scale" &&
            operation != "trace" && operation != "transpose") {
            throw std::invalid_argument("unknown operation");
        }
        if (type == "triangle" && operation == "transpose") {
            throw std::invalid_argument("transpose is available only for square matrices");
        }

        std::cout << "Size: ";
        std::size_t size{};
        if (!(std::cin >> size) || size == 0 || size > 100) {
            throw std::invalid_argument("size must be between 1 and 100");
        }

        if (type == "square") {
            run_calculation<TMatrix<double>>(std::cin, std::cout, operation, size);
        } else {
            run_calculation<TTriangleMatrix<double>>(std::cin, std::cout, operation, size);
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}

