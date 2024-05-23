#pragma once
#include <algorithm>
#include <new>
#include <stddef.h>
#include <stdexcept>

template <typename T> class Queue {
public:
  Queue();
  Queue(const Queue &);
  Queue(Queue &&);
  Queue &operator=(const Queue &);
  Queue &operator=(Queue &&);
  ~Queue();
  // Add new element at the end of the queue
  template <typename U> Queue &push(U &&);
  // Pop an element from the begin
  T pop();
  T &front();
  size_t size() const;
  size_t capacity() const;
  bool empty() const;
  bool full() const;
  void clear();

private:
  void copyQueue(const Queue &);
  size_t capacity_{0};
  size_t size_{0};
  T *arr_{nullptr};
  // Index sa koje strane se dodaje
  size_t back_{0};
  // Index sa koje strane se uklanja
  size_t front_{0};
};
// Vas kod ovdje

template <typename T> Queue<T>::Queue() {
  capacity_ = 10;
  size_ = 0;
  arr_ = new T[capacity_];
  back_ = 0;
  front_ = 0;
}

template <typename T> Queue<T>::~Queue() {
  size_ = 0;
  capacity_ = 0;
  delete[] arr_;
  arr_ = nullptr;
  front_ = 0;
  back_ = 0;
}

template <typename T> size_t Queue<T>::size() const { return size_; }
template <typename T> bool Queue<T>::empty() const { return size_ == 0; }
template <typename T> size_t Queue<T>::capacity() const { return capacity_; }

template <typename T>
template <typename U>
Queue<T> &Queue<T>::push(U &&value) {
  if (size_ == capacity_) {
    if (capacity_ == 0)
      capacity_ = 1;

    capacity_ *= 2;
    T *tmp = new T[capacity_];
    std::copy(arr_, arr_ + size_, tmp);
    delete[] arr_;
    arr_ = tmp;
  }
  arr_[back_++] = std::forward<U>(value);
  size_++;
  return *this;
}

template <typename T> Queue<T>::Queue(const Queue &other) {
  size_ = other.size_;
  capacity_ = other.capacity_;
  front_ = other.front_;
  back_ = other.back_;
  arr_ = new T[capacity_];
  std::copy(other.arr_, other.arr_ + size_, arr_);
}
template <typename T>
/* O(n) ne valja!!!
  T Queue<T>::pop(){
  T rez=arr_[0];
  T *tmp=new T[capacity_];
  for(int i=1,j=0; i < size_; i++){
    tmp[j]=arr_[i];
    j++;
  }
delete [] arr_;
arr_=tmp;
size_--;
  return rez;
}*/

// O(1)
T Queue<T>::pop() {
 if (size_ == 0) {
    throw std::out_of_range("Queue is empty");
  }

  T result = arr_[front_];
  front_ = (front_ + 1) % capacity_;
  size_--;

  if (size_ == 0) {
    front_ = 0;
    back_ = 0;
  } else {
    front_ = (front_ == capacity_) ? 0 : front_; 
  }

  return result;
}

template <typename T> T &Queue<T>::front() { 
  if(size_==0){
    throw std::out_of_range("Queue is empty!");
  }
  return arr_[front_]; }

template <typename T> Queue<T>::Queue(Queue &&other) {
  capacity_ = other.capacity_;
  size_ = other.size_;
  front_ = other.front_;
  back_ = other.back_;
  arr_ = other.arr_;
  other.arr_ = nullptr;
  other.capacity_ = 0;
  other.size_ = 0;
  other.front_ = 0;
  other.back_ = 0;
}
template <typename T> Queue<T> &Queue<T>::operator=(const Queue &other) {
  if (this != &other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    front_ = other.front_;
    back_ = other.back_;
    delete[] arr_;
    arr_ = new T[capacity_];
    std::copy(other.arr_, other.arr_ + size_, arr_);
  }
  return *this;
}
template <typename T> Queue<T> &Queue<T>::operator=(Queue &&other) {
  if (this != &other) {
    capacity_ = other.capacity_;
    size_ = other.size_;
    front_ = other.front_;
    back_ = other.back_;
    delete[] arr_;
    arr_ = other.arr_;
    other.arr_ = nullptr;
    other.capacity_ = 0;
    other.size_ = 0;
    other.front_ = 0;
    other.back_ = 0;
  }
  return *this;
}

template <typename T>
void Queue<T>::clear(){
  delete [] arr_;
  arr_ = new T [capacity_];
  size_=0;
  front_=0;
  back_=0;
}

template <typename T>
bool Queue<T>::full()const{
return size_==capacity_;
}
