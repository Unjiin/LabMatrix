#pragma once

#include "TMathVector.h"

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

template <typename T>
class TMatrix : public TMathVector<TMathVector<T>> {
public:
    using Row = TMathVector<T>;
    using Base = TMathVector<Row>;

    TMatrix() = default;
    explicit TMatrix(std::size_t size, const T& value = T{});
    TMatrix(std::initializer_list<std::initializer_list<T>> rows);

    [[nodiscard]] std::size_t size() const noexcept;

    [[nodiscard]] TMatrix operator+(const TMatrix& other) const;
    [[nodiscard]] TMatrix operator-(const TMatrix& other) const;
    [[nodiscard]] TMatrix operator*(const TMatrix& other) const;
    [[nodiscard]] TMatrix operator*(const T& scalar) const;

    TMatrix& operator+=(const TMatrix& other);
    TMatrix& operator-=(const TMatrix& other);
    TMatrix& operator*=(const T& scalar);

    [[nodiscard]] TMatrix transposed() const;
    [[nodiscard]] T trace() const;
    [[nodiscard]] static TMatrix identity(std::size_t size);

private:
    void ensure_square() const;
    void ensure_same_size(const TMatrix& other) const;
};

template <typename T>
TMatrix<T>::TMatrix(const std::size_t size, const T& value)
    : Base(size) {
    for (std::size_t row = 0; row < size; ++row) {
        this->push_back(Row(size, value));
    }
}

template <typename T>
TMatrix<T>::TMatrix(
        std::initializer_list<std::initializer_list<T>> rows)
    : Base(rows.size()) {
    const std::size_t matrixSize = rows.size();
    for (const auto& values : rows) {
        if (values.size() != matrixSize) {
            throw std::invalid_argument("matrix must be square");
        }
        Row row(matrixSize);
        for (const T& value : values) {
            row.push_back(value);
        }
        this->push_back(std::move(row));
    }
}

template <typename T>
std::size_t TMatrix<T>::size() const noexcept {
    return this->get_size();
}

template <typename T>
void TMatrix<T>::ensure_square() const {
    for (std::size_t row = 0; row < size(); ++row) {
        if ((*this)[row].get_size() != size()) {
            throw std::logic_error("matrix must be square");
        }
    }
}

template <typename T>
void TMatrix<T>::ensure_same_size(const TMatrix& other) const {
    ensure_square();
    other.ensure_square();
    if (size() != other.size()) {
        throw std::invalid_argument("matrix sizes must be equal");
    }
}

template <typename T>
TMatrix<T> TMatrix<T>::operator+(const TMatrix& other) const {
    TMatrix result(*this);
    return result += other;
}

template <typename T>
TMatrix<T> TMatrix<T>::operator-(const TMatrix& other) const {
    TMatrix result(*this);
    return result -= other;
}

template <typename T>
TMatrix<T> TMatrix<T>::operator*(const TMatrix& other) const {
    ensure_same_size(other);
    TMatrix result(size(), T{});
    for (std::size_t row = 0; row < size(); ++row) {
        for (std::size_t column = 0; column < size(); ++column) {
            for (std::size_t index = 0; index < size(); ++index) {
                result[row][column] +=
                    (*this)[row][index] * other[index][column];
            }
        }
    }
    return result;
}

template <typename T>
TMatrix<T> TMatrix<T>::operator*(const T& scalar) const {
    TMatrix result(*this);
    return result *= scalar;
}

template <typename T>
TMatrix<T>& TMatrix<T>::operator+=(const TMatrix& other) {
    ensure_same_size(other);
    for (std::size_t row = 0; row < size(); ++row) {
        (*this)[row] += other[row];
    }
    return *this;
}

template <typename T>
TMatrix<T>& TMatrix<T>::operator-=(const TMatrix& other) {
    ensure_same_size(other);
    for (std::size_t row = 0; row < size(); ++row) {
        (*this)[row] -= other[row];
    }
    return *this;
}

template <typename T>
TMatrix<T>& TMatrix<T>::operator*=(const T& scalar) {
    ensure_square();
    for (std::size_t row = 0; row < size(); ++row) {
        (*this)[row] *= scalar;
    }
    return *this;
}

template <typename T>
TMatrix<T> TMatrix<T>::transposed() const {
    ensure_square();
    TMatrix result(size(), T{});
    for (std::size_t row = 0; row < size(); ++row) {
        for (std::size_t column = 0; column < size(); ++column) {
            result[column][row] = (*this)[row][column];
        }
    }
    return result;
}

template <typename T>
T TMatrix<T>::trace() const {
    ensure_square();
    T result{};
    for (std::size_t index = 0; index < size(); ++index) {
        result += (*this)[index][index];
    }
    return result;
}

template <typename T>
TMatrix<T> TMatrix<T>::identity(const std::size_t size) {
    TMatrix result(size, T{});
    for (std::size_t index = 0; index < size; ++index) {
        result[index][index] = T{1};
    }
    return result;
}
