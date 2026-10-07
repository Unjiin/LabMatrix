#pragma once

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>

inline constexpr std::size_t MEM_STEP = 15;

[[nodiscard]] constexpr std::size_t calculate_capacity(
        const std::size_t size) noexcept {
    return (size / MEM_STEP + 1) * MEM_STEP;
}

template <typename T>
class TVector;

template <typename T>
class TMemData {
public:
    explicit TMemData(const std::size_t requestedCapacity = 0)
        : capacity_(calculate_capacity(requestedCapacity)),
          data_(std::make_unique<T[]>(capacity_)) {}

    TMemData(std::initializer_list<T> values)
        : TMemData(values.begin(), values.size()) {}

    TMemData(const T* values, const std::size_t size)
        : size_(size),
          capacity_(calculate_capacity(size)),
          data_(std::make_unique<T[]>(capacity_)) {
        if (values == nullptr && size != 0) {
            throw std::invalid_argument("values must not be null");
        }
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = values[i];
        }
    }

    TMemData(const TMemData& other)
        : size_(other.size_),
          capacity_(other.capacity_),
          data_(capacity_ == 0 ? nullptr : std::make_unique<T[]>(capacity_)) {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    TMemData(TMemData&&) noexcept = default;
    ~TMemData() = default;

    TMemData& operator=(const TMemData& other) {
        if (this == &other) {
            return *this;
        }
        TMemData copy(other);
        swap(copy);
        return *this;
    }

    TMemData& operator=(TMemData&&) noexcept = default;

    void swap(TMemData& other) noexcept {
        using std::swap;
        swap(size_, other.size_);
        swap(capacity_, other.capacity_);
        swap(data_, other.data_);
    }

    [[nodiscard]] bool is_empty() const noexcept {
        return size_ == 0;
    }

    [[nodiscard]] bool is_full() const noexcept {
        return size_ == capacity_;
    }

    [[nodiscard]] std::size_t get_size() const noexcept {
        return size_;
    }

    [[nodiscard]] std::size_t get_capacity() const noexcept {
        return capacity_;
    }

    [[nodiscard]] const T* get_data_const() const noexcept {
        return data_.get();
    }

    [[nodiscard]] T* get_data_changeable() noexcept {
        return data_.get();
    }

    void set_size(const std::size_t size) {
        if (size > capacity_) {
            throw std::invalid_argument("size is greater than capacity");
        }
        size_ = size;
    }

    void clear_memory() {
        size_ = 0;
        capacity_ = MEM_STEP;
        data_ = std::make_unique<T[]>(capacity_);
    }

    [[nodiscard]] bool operator==(const TMemData& other) const {
        if (size_ != other.size_) {
            return false;
        }
        for (std::size_t i = 0; i < size_; ++i) {
            if (data_[i] != other.data_[i]) {
                return false;
            }
        }
        return true;
    }

private:
    std::size_t size_{};
    std::size_t capacity_{};
    std::unique_ptr<T[]> data_;

    void reallocate(const std::size_t newCapacity) {
        auto newData = newCapacity == 0
            ? nullptr
            : std::make_unique<T[]>(newCapacity);
        const std::size_t copyCount =
            size_ < newCapacity ? size_ : newCapacity;
        for (std::size_t i = 0; i < copyCount; ++i) {
            newData[i] = std::move_if_noexcept(data_[i]);
        }
        data_ = std::move(newData);
        capacity_ = newCapacity;
        size_ = copyCount;
    }

    void ensure_capacity(const std::size_t requiredCapacity) {
        if (requiredCapacity <= capacity_) {
            return;
        }
        reallocate(calculate_capacity(requiredCapacity));
    }

    template <typename>
    friend class TVector;
};

