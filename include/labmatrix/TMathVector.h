#pragma once

#include "TVector.h"

#include <cmath>
#include <cstddef>
#include <stdexcept>

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

private:
    void ensure_same_size(const TMathVector& other) const;
};

template <typename T>
TMathVector<T>::TMathVector(const std::size_t size, const T& value)
    : TVector<T>(size) {
    for (std::size_t i = 0; i < size; ++i) {
        this->push_back(value);
    }
}

template <typename T>
void TMathVector<T>::ensure_same_size(const TMathVector& other) const {
    if (this->get_size() != other.get_size()) {
        throw std::invalid_argument("vector sizes must be equal");
    }
}

template <typename T>
TMathVector<T> TMathVector<T>::operator+(const TMathVector& other) const {
    TMathVector result(*this);
    return result += other;
}

template <typename T>
TMathVector<T> TMathVector<T>::operator-(const TMathVector& other) const {
    TMathVector result(*this);
    return result -= other;
}

template <typename T>
TMathVector<T> TMathVector<T>::operator*(const T& scalar) const {
    TMathVector result(*this);
    return result *= scalar;
}

template <typename T>
TMathVector<T> TMathVector<T>::operator/(const T& scalar) const {
    TMathVector result(*this);
    return result /= scalar;
}

template <typename T>
T TMathVector<T>::operator*(const TMathVector& other) const {
    ensure_same_size(other);
    T result{};
    for (std::size_t i = 0; i < this->get_size(); ++i) {
        result += (*this)[i] * other[i];
    }
    return result;
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator+=(const TMathVector& other) {
    ensure_same_size(other);
    for (std::size_t i = 0; i < this->get_size(); ++i) {
        (*this)[i] += other[i];
    }
    return *this;
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator-=(const TMathVector& other) {
    ensure_same_size(other);
    for (std::size_t i = 0; i < this->get_size(); ++i) {
        (*this)[i] -= other[i];
    }
    return *this;
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator*=(const T& scalar) {
    for (std::size_t i = 0; i < this->get_size(); ++i) {
        (*this)[i] *= scalar;
    }
    return *this;
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator/=(const T& scalar) {
    if (scalar == T{}) {
        throw std::domain_error("division by zero");
    }
    for (std::size_t i = 0; i < this->get_size(); ++i) {
        (*this)[i] /= scalar;
    }
    return *this;
}

template <typename T>
double TMathVector<T>::length() const {
    long double squaredLength{};
    for (std::size_t i = 0; i < this->get_size(); ++i) {
        const long double value = static_cast<long double>((*this)[i]);
        squaredLength += value * value;
    }
    return std::sqrt(static_cast<double>(squaredLength));
}
