#include <cstdio>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>

namespace phundrak {
using size_type = size_t;

template <class T, class Allocator = std::allocator<T>> class vector {

private:
  void double_capacity() {
    if (data_) {
      T *olddata = data_;
      capacity_ <<= 1;
      data_ = new T[capacity_];
      for (size_t i = 0; i < size_; ++i)
        data_[i] = olddata[i];
      delete[] olddata;
    } else {
      data_ = new T[1];
      capacity_ = 1;
    }
  }

  T *data_;
  size_t size_;
  size_t capacity_;
  Allocator alloc_;

public:
  ///////////////////////////////////////////////////////////////////////////
  //                            Member functions                           //
  ///////////////////////////////////////////////////////////////////////////

  // constructor ////////////////////////////////////////////////////////////

  vector() noexcept(noexcept(Allocator()))
      : vector{Allocator()} {}

  explicit vector(const Allocator &alloc) noexcept
      : data_{nullptr}, size_{0}, capacity_{0}, alloc_{alloc} {}

  vector(size_type count, const T &value, const Allocator &alloc = Allocator())
      : vector{alloc}, data_{nullptr}, size_{0}, capacity_{0} {
    for (size_t i = 0; i < count; ++i)
      push_back(value);
  }

  explicit vector(size_type count, const Allocator &alloc = Allocator())
      : data_{nullptr}, size_{0}, capacity_{0}, alloc_{alloc} {
    for (size_type i = 0; i < count; ++i)
      push_back(T{});
  }

  template <class InputIt>
  vector(InputIt first, InputIt last, const Allocator &alloc = Allocator())
      : data_{nullptr}, size_{0}, capacity_{0}, alloc_{alloc} {
    for (; first != last; ++first)
      push_back(*first);
  }

  // Copy constructor ///////////////////////////////////////////////////////

  vector(const vector<T> &other)
      : data_{new T[other.size_]}, size_{other.size_},
        capacity_{other.capacity_}, alloc_{other.alloc_} {
    if (!alloc_) {
      alloc_ = std::allocator_traits<Allocator>::
          select_on_container_copy_construction(other.get_allocator());
    }
    for (size_type i = 0; i < size_; ++i)
      data_[i] = other.data_[i];
  }

  vector(const vector &other, const Allocator &alloc)
      : data_{new T[other.size_]}, size_{other.size},
        capacity_{other.capacity_}, alloc_{alloc} {
    try {
      if (alloc_ != other.alloc_)
        throw 20;
    } catch (int error) {
      std::cout << "Error in phundrak::vector(const vector &other, const "
                   "Allocator &alloc) :\nThe allocator "
                << alloc
                << " passed as argument is different from other’s allocator "
                << other.alloc_ << "\nAborting...\n";
      std::terminate();
    }

    for (size_type i = 0; i < size_; ++i)
      data_[i] = other.data_[i];
  }

  // Move constructor ///////////////////////////////////////////////////////

  vector(vector<T> &&other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(alloc_, other.alloc_);
  }

  vector(vector &&other, const Allocator &alloc) {
    if (alloc != other.alloc_) {
      capacity_ = other.capacity_;
      size_ = other.size_;
      data_ = new T[capacity_];
      alloc_ = alloc;
      for (size_type i = 0; i < size_; ++i) {
        data_[i] = std::move(other.data_[i]);
      }
    } else {
      std::swap(capacity_, other.capacity);
      std::swap(size_, other.size);
      std::swap(data_, other.data_);
      std::swap(alloc_, other.alloc_);
    }
  }

  //! Destructor
  virtual ~vector() noexcept { delete[] data_; }

  //! Copy assignment operator
  vector &operator=(const vector<T> &other) {
    vector<T> w{other};
    std::swap(data_, w.data_);
    std::swap(size_, w.size_);
    std::swap(capacity_, w.capacity_);
    return *this;
  }

  //! Move assignment operator
  vector &operator=(vector &&other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    return *this;
  }

  T &operator=(size_t pos) { return data_[pos]; }
  const T &operator=(size_t pos) const { return data_[pos]; }

  void assign(size_t count, const T &value) {
    clear();
    reserve(count);
    for (size_t i = 0; i < count; ++i)
      push_back(value);
  }

  template <typename InputIt,
            typename std::enable_if_t<!std::is_integral<InputIt>::value,
                                      InputIt> * = nullptr>
  void assign(InputIt first, InputIt last) {
    clear();
    capacity_ = std::distance(first, last);
    size_ = capacity_;
    data_ = new T[capacity_];
    for (int i = 0; first != last, i < size_; ++first, ++i)
      data_[i] = *first;
  }

  // Element access /////////////////////////////////////////////////////////

  T &at(size_t pos) {
    try {
      if (pos >= size_)
        throw std::out_of_range("Out of range");
    } catch (const std::out_of_range &e) {
      std::cout << e.what() << " in phundrak::vector " << this << '\n';
      std::terminate();
    }
    return data_[pos];
  }

  const T &at(size_t pos) const {
    try {
      if (pos >= size_ || pos < 0)
        throw std::out_of_range("Out of range");
    } catch (const std::out_of_range &e) {
      std::cout << e.what() << " in phundrak::vector " << this << '\n';
      std::terminate();
    }

    return data_[pos];
  }

  T &operator[](size_t pos) { return data_[pos]; }
  const T &operator[](size_t pos) const { return data_[pos]; }

  T &front() { return data_[0]; }
  const T &front() const { return data_[0]; }

  T &back() { return data_[size_ - 1]; }
  const T &back() const { return data_[size_ - 1]; }

  T *data() noexcept { return data_; }
  const T *data() const noexcept { return data_; }

  // Iterators //////////////////////////////////////////////////////////////

  // I don't know how to create custom iterators :(

  // Capacity ///////////////////////////////////////////////////////////////

  bool empty() const noexcept { return (data_ == nullptr) ? true : false; }

  size_t size() const noexcept { return size_; }

  void reserve(size_t new_cap) {
    while (capacity_ < new_cap)
      double_capacity();
  }

  size_t capacity() const noexcept { return capacity_; }

  void shrink_to_fit() {
    T *olddata = data_;
    capacity_ = size_;
    data_ = new T[capacity_];
    for (size_t i = 0; i < size_; ++i)
      data_[i] = olddata[i];
    delete[] olddata;
  }

  // Modifiers //////////////////////////////////////////////////////////////

  void clear() noexcept {
    delete[] data_;
    data_ = new T[capacity_];
    size_ = 0;
  }

  // insert: can't do iterators :(
  // emplace: can't do iterators :(
  // erase: can't do iterators :(

  void push_back(const T &value) {
    ++size_;
    reserve(size_);
    data_[size_ - 1] = value;
  }

  void push_back(T &&value) {
    ++size_;
    reserve(size_);
    data_[size_ - 1] = std::move(value);
  }

  // emplace_back: don't know how to use std::allocator_traits

  void pop_back() {
    if (size_ > 0)
      --size_;
  }

  void resize(size_t count, T value = T()) {
    if (count < size_)
      size_ = count;
    else if (count > size_)
      reserve(count);
    while (size_ < count)
      push_back(value);
  }

  void swap(vector<T> &other) {
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
    std::swap(data_, other.data_);
  }

protected:
};

} // namespace phundrak
