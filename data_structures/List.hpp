#pragma once
#include <cstddef> // std::ptrdiff_t
#include <iostream>
#include <iterator> // std::forward_iterator_tag

template <typename T>
class List {
private:
  struct Node {
    T data;
    Node* sgte;
    Node(const T& v) : data(v), sgte(nullptr) {}
  };

  Node* head;
  Node* tail;
  size_t count;

public:
  List() : head(nullptr), tail(nullptr), count(0) {}

  // Evitamos copias
  List(const List&) = delete;
  List& operator=(const List&) = delete;

  // Constructor por movimiento
  List(List&& other) noexcept :
      head(other.head),
      tail(other.tail),
      count(other.count) {
    other.head = other.tail = nullptr;
    other.count = 0;
  }

  // Operador de asignación por movimiento
  List& operator=(List&& other) noexcept {
    if (this != &other) {
      clear();

      head = other.head;
      tail = other.tail;
      count = other.count;

      other.head = other.tail = nullptr;
      other.count = 0;
    }
    return *this;
  }

  ~List() {
    clear();
  }

  void push_back(const T& v) {
    Node* newNode = new Node(v);
    if (!head) {
      head = tail = newNode;
    } else {
      tail->sgte = newNode;
      tail = newNode;
    }
    ++count;
  }

  void push_front(const T& v) {
    Node* newNode = new Node(v);
    newNode->sgte = head;
    head = newNode;
    if (!tail)
      tail = newNode;
    ++count;
  }

  void clear() {
    Node* current = head;
    while (current) {
      Node* tmp = current;
      current = current->sgte;
      delete tmp;
    }
    head = tail = nullptr;
    count = 0;
  }

  bool empty() const {
    return head == nullptr;
  }

  size_t size() const {
    return count;
  }

  class Iterator {
    Node* current;

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using reference = const T&;
    using pointer = const T*;
    using difference_type = std::ptrdiff_t;

    Iterator(Node* node) : current(node) {}

    bool operator!=(const Iterator& other) const {
      return current != other.current;
    }

    reference operator*() const {
      if (!current) {
        std::cerr << "ERROR: desreferenciando iterador nulo (nullptr)\n";
        std::abort();
      }
      return current->data;
    }

    Iterator& operator++() {
      if (current)
        current = current->sgte;
      return *this;
    }
  };

  Iterator begin() const {
    return Iterator(head);
  }

  Iterator end() const {
    return Iterator(nullptr);
  }
};
