// TODO: Fix those damn iterators

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

namespace phundrak {

using size_type = size_t;

template <class T, class Allocator = std::allocator<T>> class list {

private:
  struct cell {
    cell() = default;
    explicit cell(const T &value) : x{value}, p{nullptr}, n{nullptr} {}
    explicit cell(T &&value) : x{value}, p{nullptr}, n{nullptr} {}
    cell(const cell &other) : x{other.x}, p{other.p}, n{other.n} {}
    cell(cell &&other) : x{T{}}, p{nullptr}, n{nullptr} {
      std::swap(x, other.x);
      std::swap(p, other.p);
      std::swap(n, other.n);
    }
    cell &operator=(const cell &other) {
      x = other.x;
      p = other.p;
      n = other.n;
      return *this;
    }
    cell &operator=(cell &&other) {
      std::swap(x, other.x);
      std::swap(other.n, n);
      std::swap(p, other.p);
      return *this;
    }
    cell *p;
    cell *n;
    T x;
  };

  cell *sentry;
  const Allocator alloc_;

public:
  class iterator;
  class reverse_iterator;
  using const_iterator = const iterator;
  using const_reverse_iterator = const reverse_iterator;

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

  bool empty() const noexcept { return sentry->p == sentry; }

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

  // clear ////////////////////////////////////////////////////////////////////

  void clear() {
    cell *it = sentry->n;
    while (it != sentry) {
      cell *todel = it;
      it = it->n;
      delete todel;
    }
  }

  // insert ///////////////////////////////////////////////////////////////////

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

  template <class InputIt>
  iterator insert(const_iterator pos, InputIt first, InputIt last) {
    for (; first != last; ++first)
      insert(pos, *first);
    return iterator{pos};
  }

  // emplace //////////////////////////////////////////////////////////////////

  template <class... Args>
  iterator emplace(const_iterator pos, Args &&... args) {
    return insert(pos, T{std::forward<Args>(args)...});
  }

  // erase ////////////////////////////////////////////////////////////////////

  iterator erase(const_iterator pos) {
    pos->p->n = pos->n;
    pos->n->p = pos->p;
    pos->n = nullptr;
    pos->p = nullptr;
    cell *todel = pos;
    ++pos;
    delete todel;
    return pos;
  }

  iterator erase(const_iterator begin, const_iterator end) {
    while (begin != end) {
      begin = erase(begin);
    }
    return begin;
  }

  // push_back ////////////////////////////////////////////////////////////////

  void push_back(const T &v) {
    cell *c = new cell;
    c->x = v;
    c->p = sentry->p;
    c->n = sentry;
    sentry->p->n = c;
    sentry->p = c;
  }

  void push_back(T &&v) {
    cell *c = new cell;
    std::swap(c->x, v);
    c->p = sentry->p;
    c->n = sentry;
    sentry->p->n = c;
    sentry->p = c;
  }

  // emplace_back /////////////////////////////////////////////////////////////

  template <class... Args> T &emplace_back(Args &&... args) {
    emplace(begin(), args...);
    return *begin();
  }

  // pop_back /////////////////////////////////////////////////////////////////

  void pop_back() {
    cell *c = sentry->p;
    sentry->p = c->p;
    c->p->n = sentry;
    delete c;
  }

  // push_front ///////////////////////////////////////////////////////////////

  void push_front(const T &v) {
    cell *c = new cell;
    c->x = v;
    c->n = sentry->n;
    c->p = sentry;
    sentry->n->p = c;
    sentry->n = c;
  }

  void push_front(T &&value) {
    cell *c = new cell;
    std::swap(c->x, value);
    c->n = sentry->n;
    c->p = sentry;
    sentry->n->p = c;
    sentry->p = c;
  }

  // emplace_front ////////////////////////////////////////////////////////////

  template <class... Args> T &emplace_front(Args &&... args) {
    emplace(end(), args...);
    return *end();
  }

  // pop_front ////////////////////////////////////////////////////////////////

  void pop_front() {
    cell *c = sentry->n;
    sentry->n = c->n;
    c->n->p = sentry;
    delete c;
  }

  // resize ///////////////////////////////////////////////////////////////////

  void resize(size_type count) {
    if (count > size())
      while (size() < count)
        push_back(T());
    else
      while (size() > count)
        pop_back();
  }

  void resize(size_type count, const T &value) {
    if (count > size())
      while (size() < count)
        push_back(value);
    else
      while (size() > count)
        pop_back();
  }

  // swap /////////////////////////////////////////////////////////////////////

  void swap(list &other) noexcept {
    try {
      if (get_allocator() != other.get_allocator())
        throw 20;
    } catch (int e) {
      std::cout << "An error has occured: " << this << " and " << *other
                << " do not have the same allocator.\nAborting...\n";
      std::terminate();
    }
    std::swap(other.sentry, sentry);
  }

  /////////////////////////////////////////////////////////////////////////////
  //                                Operations                               //
  /////////////////////////////////////////////////////////////////////////////

  // merge ////////////////////////////////////////////////////////////////////

  void merge(list &other) {
    if (this == other)
      return;
    try {
      if (get_allocator() != other.get_allocator())
        throw 20;
    } catch (int error) {
      std::cout << "Error in void List<T>::merge(list& other): " << this
                << " and " << *other << " do not share the same allocator.\n";
    }
    sentry->p->n = other.sentry->n;
    other.sentry->n->p = sentry->p;
    other.sentry->p->n = sentry;
    sentry->p = other.sentry->p;
    other.sentry->n = other.sentry;
    other.sentry->p = other.sentry;
    std::sort(*this);
  }

  void merge(list &&other) {
    if (this == other)
      return;
    try {
      if (get_allocator() != other.get_allocator())
        throw 20;
    } catch (int error) {
      std::cout << "Error in void List<T>::merge(list &&other):\n"
                << this << " and " << *other
                << " do not share the same allocator.\n";
    }
    sentry->p->n = other.sentry->n;
    other.sentry->n->p = sentry->p;
    other.sentry->p->n = sentry;
    sentry->p = other.sentry->p;
    other.sentry->n = other.sentry;
    other.sentry->p = other.sentry;
    std::sort(*this);
  }

  template <class Compare> void merge(list &other, Compare comp) {
    if (this == other)
      return;
    try {
      if (get_allocator() != other.get_allocator())
        throw 20;
    } catch (int error) {
      std::cout << "Error in template <class Compare> void List<T>::merge(list "
                   "&other, Compare comp):\n"
                << this << " and " << *other
                << " do not share the same allocator.\n";
    }
    sentry->p->n = other.sentry->n;
    other.sentry->n->p = sentry->p;
    other.sentry->p->n = sentry;
    sentry->p = other.sentry->p;
    other.sentry->n = other.sentry;
    other.sentry->p = other.sentry;
    std::sort(*this, comp);
  }

  template <class Compare> void merge(list &&other, Compare comp) {
    if (this == other)
      return;
    try {
      if (get_allocator() != other.get_allocator())
        throw 20;
    } catch (int error) {
      std::cout << "Error in template<class Compare> void "
                   "List<T>::merge(list&& other, Compare comp):\n"
                << this << " and " << *other
                << " do not share the same allocator.\n";
    }
    sentry->p->n = other.sentry->n;
    other.sentry->n->p = sentry->p;
    other.sentry->p->n = sentry;
    sentry->p = other.sentry->p;
    other.sentry->n = other.sentry;
    other.sentry->p = other.sentry;
    std::sort(*this, comp);
  }

  // splice ///////////////////////////////////////////////////////////////////

  void splice(const_iterator pos, list &other) {
    try {
      if (get_allocator() != other.get_allocator())
        throw 20;
    } catch (int error) {
      std::cout
          << "Error in void List<T>::splice(const_iterator pos, list& other):\n"
          << this << " and " << *other << " do not share the same allocator.\n";
    }
    pos->p->n = other.sentry->n;
    other.sentry->p->n = pos;
    other.sentry->n->p = pos->p;
    pos->p = other.sentry->p;
    other.sentry->p = other.sentry;
    other.sentry->n = other.sentry;
  }

  void splice(const_iterator pos, list &&other) {
    try {
      if (get_allocator() != other.get_allocator())
        throw 20;
    } catch (int error) {
      std::cout
          << "Error in void List<T>::splice(const_iterator pos, list& other):\n"
          << this << " and " << *other << " do not share the same allocator.\n";
    }
    pos->p->n = other.sentry->n;
    other.sentry->p->n = pos;
    other.sentry->n->p = pos->p;
    pos->p = other.sentry->p;
    other.sentry->p = other.sentry;
    other.sentry->n = other.sentry;
  }

  void splice(const_iterator pos, list &other, const_iterator it) {
    try {
      if (get_allocator() != other.get_allocator())
        throw 20;
    } catch (int error) {
      std::cout
          << "Error in void List<T>::splice(const_iterator pos, list& other):\n"
          << this << " and " << *other << " do not share the same allocator.\n";
    }

    it->p->n = it->n;
    it->n->p = it->p;
    it->p = it->p;
    it->n = pos;
    pos->p->n = it;
    pos->p = it;
  }

  void splice(const_iterator pos, list &&other, const_iterator it) {
    try {
      if (get_allocator() != other.get_allocator())
        throw 20;
    } catch (int error) {
      std::cout
          << "Error in void List<T>::splice(const_iterator pos, list& other):\n"
          << this << " and " << *other << " do not share the same allocator.\n";
    }

    it->p->n = it->n;
    it->n->p = it->p;
    it->p = it->p;
    it->n = pos;
    pos->p->n = it;
    pos->p = it;
  }

  void splice(const_iterator pos, list &other, const_iterator first,
              const_iterator last) {
    try {
      if (get_allocator() != other.get_allocator())
        throw 20;
    } catch (int error) {
      std::cout
          << "Error in void List<T>::splice(const_iterator pos, list& other):\n"
          << this << " and " << *other << " do not share the same allocator.\n";
    }

    first->p->n = last;
    pos->p->n = first;
    last->p->n = pos;
    auto cell = last->p;
    last->p = first->p;
    first->p = pos->n;
    pos->n = cell;
  }

  void splice(const_iterator pos, list &&other, const_iterator first,
              const_iterator last) {
    try {
      if (get_allocator() != other.get_allocator())
        throw 20;
    } catch (int error) {
      std::cout
          << "Error in void List<T>::splice(const_iterator pos, list& other):\n"
          << this << " and " << *other << " do not share the same allocator.\n";
    }

    first->p->n = last;
    pos->p->n = first;
    last->p->n = pos;
    auto cell = last->p;
    last->p = first->p;
    first->p = pos->n;
    pos->n = cell;
  }

  // remove, remove_if ////////////////////////////////////////////////////////

  void remove(const T &value) {
    for (auto itr = begin(); itr != end(); ++itr)
      if (itr->x == value) {
        auto elem = itr;
        --itr;
        elem->p->n = elem->n;
        elem->n->p = elem->p;
        elem->n = nullptr;
        elem->p = nullptr;
        delete elem;
      }
  }

  template <class UnaryPredicate> void remove(UnaryPredicate p) {
    for (auto itr = begin(); itr != end(); ++itr) {
      if (p(*itr)) {
        auto elem = itr;
        --itr;
        elem->p->n = elem->n;
        elem->n->p = elem->p;
        elem->n = nullptr;
        elem->p = nullptr;
        delete elem;
      }
    }
  }

  // reverse //////////////////////////////////////////////////////////////////

  void reverse() noexcept {
    list<T> list{};
    while (sentry->n != sentry) {
      sentry->n->p = list.sentry->p;
      list.sentry->p->n = sentry->n;
      sentry->n->n->p = sentry;
      list.sentry->p = sentry->n;
      sentry->n = sentry->n->n;
      list.sentry->p->n = list.sentry;
    }
  }

  // unique ///////////////////////////////////////////////////////////////////

  void unique() {
    for(auto elem = sentry->n; elem->n != sentry; elem = elem->n)
      if(elem->x == elem->n->x)
        erase(iterator{elem->n});
  }

  template<class BinaryPredicate> void unique(BinaryPredicate p) {
    for(auto elem = sentry->n; elem->n != sentry; elem = elem->n)
      if(p(elem, elem->n))
        erase(iterator{elem->n});
  }

  /////////////////////////////////////////////////////////////////////////////
  //                              Iterator class                             //
  /////////////////////////////////////////////////////////////////////////////

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
    bool operator==(const iterator &other) { return other.it == it; }
    bool operator==(iterator &&other) { return other.it == it; }

    bool operator!=(cell *point) { return point != it; }
    bool operator!=(const iterator &other) { return other.it != it; }
    bool operator!=(iterator &&other) { return other.it != it; }

    T& operator*() { return it->x; }
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
};

} // namespace phundrak
