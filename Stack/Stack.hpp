#pragma once
#include <algorithm>
#include <stddef.h>
#include <stdexcept>

template <typename T> class Stack {
public:
  Stack();
  Stack(const Stack &);
  Stack(Stack &&);
  Stack &operator=(const Stack &);
  Stack &operator=(Stack &&);
  T pop();
  T &top() const;
  // Savrseno proslijedjivane (forward referenca)
  template <typename U> Stack<T> &push(U &&);
  size_t size() const;
  size_t capacity() const;
  bool empty() const;
  bool full() const;
  ~Stack();
  void clear();
  void resize(size_t newSize);
  bool operator==(const Stack &);
  bool operator!=(const Stack &);

private:
  void realoc() {
    if (capacity_ == 0)
      capacity_ = 1;
    capacity_ *= 2;
    T *tmp = new T[capacity_];
    std::copy(arr_, arr_ + size_, tmp);
    delete[] arr_;
    arr_ = tmp;
  }
  size_t capacity_{0};
  size_t size_{0};
  T *arr_{nullptr};
};

template <typename T> Stack<T>::Stack() {
    capacity_ = 10;
  size_ = 0;
  arr_ = new T[capacity_];
}
template <typename T> Stack<T>::~Stack() {
  capacity_ = 0;
  size_ = 0;
  delete[] arr_;
  arr_=nullptr;
}

template <typename T> size_t Stack<T>::size() const { return size_; }
template <typename T> bool Stack<T>::empty() const { return size_ == 0; }

template <typename T> Stack<T>::Stack(const Stack &other) {
  size_ = other.size_;
  capacity_ = other.capacity_;
  arr_ = new T[capacity_];
  std::copy(other.arr_, other.arr_ + size_, arr_);
}

template <typename T> T &Stack<T>::top() const {
  if (size_ == 0)
    throw std::out_of_range("Stack is empty!");
  return arr_[size_ - 1];
}

template <typename T>
template <typename U>
Stack<T> &Stack<T>::push(U &&value) {
  if (size_ == capacity_)
    realoc();
  arr_[size_++] = std::forward<U>(value);
  return *this;
}

template <typename T> Stack<T>::Stack(Stack &&other) {
  size_ = other.size_;
  capacity_ = other.capacity_;
  delete[] arr_;
  arr_ = other.arr_;
  other.arr_ = nullptr;
  other.size_ = 0;
  other.capacity_ = 0;
}

template <typename T> Stack<T> &Stack<T>::operator=(const Stack &other) {
  if (this != &other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    delete[] arr_;
    arr_ = new T[capacity_];
    std::copy(other.arr_, other.arr_ + size_, arr_);
  }
  return *this;
}

template <typename T> Stack<T> &Stack<T>::operator=(Stack &&other) {
  if (this != &other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    delete[] arr_;
    arr_ = other.arr_;
    other.arr_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }
  return *this;
}

template <typename T> T Stack<T>::pop() {
  if (size_ == 0)
    throw std::out_of_range("Stack is empty!");
  return arr_[--size_];
}

template <typename T> size_t Stack<T>::capacity()const{return capacity_;}
template <typename T> bool Stack<T>::full()const{return size_==capacity_;}

template <typename T> void Stack<T>::clear(){
delete [] arr_;
arr_= new T[capacity_];
size_=0;
}

template <typename T>
bool Stack<T>::operator==(const Stack& other){
if(size_!=other.size_)
    return false;
for(int i=0; i<size_;++i){
    if(arr_[i]!=other.arr_[i])
      return false;
  }
  return true;
}

template <typename T>
bool Stack<T>::operator!=(const Stack& other){
return !(*this==other);
}

template <typename T>
void Stack<T>::resize(size_t NewSize){
if(NewSize<=capacity_){
    if(NewSize<=size_){
      size_=NewSize;
    }
    else{
      for(size_t i=size_; i<NewSize; ++i){
        arr_[i]=T();
      }
      size_=NewSize;
    }
  }else{
    T *tmp=new T[NewSize];
    std::copy(arr_, arr_+size_, tmp);
    for(auto i=size_; i<NewSize; ++i)
      tmp[i]=T();
    delete [] arr_;
    arr_=tmp;
    size_=NewSize;
    capacity_=NewSize;
  }
}
