#pragma once
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>
template <typename T> class MojVektor {
public:
  class Iterator;
  MojVektor();
  MojVektor(const std::initializer_list<T> &);
  MojVektor(const MojVektor &);
  MojVektor &operator=(const MojVektor &);
  MojVektor(MojVektor &&);
  MojVektor &operator=(MojVektor &&);
  ~MojVektor();
  MojVektor &push_back(const T &);
  MojVektor &push_front(const T &);
  MojVektor &push_back(T &&);
  MojVektor &push_front(T &&);
  size_t size() const;
  T &at(size_t) const;
  T &operator[](size_t) const;
  void clear();
  void resize(size_t newSize, const T &difference_value);
  MojVektor &pop_back();
  MojVektor &pop_front();
  T &back() const;
  T &front() const;
  bool empty() const;
  size_t capacity() const;
  bool operator==(const MojVektor &) const;
  bool operator!=(const MojVektor &) const;
  bool full() const;
  MojVektor subvector(Iterator begin, Iterator end);
  Iterator begin() const { return Iterator{arr_}; };
  Iterator end() const { return Iterator{arr_ + size_}; };
  Iterator find(const T &) const;
  Iterator erase(Iterator pos);
  Iterator insert(Iterator, const T &);
  Iterator insert(Iterator, T &&);
  Iterator rbegin() const { return Iterator{arr_ + size_ - 1}; }
  Iterator rend() const { return Iterator{arr_ - 1}; }
  Iterator erase(Iterator beginIt, Iterator endIt);
  void rotate();
  void rotate(Iterator beginIt, Iterator endIt);

  T *data();

private:
  void realoc();
  size_t capacity_;
  size_t size_;
  T *arr_;
};

template <typename T>
class MojVektor<T>::Iterator
    : public std::iterator<std::random_access_iterator_tag, T> {
public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T *;
  using reference = T &;

  Iterator() : ptr_(nullptr) {}
  Iterator(pointer ptr) : ptr_(ptr) {}
  Iterator(const Iterator &other) : ptr_(other.ptr_) {}
  Iterator(Iterator &&other) : ptr_(other.ptr_) { other.ptr_ = nullptr; }

  Iterator &operator=(const Iterator &other) {
    if (this != &other) {
      ptr_ = other.ptr_;
    }
    return *this;
  }

  int operator-(const Iterator &other) const { return ptr_ - other.ptr_; }
  Iterator &operator++() {
    ++ptr_;
    return *this;
  }
  Iterator operator+(int n) const { return Iterator{ptr_ + n}; }
  bool operator<(const Iterator &other) const { return ptr_ < other.ptr_; }
  Iterator &operator--() {
    --ptr_;
    return *this;
  }

  Iterator operator--(int) {
    auto temp = *this;
    --ptr_;
    return temp;
  }
  Iterator operator-(int n) const { return Iterator{ptr_ - n}; }
  Iterator operator++(int) {
    auto temp = *this;
    ++ptr_;
    return temp;
  }
  Iterator &operator=(Iterator &&other) noexcept {
    if (this != &other) {
      ptr_ = other.ptr_;
      other.ptr_ = nullptr;
    }
    return *this;
  }
  T *operator[](size_t index) { return ptr_ + index; }
  T *operator[](size_t index) const { return ptr_ + index; }
  reference operator*() const { return *ptr_; }
  pointer operator->() const { return ptr_; }
  bool operator==(const Iterator &other) const { return ptr_ == other.ptr_; }
  bool operator!=(const Iterator &other) const { return ptr_ != other.ptr_; }
  T *data() { return ptr_; }

private:
  T *ptr_;
};

// Implementacija ovdje

template <typename T>
MojVektor<T>::MojVektor() : capacity_(10), size_(0), arr_(new T[capacity_]){};

template <typename T>
MojVektor<T>::MojVektor(const std::initializer_list<T> &rhs)
    : capacity_(rhs.size()), size_(rhs.size()), arr_(new T[capacity_]) {
  std::copy(rhs.begin(), rhs.end(), arr_);
};
template <typename T> size_t MojVektor<T>::capacity() const {
  return capacity_;
}

template <typename T> bool MojVektor<T>::empty() const {
  if (size_ == 0)
    return true;
  return false;
}

template <typename T> MojVektor<T>::~MojVektor() {
  capacity_ = 0;
  size_ = 0;
  delete[] arr_;
  arr_ = nullptr;
}

template <typename T> T &MojVektor<T>::operator[](size_t i) const {
  return arr_[i];
}

template <typename T> MojVektor<T>::MojVektor(const MojVektor<T> &rhs) {
  capacity_ = rhs.capacity();
  size_ = rhs.size();
  if (size_ > 0) {
    arr_ = new T[capacity_];
    for (size_t i = 0; i < size_; ++i) {
      arr_[i] = rhs.arr_[i];
    }
  }
}

template <typename T>
MojVektor<T> &MojVektor<T>::operator=(const MojVektor<T> &rhs) {
  if (this != &rhs) {
    delete[] arr_;

    capacity_ = rhs.capacity();
    size_ = rhs.size();
    arr_ = new T[capacity_];
    for (size_t i = 0; i < size_; ++i) {
      arr_[i] = rhs[i];
    }
  }
  return *this;
}

template <typename T> MojVektor<T>::MojVektor(MojVektor<T> &&rhs) {
  capacity_ = rhs.capacity();
  size_ = rhs.size();
  arr_ = new T[size_];
  arr_ = rhs.arr_;
  rhs.capacity_ = 0;
  rhs.size_ = 0;
  rhs.arr_ = nullptr;
}

template <typename T>
MojVektor<T> &MojVektor<T>::operator=(MojVektor<T> &&rhs) {
  if (this != &rhs) {
    delete[] arr_;

    arr_ = rhs.arr_;
    size_ = rhs.size();
    capacity_ = rhs.capacity();

    rhs.arr_ = nullptr;
    rhs.size_ = 0;
    rhs.capacity_ = 0;
  }
  return *this;
}

template <typename T> T *MojVektor<T>::data() { return arr_; }

template <typename T> MojVektor<T> &MojVektor<T>::push_back(const T &val) {
  if (size_ >= capacity_)
    realoc();
  arr_[size_++] = val;
  return *this;
}

template <typename T> void MojVektor<T>::realoc() {
  capacity_ = capacity_ > 0 ? capacity_ * 2 : 1;
  T *new_arr = new T[capacity_];
  for (size_t i = 0; i < size_; ++i) {
    new_arr[i] = arr_[i];
  }
  delete[] arr_;
  arr_ = new_arr;
}
template <typename T> void MojVektor<T>::clear() {
  size_ = 0;
  delete[] arr_;
  arr_ = nullptr;
  capacity_ = 0;
}

template <typename T> size_t MojVektor<T>::size() const { return size_; }

template <typename T>

MojVektor<T> &MojVektor<T>::push_back(T &&elem) {
  if (size_ >= capacity_) {
    realoc();
  }
  arr_[size_++] = std::move(elem);
  return *this;
}

template <typename T> T &MojVektor<T>::at(size_t i) const {
  if (i >= size_) {
    throw std::out_of_range("ensar reko ne moze");
  }
  return arr_[i];
}

template <typename T>
void MojVektor<T>::resize(size_t newSize, const T &difference_value) {
  if (newSize < size_) {
    size_ = newSize;
  } else if (newSize > size_) {
    if (newSize > capacity_) {
      capacity_ = newSize;
      T *newArr = new T[capacity_];

      for (size_t i = 0; i < size_; ++i) {
        newArr[i] = arr_[i];
      }
      for (size_t i = size_; i < newSize; ++i) {
        newArr[i] = difference_value;
      }

      delete[] arr_;
      arr_ = newArr;
    } else {
      for (size_t i = size_; i < newSize; ++i) {
        arr_[i] = difference_value;
      }
    }
    size_ = newSize;
  }
}

template <typename T> MojVektor<T> &MojVektor<T>::pop_back() {
  if (size_ == 0) {
    throw std::out_of_range("ensar reko ne moze!!!");
  }
  if (size_ >= 1) {
    size_--;
  }
  return *this;
}

template <typename T> T &MojVektor<T>::back() const {
  if (size_ == 0) {
    throw std::out_of_range("ensar reko ne moze!");
  }
  return arr_[size_ - 1];
}

template <typename T> T &MojVektor<T>::front() const {
  if (size_ == 0) {
    throw std::out_of_range("Ensar reko ne moze");
  }
  return arr_[0];
}

template <typename T>
bool MojVektor<T>::operator==(const MojVektor<T> &rhs) const {
  if (size_ != rhs.size_)
    return false;
  for (size_t i = 0; i < size_; ++i) {
    if (arr_[i] != rhs.arr_[i])
      return false;
  }
  return true;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const MojVektor<T> &v) {
  os << '{';
  for (size_t i = 0; i < v.size(); ++i) {
    os << v.at(i);
    if (i != v.size() - 1)
      os << ", ";
  }
  os << '}';
  return os;
}

template <typename T> bool MojVektor<T>::full() const {
  if (size_ == capacity_)
    return true;
  return false;
}
// 2,3,4,5
template <typename T> MojVektor<T> &MojVektor<T>::push_front(const T &elem) {
  if (size_ == capacity_) {

    realoc();
  }

  for (size_t i = size_; i > 0; --i) {
    arr_[i] = std::move(arr_[i - 1]);
  }

  arr_[0] = elem;

  ++size_;
  return *this;
}

template <typename T> MojVektor<T> &MojVektor<T>::push_front(T &&elem) {
  if (size_ == capacity_) {

    realoc();
  }

  for (size_t i = size_; i > 0; --i) {
    arr_[i] = std::move(arr_[i - 1]);
  }

  arr_[0] = std::move(elem);

  ++size_;
  return *this;
}

template <typename T> MojVektor<T> &MojVektor<T>::pop_front() {
  if (size_ == 0) {
    throw std::out_of_range("Attempt to pop from an empty vector");
  }

  for (size_t i = 0; i < size_ - 1; ++i) {
    arr_[i] = std::move(arr_[i + 1]);
  }

  --size_;

  return *this;
}

template <typename T>
typename MojVektor<T>::Iterator MojVektor<T>::find(const T &value) const {
  for (auto it = begin(); it != end(); ++it) {
    if (*it == value)
      return it;
  }
  return end();
}

template <typename T>
typename MojVektor<T>::Iterator
MojVektor<T>::erase(typename MojVektor<T>::Iterator pos) {
  if (pos == end())
    return end();
  if (pos.data() == &back()) {
    --size_;
    return end();
  }
  std::move(pos.data() + 1, arr_ + size_, arr_ + (pos.data() - arr_));
  --size_;
  return Iterator{arr_ + (pos.data() - arr_)};
}

template <typename T>
typename MojVektor<T>::Iterator
MojVektor<T>::erase(typename MojVektor<T>::Iterator beginIt,
                    typename MojVektor<T>::Iterator endIt) {
  if (endIt - beginIt < 0)
    throw std::out_of_range{"ensar reko ne moze!"};
  std::move(endIt.data(), arr_ + size_, beginIt.data());
  size_ -= endIt - beginIt;
  return Iterator{arr_ + (beginIt.data() - arr_)};
}
template <typename T>
typename MojVektor<T>::Iterator
MojVektor<T>::insert(typename MojVektor<T>::Iterator pos, const T &value) {
  if (pos == end())
    throw std::out_of_range{"ensa reko ne moze"};
  if (pos == begin()) {
    push_front(value);
    return Iterator{arr_};
  }
  int insert_pos = pos.data() - arr_;
  if (size_ < capacity_) {
    std::move(pos.data(), arr_ + size_, arr_ + insert_pos + 1);
    *pos = value;
    ++size_;
    return pos;
  }
  capacity_ *= 2;
  auto temp = new T[capacity_];
  std::move(arr_, pos.data(), temp);
  std::move(arr_ + insert_pos, arr_ + size_, temp + insert_pos + 1);
  delete[] arr_;
  arr_ = temp;
  arr_[insert_pos] = value;
  ++size_;
  return Iterator{arr_ + insert_pos};
}

template <typename T>
typename MojVektor<T>::Iterator
MojVektor<T>::insert(typename MojVektor<T>::Iterator pos, T &&value) {
  if (pos == end())
    throw std::out_of_range{"ensar reko ne moze!"};
  if (pos == begin()) {
    push_front(value);
    return Iterator{arr_};
  }
  int insert_pos = pos.data() - arr_;
  if (size_ < capacity_) {
    std::move(pos.data(), arr_ + size_, arr_ + insert_pos + 1);
    *pos = std::move(value);
    ++size_;
    return pos;
  }
  capacity_ *= 2;
  auto temp = new T[capacity_];
  std::move(arr_, pos.data(), temp);
  std::move(arr_ + insert_pos, arr_ + size_, temp + insert_pos + 1);
  delete[] arr_;
  arr_ = temp;
  arr_[insert_pos] = std::move(value);
  ++size_;
  return Iterator{arr_ + insert_pos};
}

template <typename T>
MojVektor<T> MojVektor<T>::subvector(typename MojVektor<T>::Iterator begin,
                                     typename MojVektor<T>::Iterator end) {
  MojVektor<T> result;
  int difference = end - begin;
  if (difference < 0)
    throw std::invalid_argument{"ensar reko ne moze!"};
  result.arr_ = new T[difference];
  result.size_ = difference;
  result.capacity_ = 2 * difference;
  std::copy(begin.data(), end.data(), result.arr_);
  return result;
}

template <typename T> void MojVektor<T>::rotate() {
  auto itr = rbegin();
  auto it = begin();
  while (it < itr) {
    std::swap(*it, *itr);
    ++it;
    --itr;
  }
}

template <typename T>
void MojVektor<T>::rotate(typename MojVektor<T>::Iterator beginIt,
                          typename MojVektor<T>::Iterator endIt) {
  auto itr = endIt - 1;
  auto it = beginIt;
  while (it < itr) {
    std::swap(*it, *itr);
    ++it;
    --itr;
  }
}
