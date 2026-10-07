#pragma once

#include "TVector.h"

#include <cstddef>

template <typename T>
class TMathVector : public TVector<T> {
public:
    using TVector<T>::TVector;

    TMathVector() = default;
    explicit TMathVector(std::size_t size, const T& value);

    [[nodiscard]] TMathVector operator+(const TMathVector& other) const;
    [[nodiscard]] TMathVector operator-(const TMathVector& other) const;
    [[nodiscard]] TMathVector operator*(const T& scalar) const;
    [[nodiscard]] TMathVector operator/(const T& scalar) const;
    [[nodiscard]] T operator*(const TMathVector& other) const;

    TMathVector& operator+=(const TMathVector& other);
    TMathVector& operator-=(const TMathVector& other);
    TMathVector& operator*=(const T& scalar);
    TMathVector& operator/=(const T& scalar);

    [[nodiscard]] double length() const;
};

