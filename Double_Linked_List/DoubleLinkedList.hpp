#pragma once

#include <cmath>
#include <cstddef>
#include <exception>
#include <functional>
#include <iterator>
#include <stdexcept>

template <typename T> struct Node {
  Node *prev{nullptr};
  Node *next{nullptr};
  T data;
  Node(const T &element) : data{element}, next{nullptr}, prev{nullptr} {}
};

template <typename T> class DoubleLinkedList {
public:
  class Iterator;
  DoubleLinkedList();
  DoubleLinkedList(const DoubleLinkedList &);
  DoubleLinkedList(DoubleLinkedList &&);
  DoubleLinkedList &operator=(const DoubleLinkedList &);
  DoubleLinkedList &operator=(DoubleLinkedList &&);
  ~DoubleLinkedList();

  DoubleLinkedList &push_back(const T &);
  DoubleLinkedList &push_front(const T &);
  DoubleLinkedList &push_back(T &&);
  DoubleLinkedList &push_front(T &&);
  DoubleLinkedList &pop_front();
  DoubleLinkedList &pop_back();
  bool empty() const;
  size_t size() const;
  T &front();
  T &back();
  Iterator begin() const{return Iterator(head_);}
  Iterator end() const{return Iterator(nullptr);}
  const Iterator cbegin() const{return Iterator(head_);}
  const Iterator cend() const{return Iterator(nullptr);}
  Iterator rbegin(){return Iterator(tail_);}
  Iterator rend(){return Iterator(nullptr);}
  const Iterator rcbegin() const{return Iterator(tail_);}
  const Iterator rcend() const{return Iterator(nullptr);}
  void clear();
  Iterator insert(Iterator pos, const T &element);
  Iterator erase(Iterator pos);
  Iterator erase(Iterator beginIt, Iterator endIt);
  void remove_if(std::function<bool(const T &)> &&p);
  void reverse();
  Iterator find(const T &element) const;

private:
  void dealoc();
  Node<T> *head_{nullptr};
  Node<T> *tail_{nullptr};
  size_t size_{0};
};

template <typename T>
class DoubleLinkedList<T>::Iterator
    : public std::iterator<std::bidirectional_iterator_tag, T> {
public:
  friend class DoubleLinkedList<T>;
  Iterator() : p_{nullptr}{}
  Iterator(Node<T> *p) : p_{p} {}
  Iterator(Iterator &&other) : p_{other.p_}{
  other.p_=nullptr;
  }
  Iterator (const Iterator& other) : p_(other.p_){}
  Iterator &operator=(Iterator&&other){
  p_=other.p_;
  other.p_=nullptr;
  return *this;
  }
  bool operator ==(const Iterator& other) const {
    return other.p_==p_;
  }
  bool operator !=(const Iterator& other) const {
    return other.p_!=p_;
  }
  Iterator &operator++() {
    if (p_ != nullptr)
      p_ = p_->next;
    return *this;
  }
  Iterator operator++(int) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
  }
  Iterator &operator--() {
    if (p_ != nullptr)
      p_ = p_->prev;
    return *this;
  }
  Iterator operator--(int) {
    Iterator tmp = *this;
    --(*this);
    return tmp;
  }
  Node<T> * node(){return p_;}
  T &operator*() { return p_->data; }
  T *operator->() { return &p_->data; }

private:
  Node<T> *p_;
};
template <typename T> bool DoubleLinkedList<T>::empty() const {
  return size_ == 0;
}

template <typename T>
DoubleLinkedList<T> &DoubleLinkedList<T>::push_back(const T &elem) {
  Node<T> *tmp = new Node<T>(elem);
  if (empty()) {
    head_ = tail_ = tmp;
  } else {
    tmp->prev = tail_;
    tail_->next = tmp;
    tail_ = tmp;
  }
  size_++;
  return *this;
}

template <typename T>
DoubleLinkedList<T> &DoubleLinkedList<T>::push_back(T &&elem) {
  Node<T> *tmp = new Node<T>(std::move(elem));
  if (empty()) {
    head_ = tail_ = tmp;
  } else {
    tmp->prev = tail_;
    tail_->next = tmp;
    tail_ = tmp;
  }
  size_++;
  return *this;
}
template <typename T> DoubleLinkedList<T>::~DoubleLinkedList() {
  Node<T> *tmp = head_;
  while (tmp != nullptr) {
    Node<T> *del = tmp;
    tmp = tmp->next;
    delete del;
  }
  size_ = 0;
  head_ = tail_ = nullptr;
}
template <typename T> DoubleLinkedList<T>::DoubleLinkedList() {
  size_ = 0;
  head_ = tail_ = nullptr;
}

template <typename T> T &DoubleLinkedList<T>::front() {
  if (empty()) {
    throw std::out_of_range("List is empty!");
  }
  return head_->data;
}

template <typename T> T &DoubleLinkedList<T>::back() {
  if (empty()) {
    throw std::out_of_range("List is empty!");
  }
  return tail_->data;
}

template <typename T> size_t DoubleLinkedList<T>::size() const { return size_; }

template <typename T>
DoubleLinkedList<T> &DoubleLinkedList<T>::push_front(const T &elem) {
  Node<T> *tmp = new Node<T>(elem);
  if (empty()) {
    head_ = tail_ = tmp;
  } else {
    head_->prev = tmp;
    tmp->next = head_;
    head_ = tmp;
  }
  size_++;
  return *this;
}

template <typename T>
DoubleLinkedList<T> &DoubleLinkedList<T>::push_front(T &&elem) {
  Node<T> *tmp = new Node<T>(std::move(elem));
  if (empty()) {
    head_ = tail_ = tmp;
  } else {
    head_->prev = tmp;
    tmp->next = head_;
    head_ = tmp;
  }
  size_++;
  return *this;
}

template <typename T> DoubleLinkedList<T> &DoubleLinkedList<T>::pop_front() {
  if (empty()) {
    throw std::out_of_range("List is empty!");
  }
  Node<T> *del = head_;
  if (head_ == tail_) {
    head_ = tail_ = nullptr;
  } else {
    head_ = head_->next;
    head_->prev = nullptr;
  }
  delete del;
  size_--;
  return *this;
}

template <typename T> DoubleLinkedList<T> &DoubleLinkedList<T>::pop_back() {
  if (empty()) {
    throw std::out_of_range("List is empty");
  }
  Node<T> *del = tail_;
  if (head_ == tail_)
    head_ = tail_ = nullptr;
  else {
    tail_ = tail_->prev;
    tail_->next = nullptr;
  }
  delete del;
  size_--;
  return *this;
}

template <typename T>
DoubleLinkedList<T>::DoubleLinkedList(const DoubleLinkedList<T> &rhs) {
  Node<T> *tmp = rhs.head_;
  while (tmp != nullptr) {
    push_back(tmp->data);
    tmp = tmp->next;
  }
}

template <typename T>
DoubleLinkedList<T>::DoubleLinkedList(DoubleLinkedList<T> &&rhs) {
  head_ = rhs.head_;
  tail_ = rhs.tail_;
  size_ = rhs.size_;
  rhs.head_ = nullptr;
  rhs.tail_ = nullptr;
  rhs.size_ = 0;
}

template <typename T>
DoubleLinkedList<T> &
DoubleLinkedList<T>::operator=(const DoubleLinkedList<T> &rhs) {
  if (this == &rhs) {
    return *this;
  }
  clear();
  Node<T> *tmp = rhs.head_;
  while (tmp != nullptr) {
    push_back(tmp->data);
    tmp = tmp->next;
  }
  size_ = rhs.size_;
  return *this;
}

template <typename T>
DoubleLinkedList<T> &DoubleLinkedList<T>::operator=(DoubleLinkedList<T> &&rhs) {
  if (this == &rhs)
    return *this;
  clear();
  head_ = rhs.head_;
  tail_ = rhs.tail_;
  size_ = rhs.size_;
  rhs.head_ = nullptr;
  rhs.tail_ = nullptr;
  rhs.size_ = 0;
  return *this;
}

template <typename T> void DoubleLinkedList<T>::clear() {
  Node<T> *current = head_;
  while (current != nullptr) {
    Node<T> *next = current->next;
    delete current;
    current = next;
  }
  head_ = tail_ = nullptr;
  size_ = 0;
}

template <typename T>
class DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::insert(Iterator pos, const T& elem){
  Node<T> *newNode = new Node<T>(elem);
  if(pos == end()){
    if(tail_!=nullptr){ 
    tail_->next=newNode;
    newNode->prev=tail_;
    tail_=newNode;
    } else {
      head_=tail_=newNode;
    }
  }else{
    Node<T> * current=pos.node();
    newNode->next=current;
    newNode->prev=current->prev;
    if(current->prev!=nullptr){
      current->prev->next=newNode;
    }else {
    head_=newNode;
    }
    current->prev=newNode;
  }
  size_++;
  return Iterator(newNode);
} 
template<typename T>
class DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::erase(Iterator pos){
if(pos==end()){
  throw std::out_of_range("Out of range!");
  }
  Node<T> *toDelete=pos.p_;
  Node<T> *nextNode=toDelete->next;

  if(toDelete->prev){
  toDelete->prev->next=nextNode;
  }else{
  head_=nextNode;
  }
  if(nextNode){
  toDelete->prev->next=nextNode;
  }else{
  tail_=toDelete->prev;
  }

  delete toDelete;
  size_--;

  if(nextNode)
    return Iterator(nextNode);
  else
   return end();

}
template<typename T>
class DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::erase(Iterator beg, Iterator end){
  if(beg==Iterator(nullptr) || end==Iterator(nullptr)){
    throw std::out_of_range("error");
  }
  Node<T> *current = beg.p_;
  Node<T> *prevv = beg.p_->prev;
  Node<T> *last = end.p_;
  while(current!=last){
    Node<T> *next=current->next;
    delete current;
    current=next;
    size_--;
  }
  if(prevv){
    prevv->next=last;
  }else{
    head_=last;
  }
  if(last){
    last->prev=prevv;
  }else{
    tail_=prevv;
  }
  return Iterator(last);
  }
template <typename T>
 void DoubleLinkedList<T>::remove_if(std::function<bool(const T &)> &&p){
     Node<T> *current = head_;

    while (current != nullptr) {
        Node<T> *next = current->next;
        Node<T> *prev = current->prev;

        if (p(current->data)) {
            if (current == head_) {
                head_ = next;
                if (head_) {
                    head_->prev = nullptr;
                }
            } else {
                prev->next = next;
            }

            if (current == tail_) {
                tail_ = prev;
                if (tail_) {
                    tail_->next = nullptr;
                }
            } else {
                if (next) {
                    next->prev = prev;
                }
            }

            delete current;
            size_--;
            current = next;
        } else {
            current = next;
        }
    }

    if (head_ == nullptr) {
        tail_ = nullptr;
    }}

template <typename T>
void DoubleLinkedList<T>::reverse(){
  Node<T> *current=head_;
  Node<T> *tmp=nullptr;
  while(current!=nullptr){
    tmp=current->prev;
    current->prev=current->next;
    current->next=tmp;
    current=current->prev;
  }
  if(tmp!=nullptr){
  tail_=head_;
  head_=tmp->prev;
  }
}
template <typename T>
class DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::find(const T &element) const{
Node<T> *current=head_;
while(current!=nullptr){
    Node<T> *tmp=current->next;
    if(current->data==element)
      return Iterator(current);
    current=tmp;
  }
  return end();
}

