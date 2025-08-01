#ifndef QUEUE_H
#define QUEUE_H

template <typename T>
class Queue {
private:
  struct Node {
    T data;
    Node* next;
    Node(const T& val) : data(val), next(nullptr) {}
  };

  Node* frontNode;
  Node* backNode;
  int count;

public:
  Queue() : frontNode(nullptr), backNode(nullptr), count(0) {}

  ~Queue() {
    while (!empty()) {
      pop();
    }
  }

  void push(const T& value) {
    Node* newNode = new Node(value);
    if (empty()) {
      frontNode = backNode = newNode;
    } else {
      backNode->next = newNode;
      backNode = newNode;
    }
    ++count;
  }

  void pop() {
    if (empty())
      return;
    Node* temp = frontNode;
    frontNode = frontNode->next;
    delete temp;
    --count;
    if (frontNode == nullptr)
      backNode = nullptr;
  }

  T& front() {
    return frontNode->data;
  }

  const T& front() const {
    return frontNode->data;
  }

  bool empty() const {
    return count == 0;
  }

  int size() const {
    return count;
  }
};

template <typename T, int CAPACITY = 1000000>
class PriorityQueue {
private:
  T data[CAPACITY];
  int count;

  // Función auxiliar para comparar dos elementos (min-heap)
  bool less(int i, int j) const {
    return data[i].first < data[j].first;
  }

  void swap(int i, int j) {
    T temp = data[i];
    data[i] = data[j];
    data[j] = temp;
  }

  void siftUp(int index) {
    while (index > 0) {
      int parent = (index - 1) / 2;
      if (less(index, parent)) {
        swap(index, parent);
        index = parent;
      } else {
        break;
      }
    }
  }

  void siftDown(int index) {
    while (2 * index + 1 < count) {
      int left = 2 * index + 1;
      int right = 2 * index + 2;
      int smallest = left;

      if (right < count && less(right, left)) {
        smallest = right;
      }

      if (less(smallest, index)) {
        swap(index, smallest);
        index = smallest;
      } else {
        break;
      }
    }
  }

public:
  PriorityQueue() : count(0) {}

  bool empty() const {
    return count == 0;
  }

  bool full() const {
    return count == CAPACITY;
  }

  int size() const {
    return count;
  }

  void push(const T& value) {
    if (full())
      return; // O lanzar excepción
    data[count] = value;
    siftUp(count);
    ++count;
  }

  void pop() {
    if (empty())
      return; // O lanzar excepción
    data[0] = data[count - 1];
    --count;
    siftDown(0);
  }

  T& top() {
    return data[0];
  }

  const T& top() const {
    return data[0];
  }

  void clear() {
    count = 0;
  }
};

#endif
