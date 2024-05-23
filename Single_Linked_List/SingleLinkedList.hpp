#pragma once
#include <functional>
#include <iostream>
#include <iterator>
#include <stdexcept>

template <typename T> struct Node {
  T data;
  Node *next;
  Node(const T &element) : data{element}, next{nullptr} {}
};

template <typename T> class SingleLinkedList {
public:
  class Iterator;
  SingleLinkedList();
  SingleLinkedList(const SingleLinkedList &);
  SingleLinkedList(SingleLinkedList &&);
  SingleLinkedList &operator=(const SingleLinkedList &);
  SingleLinkedList &operator=(SingleLinkedList &&);
  ~SingleLinkedList();
  SingleLinkedList &push_back(const T &);
  SingleLinkedList &push_front(const T &);
  SingleLinkedList &pop_front();
  SingleLinkedList &pop_back();
  bool empty() const;
  size_t size() const;
  SingleLinkedList &extend(const SingleLinkedList &);
  Iterator find(const T &element) const;
  Iterator insert(Iterator pos, const T &element);
  Iterator erase(Iterator pos);
  Iterator begin() const { return Iterator(head_); };
  Iterator end() const { return Iterator(tail_->next); };
  const Iterator cbegin() const{
    return Iterator(head_);
  };
  const Iterator cend() const{
    return Iterator(tail_->next);
  };
  const Iterator rcbegin() const{
    return Iterator(tail_);
  };
  Iterator rbegin() const{
    return Iterator(tail_);
  };
  Node<T> *head() const;
  Node<T> *tail() const;
  const T &back() const{
    return tail_->data;

  };
  const T &front() const{
    return head_->data;
  };
  Iterator erase(Iterator beginIt, Iterator endIt);
  void clear();
  void remove_if(std::function<bool(const T &)> &&p);
  void reverse();

private:
  void copyList(const SingleLinkedList &);
  void dealoc() {
    Node<T> *tmp;
    while (head_) {
      tmp = head_;
      head_ = head_->next;
      delete tmp;
    }
    tail_ = head_ = nullptr;
    size_ = 0;
  }
  Node<T> *head_{nullptr};
  Node<T> *tail_{nullptr};
  size_t size_{0};
};

template <typename T>
class SingleLinkedList<T>::Iterator
    : public std::iterator<std::forward_iterator_tag, T> {
public:
  friend class SingleLinkedList;
  Iterator() { ptr = nullptr; }
  Iterator(const Iterator &other) { ptr = other.ptr; }
  Iterator(Node<T> *other) { ptr = other; }
  Iterator(Iterator &&other) {
    ptr = other.ptr;
    other.ptr = nullptr;
  }
  Iterator &operator=(const Iterator &other) {
    if (this != &other) {
      ptr = other.ptr;
    }
    return *this;
  }
  Iterator &operator=(Iterator &&other) {
    if (this != &other) {
      ptr = other.ptr;
      other.ptr = nullptr;
    }
    return *this;
  }
  ~Iterator() { ptr = nullptr; }
  Iterator &operator++() {
    if (ptr) {
      ptr = ptr->next;
    }
    return *this;
  }
  Iterator operator++(int) {
    Iterator tmp = *this;
    if (ptr) {
      ptr = ptr->next;
    }
    return tmp;
  }

  T &operator*() { return ptr->data; }
  T operator->() { return &(ptr->data); }
  bool operator==(const Iterator &other) const { return ptr == other.ptr; }
  bool operator!=(const Iterator &other) const { return ptr != other.ptr; }
  Node<T> *node() const { return ptr; }

private:
  Node<T> *ptr;
};

template <typename T> SingleLinkedList<T>::SingleLinkedList() {
  head_ = tail_ = nullptr;
  size_ = 0;
}

template <typename T>
SingleLinkedList<T>::SingleLinkedList(const SingleLinkedList &other) {
  head_ = tail_ = nullptr;
  size_ = 0;
  Node<T> *tmp = other.head_;
  while (tmp) {
    push_back(tmp->data);
    tmp = tmp->next;
  }
}
template <typename T>
SingleLinkedList<T>::SingleLinkedList(SingleLinkedList &&other) {
  head_ = other.head_;
  tail_ = other.tail_;
  size_ = other.size_;
  other.head_ = other.tail_ = nullptr;
  other.size_ = 0;
}

template <typename T> SingleLinkedList<T>::~SingleLinkedList() {
  Node<T> *tmp;
  while (head_) {
    tmp = head_;
    head_ = head_->next;
    delete tmp;
  }
  tail_ = nullptr;
  size_ = 0;
}

template <typename T>
SingleLinkedList<T> &
SingleLinkedList<T>::operator=(const SingleLinkedList &other) {
  if (this != &other) {
    dealoc();
    Node<T> *tmp = other.head_;
    while (tmp) {
      push_back(tmp->data);
      tmp = tmp->next;
    }
  }
  return *this;
}
template <typename T>
SingleLinkedList<T> &SingleLinkedList<T>::operator=(SingleLinkedList &&other) {
  if (this != &other) {
    dealoc();
    head_ = other.head_;
    tail_ = other.tail_;
    size_ = other.size_;
    other.head_ = other.tail_ = nullptr;
    other.size_ = 0;
  }
  return *this;
}
template <typename T>
SingleLinkedList<T> &SingleLinkedList<T>::push_back(const T &elem) {
  Node<T> *tmp = new Node<T>(elem);
  if (size_ == 0) {
    head_ = tail_ = tmp;
  } else {
    tail_->next = tmp;
    tail_ = tmp;
  }
  size_++;
  return *this;
}

template <typename T>
SingleLinkedList<T> &SingleLinkedList<T>::push_front(const T &elem) {
  Node<T> *tmp = new Node<T>(elem);
  if (size_ == 0) {
    head_ = tail_ = tmp;
  } else {
    tmp->next = head_;
    head_ = tmp;
  }
  size_++;
  return *this;
}

template <typename T> SingleLinkedList<T> &SingleLinkedList<T>::pop_front() {
  if (empty())
    throw std::out_of_range("List is empty!");
  else {
    if (size_ == 1) {
      delete head_;
      head_ = tail_ = nullptr;
    } else {
      Node<T> *tmp = head_;
      head_ = head_->next;
      delete tmp;
    }
    size_--;
    return *this;
  }
}

template <typename T>
SingleLinkedList<T> &
SingleLinkedList<T>::extend(const SingleLinkedList<T> &other) {
  if (other.size_ == 0) {

  } else {
    Node<T> *tmp = other.head_;
    while (tmp) {
      push_back(tmp->data);
      tmp = tmp->next;
    }
  }
  return *this;
}
template <typename T> SingleLinkedList<T> &SingleLinkedList<T>::pop_back() {
  if (empty())
    throw std::out_of_range("List is empty!");
  else {
    if (size_ == 1) {
      delete tail_;
      head_ = tail_ = nullptr;
    } else {
      Node<T> *hlp = head_;
      while (hlp->next != tail_) {
        hlp = hlp->next;
      }
      hlp->next = nullptr;
      delete tail_;
      tail_ = hlp;
    }
    size_--;
    return *this;
  }
}

template <typename T> Node<T> *SingleLinkedList<T>::head() const {
  return head_;
}
template <typename T> Node<T> *SingleLinkedList<T>::tail() const {
  return tail_;
}

template <typename T> bool SingleLinkedList<T>::empty() const {
  return size_ == 0;
}

template <typename T> size_t SingleLinkedList<T>::size() const { return size_; }

template <typename T>
class SingleLinkedList<T>::Iterator
SingleLinkedList<T>::find(const T &elem) const {
  Node<T> *tmp = head_;
  while (tmp) {
    if (tmp->data == elem) {
      return Iterator(tmp);
    }
    tmp = tmp->next;
  }
  return end();
}

template <typename T>
class SingleLinkedList<T>::Iterator
SingleLinkedList<T>::insert(class SingleLinkedList<T>::Iterator pos,
                            const T &element) {
  Node<T>*tmp=new Node<T>(element);
  if(head_==nullptr){
  head_=tail_=tmp;
  size_++;
  return Iterator(tmp);
  }
  if(pos==Iterator(head_)){
    tmp->next=head_;
    head_=tmp;
    size_++;
    return Iterator(tmp);
  }
  Node<T> *current=head_;
  while(current->next!=nullptr && Iterator(current->next)!= pos){
  current=current->next;
  }
  tmp->next=current->next;
  current->next=tmp;
  if(current->next==nullptr){
    tail_=tmp;
  }

  size_++;
  return Iterator(tmp);
}

template <typename T>
class SingleLinkedList<T>::Iterator SingleLinkedList<T>::erase(class SingleLinkedList<T>::Iterator pos){
  if(pos== end()){
    return end();
  }
  if(pos==begin()){
    Node<T>*tmp=head_;
    Node<T> *next=tmp->next;
    head_=head_->next;
    delete tmp;
    size_--;
    return Iterator(head_);
    
  } 
  else if(pos==Iterator(tail_)){
  pop_back();
  return Iterator(tail_);
  }else{

  Node<T> *current=pos.node();
  Node<T> *prev=head_;
  Node<T> *next=current->next;
  while(prev->next!=current){
  prev=prev->next;
  }
  prev->next=current->next;
  delete current;
  size_--;
  return Iterator(next);
  }
}

template <typename T>
void SingleLinkedList<T>::reverse(){
  Node<T> *current=head_,*next=nullptr,*prev=nullptr;
  tail_=head_;
  while(current!=nullptr){
    next=current->next;
    current->next=prev;
    prev=current;
    current=next;
  }
  head_=prev;
}

template <typename T>
typename SingleLinkedList<T>::Iterator
SingleLinkedList<T>::erase(Iterator beginIt, Iterator endIt) {
    if (beginIt == endIt) {
        return endIt;
    }

    Node<T>* current = beginIt.node();
    Node<T>* prev = nullptr;
    
    if (beginIt == begin()) {
        head_ = endIt.node();
    } else {
        prev = head_;
        while (prev->next != current) {
            prev = prev->next;
        }
        prev->next = endIt.node();
    }

    while (current != endIt.node()) {
        Node<T>* tmp = current;
        current = current->next;
        delete tmp;
        --size_;
    }

    if (endIt == end()) {
        tail_ = prev;
    }

    return endIt;
}

template <typename T>
void SingleLinkedList<T>::clear() {
    dealoc();
}


template <typename T>
void SingleLinkedList<T>::remove_if(std::function<bool(const T &)> &&p) {
    Node<T>* current = head_;
    Node<T>* prev = nullptr;

    while (current) {
        if (p(current->data)) {
            Node<T>* tmp = current;
            if (prev) {
                prev->next = current->next;
            } else {
                head_ = current->next;
            }
            if (current == tail_) {
                tail_ = prev;
            }
            current = current->next;
            delete tmp;
            --size_;
        } else {
            prev = current;
            current = current->next;
        }
    }
}
