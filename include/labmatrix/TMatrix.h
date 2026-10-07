#pragma once

#include "TMathVector.h"

#include <cstddef>
#include <initializer_list>

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
    void ensure_same_size(const TMatrix& other) const;
};

