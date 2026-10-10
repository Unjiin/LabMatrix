#pragma once

#include "TMathVector.h"

#include <cstddef>
#include <initializer_list>
#include <iosfwd>

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
