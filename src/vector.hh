#include <cstdio>
#include <memory>
#include <iterator>
#include <stdexcept>
#include <iostream>

namespace phundrak {

  template<class T>
  class vector {
  public:

    // Member functions ///////////////////////////////////////////////////////

    //! Default constructor
    vector() : data_{nullptr}, size_{0}, capacity_{0} {}

    //! Copy constructor
    vector(const vector<T> &other)
        : data_{new T[other.size_]}, size_{other.size_},
          capacity_{other.capacity_} {
      for (size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
      }
    }

    //! Move constructor
    vector(vector<T> &&other) noexcept {
      std::swap(data_, other.data_);
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
    }

    //! Destructor
    virtual ~vector() noexcept {
      delete[] data_;
    }

    //! Copy assignment operator
    vector& operator=(const vector<T> &other) {
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

    template <class InputIt> void assign(InputIt first, InputIt last) {
      clear();
      capacity_ = std::distance(first, last);
      size_ = std::distance(first, last);
      data_ = new T[size_];
      for (int i = 0; first != last; ++first, ++i)
        data_[i] = *first;
    }

    // Element access /////////////////////////////////////////////////////////

    T &at(size_t pos) {
      try {
        if (pos >= size_ || pos < 0)
          throw std::out_of_range("Out of range");
      } catch (std::out_of_range e) {
        std::cout << e.what() << " in phundrak::vector " << this << '\n';
        std::terminate();
      }
      return data_[pos];
    }

    const T &at(size_t pos) const {
      try {
        if (pos >= size_ || pos < 0)
          throw std::out_of_range("Out of range");
      } catch (std::out_of_range e) {
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

    bool empty() const noexcept {
      return (size_ <= 0 || data_ == nullptr) ? true : false;
    }

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

    void swap(vector<T>& other) {
      std::swap(capacity_, other.capacity_);
      std::swap(size_, other.size_);
      std::swap(data_, other.data_);
    }

  protected:


  private:
    void double_capacity() {
      if(data_) {
        T *olddata = data_;
        capacity_ <<= 1;
        data_ = new T[capacity_];
        for(size_t i = 0; i < size_; ++i)
          data_[i] = olddata[i];
        delete[] olddata;
      } else {
        data_ = new T[1];
        capacity_ = 1;
      }
    }



    T * data_;
    size_t size_;
    size_t capacity_;

  };



}  // phundrak namespace
