#pragma once

#include "TMathVector.h"

#include <cstddef>
#include <initializer_list>
#include <ostream>
#include <stdexcept>

// Upper triangular square matrix. Row i stores only columns i..size-1.
template <typename T>
class TTriangleMatrix {
public:
    using Row = TMathVector<T>;

    class ElementProxy {
    public:
        ElementProxy(TTriangleMatrix& matrix, std::size_t row, std::size_t column);
        ElementProxy& operator=(const T& value);
        ElementProxy& operator=(const ElementProxy& other);
        operator T() const;

    private:
        TTriangleMatrix& matrix_;
        std::size_t row_;
        std::size_t column_;
    };

    class RowProxy {
    public:
        RowProxy(TTriangleMatrix& matrix, std::size_t row);
        ElementProxy operator[](std::size_t column);

    private:
        TTriangleMatrix& matrix_;
        std::size_t row_;
    };

    class ConstRowProxy {
    public:
        ConstRowProxy(const TTriangleMatrix& matrix, std::size_t row);
        T operator[](std::size_t column) const;

    private:
        const TTriangleMatrix& matrix_;
        std::size_t row_;
    };

    TTriangleMatrix() = default;
    explicit TTriangleMatrix(std::size_t size);
    TTriangleMatrix(std::initializer_list<std::initializer_list<T>> rows);

    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] std::size_t stored_element_count() const noexcept;
    [[nodiscard]] std::size_t storage_row_size(std::size_t row) const;

    RowProxy operator[](std::size_t row);
    ConstRowProxy operator[](std::size_t row) const;
    [[nodiscard]] T at(std::size_t row, std::size_t column) const;
    void set(std::size_t row, std::size_t column, const T& value);

    [[nodiscard]] bool operator==(const TTriangleMatrix& other) const;
    [[nodiscard]] TTriangleMatrix operator+(const TTriangleMatrix& other) const;
    [[nodiscard]] TTriangleMatrix operator-(const TTriangleMatrix& other) const;
    [[nodiscard]] TTriangleMatrix operator*(const TTriangleMatrix& other) const;
    [[nodiscard]] TTriangleMatrix operator*(const T& scalar) const;

    friend std::ostream& operator<<(std::ostream& output, const TTriangleMatrix& matrix) {
        return matrix.write(output);
    }

private:
    TVector<Row> rows_;
    void check_row(std::size_t row) const;
    void check_index(std::size_t row, std::size_t column) const;
    void check_same_size(const TTriangleMatrix& other) const;
    std::ostream& write(std::ostream& output) const;
};

template <typename T>
TTriangleMatrix<T>::ElementProxy::ElementProxy(
        TTriangleMatrix& matrix, std::size_t row, std::size_t column)
    : matrix_(matrix), row_(row), column_(column) {}

template <typename T>
typename TTriangleMatrix<T>::ElementProxy&
TTriangleMatrix<T>::ElementProxy::operator=(const T& value) {
    matrix_.set(row_, column_, value);
    return *this;
}

template <typename T>
typename TTriangleMatrix<T>::ElementProxy&
TTriangleMatrix<T>::ElementProxy::operator=(const ElementProxy& other) {
    return *this = static_cast<T>(other);
}

template <typename T>
TTriangleMatrix<T>::ElementProxy::operator T() const {
    return matrix_.at(row_, column_);
}

template <typename T>
TTriangleMatrix<T>::RowProxy::RowProxy(TTriangleMatrix& matrix, std::size_t row)
    : matrix_(matrix), row_(row) {}

template <typename T>
typename TTriangleMatrix<T>::ElementProxy
TTriangleMatrix<T>::RowProxy::operator[](std::size_t column) {
    matrix_.check_index(row_, column);
    return ElementProxy(matrix_, row_, column);
}

template <typename T>
TTriangleMatrix<T>::ConstRowProxy::ConstRowProxy(
        const TTriangleMatrix& matrix, std::size_t row)
    : matrix_(matrix), row_(row) {}

template <typename T>
T TTriangleMatrix<T>::ConstRowProxy::operator[](std::size_t column) const {
    return matrix_.at(row_, column);
}

template <typename T>
TTriangleMatrix<T>::TTriangleMatrix(std::size_t size) : rows_(size) {
    for (std::size_t row = 0; row < size; ++row) {
        Row stored(size - row, T{});
        stored.shrink_to_fit();
        rows_.push_back(std::move(stored));
    }
    rows_.shrink_to_fit();
}

template <typename T>
TTriangleMatrix<T>::TTriangleMatrix(
        std::initializer_list<std::initializer_list<T>> rows)
    : rows_(rows.size()) {
    const std::size_t n = rows.size();
    std::size_t row_index = 0;
    for (const auto& values : rows) {
        if (values.size() != n) {
            throw std::invalid_argument("matrix must be square");
        }
        Row stored(n - row_index);
        std::size_t column = 0;
        for (const T& value : values) {
            if (column < row_index) {
                if (value != T{}) {
                    throw std::invalid_argument("elements below diagonal must be zero");
                }
            } else {
                stored.push_back(value);
            }
            ++column;
        }
        stored.shrink_to_fit();
        rows_.push_back(std::move(stored));
        ++row_index;
    }
    rows_.shrink_to_fit();
}

template <typename T>
std::size_t TTriangleMatrix<T>::size() const noexcept {
    return rows_.get_size();
}

template <typename T>
std::size_t TTriangleMatrix<T>::stored_element_count() const noexcept {
    return size() * (size() + 1) / 2;
}

template <typename T>
std::size_t TTriangleMatrix<T>::storage_row_size(std::size_t row) const {
    check_row(row);
    return rows_[row].get_size();
}

template <typename T>
void TTriangleMatrix<T>::check_row(std::size_t row) const {
    if (row >= size()) {
        throw std::out_of_range("matrix row is out of range");
    }
}

template <typename T>
void TTriangleMatrix<T>::check_index(std::size_t row, std::size_t column) const {
    check_row(row);
    if (column >= size()) {
        throw std::out_of_range("matrix column is out of range");
    }
}

template <typename T>
void TTriangleMatrix<T>::check_same_size(const TTriangleMatrix& other) const {
    if (size() != other.size()) {
        throw std::invalid_argument("matrix sizes must be equal");
    }
}

template <typename T>
typename TTriangleMatrix<T>::RowProxy TTriangleMatrix<T>::operator[](std::size_t row) {
    check_row(row);
    return RowProxy(*this, row);
}

template <typename T>
typename TTriangleMatrix<T>::ConstRowProxy
TTriangleMatrix<T>::operator[](std::size_t row) const {
    check_row(row);
    return ConstRowProxy(*this, row);
}

template <typename T>
T TTriangleMatrix<T>::at(std::size_t row, std::size_t column) const {
    check_index(row, column);
    return column < row ? T{} : rows_[row][column - row];
}

template <typename T>
void TTriangleMatrix<T>::set(std::size_t row, std::size_t column, const T& value) {
    check_index(row, column);
    if (column < row) {
        throw std::logic_error("cannot write below diagonal");
    }
    rows_[row][column - row] = value;
}

template <typename T>
bool TTriangleMatrix<T>::operator==(const TTriangleMatrix& other) const {
    return rows_ == other.rows_;
}

template <typename T>
TTriangleMatrix<T> TTriangleMatrix<T>::operator+(const TTriangleMatrix& other) const {
    check_same_size(other);
    TTriangleMatrix result(size());
    for (std::size_t row = 0; row < size(); ++row) {
        result.rows_[row] = rows_[row] + other.rows_[row];
    }
    return result;
}

template <typename T>
TTriangleMatrix<T> TTriangleMatrix<T>::operator-(const TTriangleMatrix& other) const {
    check_same_size(other);
    TTriangleMatrix result(size());
    for (std::size_t row = 0; row < size(); ++row) {
        result.rows_[row] = rows_[row] - other.rows_[row];
    }
    return result;
}

template <typename T>
TTriangleMatrix<T> TTriangleMatrix<T>::operator*(const TTriangleMatrix& other) const {
    check_same_size(other);
    TTriangleMatrix result(size());
    for (std::size_t row = 0; row < size(); ++row) {
        for (std::size_t column = row; column < size(); ++column) {
            T value{};
            for (std::size_t index = row; index <= column; ++index) {
                value += rows_[row][index - row] * other.rows_[index][column - index];
            }
            result.rows_[row][column - row] = value;
        }
    }
    return result;
}

template <typename T>
TTriangleMatrix<T> TTriangleMatrix<T>::operator*(const T& scalar) const {
    TTriangleMatrix result(*this);
    for (std::size_t row = 0; row < size(); ++row) {
        result.rows_[row] *= scalar;
    }
    return result;
}

template <typename T>
std::ostream& TTriangleMatrix<T>::write(std::ostream& output) const {
    for (std::size_t row = 0; row < size(); ++row) {
        output << "[ ";
        for (std::size_t column = 0; column < size(); ++column) {
            output << at(row, column) << ' ';
        }
        output << ']';
        if (row + 1 < size()) {
            output << '\n';
        }
    }
    return output;
}
