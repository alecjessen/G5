#ifndef ORDEREDLINKEDLIST_H
#define ORDEREDLINKEDLIST_H

#include <iostream>

template <typename T>
struct Node {
  T data;
  Node<T> *next;
  Node(const T &d) : data(d), next(nullptr) {}
};

template <typename T>
class OrderedLinkedList {
private:
  Node<T> *head;
  int count;

public:
  OrderedLinkedList() : head(nullptr), count(0) {}

  ~OrderedLinkedList() {
    destroyList();
  }

  // Provide raw head pointer for scenarios (like hop counting) that need node access.
  Node<T>* headPtr() const { return head; }

  void destroyList() {
    Node<T> *current = head;
    while (current != nullptr) {
      Node<T> *temp = current;
      current = current->next;
      delete temp;
    }
    head = nullptr;
    count = 0;
  }

  // Insert maintaining order.
  // Assumes T supports operator< and operator> or comparisons.
  // If T is a pointer type, we compare the dereferenced values.
  void insert(const T &item) {
    Node<T> *newNode = new Node<T>(item);

    // Case 1: Empty list or item smaller than head
    if (head == nullptr || compare(item, head->data) < 0) {
      newNode->next = head;
      head = newNode;
    } else {
      // Case 2: Find insertion point
      Node<T> *current = head;
      Node<T> *previous = nullptr;

      // While current is smaller than item
      while (current != nullptr && compare(item, current->data) >= 0) {
        previous = current;
        current = current->next;
      }
      newNode->next = current;
      previous->next = newNode;
    }
    count++;
  }

  // Remove an item.
  // Returns true if found and removed.
  bool remove(const T &item) {
    if (head == nullptr) return false;

    if (compare(item, head->data) == 0) {
      Node<T> *temp = head;
      head = head->next;
      delete temp;
      count--;
      return true;
    }

    Node<T> *current = head;
    Node<T> *previous = nullptr;

    while (current != nullptr && compare(item, current->data) != 0) {
      previous = current;
      current = current->next;
    }

    if (current != nullptr) {
      previous->next = current->next;
      delete current;
      count--;
      return true;
    }

    return false;
  }

  // Remove node at specific index (0-based)
  // Returns the data removed
  T removeAt(int index) {
      if (index < 0 || index >= count) return T{};

      Node<T>* current = head;
      Node<T>* previous = nullptr;

      if (index == 0) {
          head = head->next;
          T data = current->data;
          delete current;
          count--;
          return data;
      }

      for (int i = 0; i < index; i++) {
          previous = current;
          current = current->next;
      }

      previous->next = current->next;
      T data = current->data;
      delete current;
      count--;
      return data;
  }

  // Get item at index (0-based)
  // Returns T (copy or pointer)
  T getAt(int index) const {
    if (index < 0 || index >= count) return T{};
    Node<T> *current = head;
    for (int i = 0; i < index; ++i) {
      current = current->next;
    }
    return current->data;
  }

  // Look up item. Returns pointer to Node if found, or nullptr.
  // This supports the "return pointer to node" requirement.
  Node<T>* search(const T &item) const {
      Node<T>* current = head;
      while (current != nullptr) {
          if (compare(item, current->data) == 0) {
              return current;
          }
          current = current->next;
      }
      return nullptr;
  }

  int size() const { return count; }
  bool isEmpty() const { return count == 0; }

  // Basic iterator support
  class Iterator {
      Node<T>* current;
  public:
      Iterator(Node<T>* node) : current(node) {}
      T& operator*() { return current->data; }
      T operator->() { return current->data; } // For pointers
      Iterator& operator++() { 
          if(current) current = current->next; 
          return *this; 
      }
      bool operator!=(const Iterator& other) const { return current != other.current; }
      bool operator==(const Iterator& other) const { return current == other.current; }
  };

  Iterator begin() const { return Iterator(head); }
  Iterator end() const { return Iterator(nullptr); }

private:
  // Helper to compare T, handling both values and pointers
  template <typename U>
  int compare(const U &a, const U &b) const {
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
  }

  // Specialization for pointers
  template <typename U>
  int compare(U *const &a, U *const &b) const {
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
  }
};

#endif
