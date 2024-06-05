#pragma once
#include <iostream>

template <typename T> class Node {
public:
  T data;
  Node *left;
  Node *right;

  Node(T value) : data(value), left(nullptr), right(nullptr) {}
};

template <typename T> class BinaryTree {
public:
BinaryTree() : root_(nullptr){}
~BinaryTree();

void insert(T value);
void inorder()const;
void preorder() const;
void postorder() const;
bool search(T value) const;
  
private:
  Node<T> *root_;
  void insert(Node<T> *&node, T value);
  void inorder(Node<T> *node) const;
  void preorder(Node<T> *node) const;
  void postorder(Node<T> *node) const;
  bool search(Node<T> *node, T value) const;
  void destroyTree(Node<T> *node);
};
