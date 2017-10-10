#include <cstdlib>
#include <memory>
#include <utility>

namespace phundrak {

using size_type = size_t;

template <class T, class Allocator = std::allocator<T>> class list {
private:
  struct cell {
    cell() = default;
    cell *p;
    cell *n;
    T x;
  };
  cell *sentry;

public:
  class iterator;
  class const_iterator;

  // Member types //////////////////////////////////////////////////////////////

  list() : list{Allocator()}, sentry{new cell} {}

  explicit list(const Allocator &alloc) {
    sentry = new cell;
    sentry->p = sentry;
    sentry->n = sentry;
  }

  list(size_type count, const T &value, const Allocator &alloc = Allocator())
      : list(alloc) {
    while (size() < count)
      push_back(value);
  }

  explicit list(size_type count, const Allocator &alloc = Allocator())
      : list(alloc) {
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

  list(list &&other) : list() { std::swap(other.sentry, sentry); }

  list(list &&other, const Allocator &alloc) : list(alloc) {
    std::swap(other.sentry, sentry);
  }

  list(std::initializer_list<T> init, const Allocator &alloc = Allocator())
      : list(alloc) {
    for (auto elem : init)
      push_back(elem);
  }

  virtual ~list() {
    clear();
    delete sentry;
  }

  // Element access ////////////////////////////////////////////////////////////

  T &front() const { return sentry->n->x; }
  T &back() const { return sentry->p->x; }

  // Iterators /////////////////////////////////////////////////////////////////

  iterator begin() noexcept { return iterator{sentry->n}; }

  const_iterator begin() const noexcept { return const_iterator{sentry->n}; }
  const_iterator cbegin() const noexcept { return const_iterator{sentry->n}; }

  iterator end() noexcept { return iterator{sentry}; }
  const_iterator end() const noexcept { return const_iterator{sentry}; }
  const_iterator cend() const noexcept { return const_iterator{sentry}; }

  // Capacity //////////////////////////////////////////////////////////////////

  // Modifiers /////////////////////////////////////////////////////////////////

  void clear() {
    cell *it = sentry->n;
    while (it != sentry) {
      cell *todel = it;
      it = it->n;
      delete todel;
    }
  }

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
    for (auto elem : ilist)
      push_back(elem);
    return *this;
  }

  bool empty() { return sentry->p == sentry; }

  size_type size() const {
    cell *it = sentry->n;
    size_type n = 0;
    while (it != sentry) {
      ++n;
      it = it->n;
    }
    return n;
  }

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
    iterator &operator++(int) { // i++
      iterator t;
      t.it = it;
      // iterator t{*this};
      it = it->n;
      return t;
    }

    iterator &operator--() { // --i
      it = it->p;
      return *this;
    }

    iterator &operator--(int) { // i--
      iterator t;
      t.it = it;
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

  protected:
    cell *it;
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
    const_iterator &operator++(int) = delete;
    const_iterator &operator--() = delete;
    const_iterator &operator--(int) = delete;

    ~const_iterator() {}
  };
};

} // namespace phundrak
