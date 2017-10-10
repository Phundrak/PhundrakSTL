#include <cstdlib>
#include <algorithm>
#include <memory>
#include <utility>

namespace phundrak {

using size_type = size_t;

template <class T, class Allocator = std::allocator<T>> class list {

private:
  struct cell {
    cell() = default;
    explicit cell(const T& value) : x{value}, p{nullptr}, n{nullptr} {}
    explicit cell(T&& value) : x{value}, p{nullptr}, n{nullptr} {}
    cell *p;
    cell *n;
    T x;
  };

  cell *sentry;
  const Allocator alloc_;

public:
  class iterator;
  class const_iterator;
  class reverse_iterator;
  class const_reverse_iterator;

  /////////////////////////////////////////////////////////////////////////////
  //                             Member functions                            //
  /////////////////////////////////////////////////////////////////////////////

  // Constructors /////////////////////////////////////////////////////////////

  list() : list{Allocator()}, sentry{new cell} {}

  explicit list(const Allocator &alloc) : alloc_(alloc) {
    sentry = new cell{};
    sentry->p = sentry;
    sentry->n = sentry;
  }

  list(size_type count, const T &value, const Allocator &alloc = Allocator())
      : list(alloc), alloc_{alloc} {
    while (size() < count)
      push_back(value);
  }

  explicit list(size_type count, const Allocator &alloc = Allocator())
      : list(alloc), alloc_{alloc} {
    while (size() < count)
      push_back(T());
  }

  template <class InputIt>
  list(InputIt first, InputIt last, const Allocator &alloc = Allocator())
      : list(alloc) {
    while (first != last)
      push_back(*first);
  }

  list(const list &other) : list() {
    for (auto elem : other)
      push_back(elem);
  }

  list(const list &other, const Allocator &alloc) : list(alloc) {
    for (auto elem : other)
      push_back(elem);
  }

  list(list &&other) : list() {
    std::swap(other.sentry, sentry);
    std::swap(other.alloc_, alloc_);
  }

  list(list &&other, const Allocator &alloc) : list(alloc) {
    std::swap(other.sentry, sentry);
  }

  list(std::initializer_list<T> init, const Allocator &alloc = Allocator())
      : list(alloc) {
    for (const T &elem : init)
      push_back(elem);
  }

  // Destructor ///////////////////////////////////////////////////////////////

  virtual ~list() {
    clear();
    delete sentry;
  }

  // operator= ////////////////////////////////////////////////////////////////

  list &operator=(const list &other) {
    cell *it = other.sentry->n;
    while (it != other.sentry) {
      push_back(it->x);
      it = it->n;
    }
    return *this;
  }

  list &operator=(list &&other) noexcept {
    std::swap(other.sentry, sentry);
    return *this;
  }

  list &operator=(std::initializer_list<T> ilist) {
    for (const T &elem : ilist)
      push_back(elem);
    return *this;
  }

  // Assign ///////////////////////////////////////////////////////////////////

  void assign(size_type count, const T &value) {
    clear();
    for (int i = 0; i < count; ++i)
      push_front(value);
  }

  template <class InputIt> void assign(InputIt first, InputIt last) {
    clear();
    for (; first != last; ++first)
      push_back(*first);
  }

  void assign(std::initializer_list<T> ilist) {
    clear();
    for (const T &elem : ilist)
      push_back(elem);
  }

  // get_allocator ////////////////////////////////////////////////////////////

  std::allocator<T> get_allocator() { return alloc_; }

  /////////////////////////////////////////////////////////////////////////////
  //                              Element access                             //
  /////////////////////////////////////////////////////////////////////////////

  T &front() { return sentry->n->x; }
  const T &front() const { return sentry->n->x; }

  T &back() { return sentry->p->x; }
  const T &back() const { return sentry->p->x; }

  /////////////////////////////////////////////////////////////////////////////
  //                                Iterators                                //
  /////////////////////////////////////////////////////////////////////////////

  // iterators ////////////////////////////////////////////////////////////////

  iterator begin() noexcept { return iterator{sentry->n}; }
  const_iterator begin() const noexcept { return const_iterator{sentry->n}; }
  const_iterator cbegin() const noexcept { return const_iterator{sentry->n}; }

  iterator end() noexcept { return iterator{sentry}; }
  const_iterator end() const noexcept { return const_iterator{sentry}; }
  const_iterator cend() const noexcept { return const_iterator{sentry}; }

  // reverse iterators ////////////////////////////////////////////////////////

  reverse_iterator rbegin() noexcept { return reverse_iterator{sentry->p}; }
  const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator{sentry->p};
  }
  const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator{sentry->p};
  }

  reverse_iterator rend() noexcept { return reverse_iterator{sentry}; }
  const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator{sentry};
  }
  const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator{sentry};
  }

  /////////////////////////////////////////////////////////////////////////////
  //                                 Capacity                                //
  /////////////////////////////////////////////////////////////////////////////

  bool empty() const noexcept {
    return sentry->p == sentry;
  }

  size_type size() const {
    cell *it = sentry->n;
    size_type n = 0;
    while (it != sentry) {
      ++n;
      it = it->n;
    }
    return n;
  }

  /////////////////////////////////////////////////////////////////////////////
  //                                Modifiers                                //
  /////////////////////////////////////////////////////////////////////////////

  void clear() {
    cell *it = sentry->n;
    while (it != sentry) {
      cell *todel = it;
      it = it->n;
      delete todel;
    }
  }

  iterator insert(const_iterator pos, const T &value) {
    cell *elem = new cell{value};
    elem->n = pos;
    elem->p = pos->p;
    pos->p->n = elem;
    pos->p = elem;
    return iterator{pos};
  }

  iterator insert(const_iterator pos, T &&value) {
    cell *elem = new cell{value};
    elem->n = pos;
    elem->p = pos->p;
    pos->p->n = elem;
    pos->p = elem;
    return iterator{pos};
  }

  template<class InputIt>
  iterator insert(const_iterator pos, InputIt first, InputIt last) {
    for(; first != last; ++first)
      insert(pos, *first);
    return iterator{pos};
  }



  bool empty() { return sentry->p == sentry; }

  void push_front(const T &v) {
    cell *c = new cell;
    c->x = v;
    c->n = sentry->n;
    c->p = sentry;
    sentry->n->p = c;
    sentry->n = c;
  }

  void push_back(const T &v) {
    cell *c = new cell;
    c->x = v;
    c->p = sentry->p;
    c->n = sentry;
    sentry->p->n = c;
    sentry->p = c;
  }

  void pop_front() {
    cell *c = sentry->n;
    sentry->n = c->n;
    c->n->p = sentry;
    delete c;
  }

  void pop_back() {
    cell *c = sentry->p;
    sentry->p = c->p;
    c->p->n = sentry;
    delete c;
  }

  class iterator {

  protected:
    cell *it;

  public:
    iterator() : it{nullptr} {}
    explicit iterator(cell *point) : it{point} {}

    iterator(const iterator &other) : it{other.it} {}

    iterator(iterator &&other) { std::swap(it, other.it); }

    iterator &operator=(cell *point) {
      it = point;
      return *this;
    }
    iterator &operator=(const iterator &other) {
      it = other.it;
      return *this;
    }
    iterator &operator=(iterator &&other) {
      std::swap(it, other.it);
      return *this;
    }

    ~iterator() {}

    iterator &operator++() { // ++i
      it = it->n;
      return *this;
    }
    iterator operator++(int) { // i++
      // iterator t;
      // t.it = it;
      iterator t{*this};
      it = it->n;
      return t;
    }

    iterator &operator--() { // --i
      it = it->p;
      return *this;
    }

    iterator operator--(int) { // i--
      // iterator t;
      // t.it = it;
      iterator t{it};
      it = it->n;
      return t;
    }

    bool operator==(cell *point) { return point == it; }
    bool operator==(const iterator &other) { return other.it = it; }
    bool operator==(iterator &&other) { return other.it == it; }

    bool operator!=(cell *point) { return point != it; }
    bool operator!=(const iterator &other) { return other.it != it; }
    bool operator!=(iterator &&other) { return other.it != it; }

    T operator*() { return it->x; }
  };

  class const_iterator : protected iterator {
  public:
    const_iterator() : iterator() {}
    explicit const_iterator(T *point) : iterator(point) {}

    explicit const_iterator(const iterator &other) : iterator(other) {}
    const_iterator(const const_iterator &other) : iterator(other) {}

    explicit const_iterator(iterator &&other) : iterator(std::move(other)) {}
    const_iterator(const_iterator &&other) : iterator(std::move(other)) {}

    const_iterator &operator++() = delete;
    const_iterator operator++(int) = delete;
    const_iterator &operator--() = delete;
    const_iterator operator--(int) = delete;

    ~const_iterator() {}
  };

  class reverse_iterator : protected iterator {
  public:
    reverse_iterator() : iterator() {}
    explicit reverse_iterator(T *point) : iterator(point) {}

    reverse_iterator(const reverse_iterator &other) : iterator(other) {}

    reverse_iterator(reverse_iterator &&other) : iterator(std::move(other)) {}

    reverse_iterator &operator++() {
      this->it = this->it->p;
      return *this;
    }

    reverse_iterator operator++(int) {
      reverse_iterator t{*this};
      this->it = this->it->p;
      return t;
    }

    reverse_iterator &operator--() {
      this->it = this->it->n;
      return *this;
    }

    reverse_iterator operator--(int) {
      reverse_iterator t{*this};
      this->it = this->it->p;
      return t;
    }

    ~reverse_iterator() {}
  };

  class const_reverse_iterator : protected iterator {
  public:
    const_reverse_iterator() : iterator() {}
    explicit const_reverse_iterator(T *point) : iterator(point) {}

    const_reverse_iterator(const const_reverse_iterator &other)
        : iterator(other) {}

    const_reverse_iterator(const_reverse_iterator &&other)
        : iterator(std::move(other)) {}

    const_reverse_iterator &operator++() = delete;
    const_reverse_iterator &operator++(int) = delete;
    const_reverse_iterator &operator--() = delete;
    const_reverse_iterator &operator--(int) = delete;

    ~const_reverse_iterator() {}
  };
};

} // namespace phundrak
