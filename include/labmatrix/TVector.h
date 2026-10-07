#pragma once

#include "TMemData.h"

#include <cstddef>
#include <initializer_list>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <utility>

template <typename T>
class TVector {
public:
    explicit TVector(const std::size_t reserveCapacity = 0)
        : memory_(reserveCapacity) {}

    TVector(std::initializer_list<T> values)
        : memory_(values) {}

    TVector(const T* values, const std::size_t size)
        : memory_(values, size) {}

    TVector(const TVector&) = default;
    TVector(TVector&&) noexcept = default;
    virtual ~TVector() = default;

    TVector& operator=(const TVector&) = default;
    TVector& operator=(TVector&&) noexcept = default;

    [[nodiscard]] bool is_empty() const noexcept {
        return memory_.is_empty();
    }

    [[nodiscard]] bool is_full() const noexcept {
        return memory_.is_full();
    }

    [[nodiscard]] std::size_t get_size() const noexcept {
        return memory_.get_size();
    }

    [[nodiscard]] std::size_t get_capacity() const noexcept {
        return memory_.get_capacity();
    }

    [[nodiscard]] const T& get_front() const {
        ensure_not_empty();
        return memory_.data_[0];
    }

    [[nodiscard]] const T& get_back() const {
        ensure_not_empty();
        return memory_.data_[memory_.size_ - 1];
    }

    T& front_ref() {
        ensure_not_empty();
        return memory_.data_[0];
    }

    T& back_ref() {
        ensure_not_empty();
        return memory_.data_[memory_.size_ - 1];
    }

    void push_front(const T& value) {
        insert(value, 0);
    }

    void push_back(const T& value) {
        memory_.ensure_capacity(memory_.size_ + 1);
        memory_.data_[memory_.size_++] = value;
    }

    void push_back(T&& value) {
        memory_.ensure_capacity(memory_.size_ + 1);
        memory_.data_[memory_.size_++] = std::move(value);
    }

    void insert(const T& value, const std::size_t position) {
        if (position > memory_.size_) {
            throw std::out_of_range("insert position is out of range");
        }
        memory_.ensure_capacity(memory_.size_ + 1);
        for (std::size_t i = memory_.size_; i > position; --i) {
            memory_.data_[i] = std::move(memory_.data_[i - 1]);
        }
        memory_.data_[position] = value;
        ++memory_.size_;
    }

    void pop_front() {
        erase(0);
    }

    void pop_back() {
        ensure_not_empty();
        --memory_.size_;
    }

    void erase(const std::size_t position) {
        ensure_not_empty();
        if (position >= memory_.size_) {
            throw std::out_of_range("erase position is out of range");
        }
        for (std::size_t i = position; i + 1 < memory_.size_; ++i) {
            memory_.data_[i] = std::move(memory_.data_[i + 1]);
        }
        --memory_.size_;
    }

    void clear() noexcept {
        memory_.size_ = 0;
    }

    void shrink_to_fit() {
        if (memory_.capacity_ != memory_.size_) {
            memory_.reallocate(memory_.size_);
        }
    }

    [[nodiscard]] const T& operator[](const std::size_t index) const noexcept {
        return memory_.data_[index];
    }

    [[nodiscard]] T& operator[](const std::size_t index) noexcept {
        return memory_.data_[index];
    }

    [[nodiscard]] const T& at(const std::size_t index) const {
        if (index >= memory_.size_) {
            throw std::out_of_range("vector index is out of range");
        }
        return memory_.data_[index];
    }

    [[nodiscard]] T& at(const std::size_t index) {
        if (index >= memory_.size_) {
            throw std::out_of_range("vector index is out of range");
        }
        return memory_.data_[index];
    }

    [[nodiscard]] bool operator==(const TVector& other) const {
        return memory_ == other.memory_;
    }

    friend std::ostream& operator<<(std::ostream& output,
                                    const TVector& vector) {
        output << "{ ";
        for (std::size_t i = 0; i < vector.get_size(); ++i) {
            output << vector[i];
            if (i + 1 != vector.get_size()) {
                output << ", ";
            }
        }
        return output << " }";
    }

    friend std::istream& operator>>(std::istream& input, TVector& vector) {
        std::size_t size{};
        if (!(input >> size)) {
            return input;
        }
        vector.clear();
        vector.memory_.ensure_capacity(size);
        for (std::size_t i = 0; i < size; ++i) {
            T value{};
            if (!(input >> value)) {
                throw std::invalid_argument("not enough vector elements");
            }
            vector.push_back(std::move(value));
        }
        return input;
    }

protected:
    TMemData<T> memory_;

private:
    void ensure_not_empty() const {
        if (is_empty()) {
            throw std::logic_error("vector is empty");
        }
    }
};
