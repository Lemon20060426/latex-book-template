#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <string>

struct Node {
    std::string data;
    Node* next;
    Node(const std::string& d) : data(d), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;

public:
    LinkedList();
    ~LinkedList();

    void add(const std::string& value);
    void print() const;
    bool find(const std::string& value) const;
    Node* get_head() const;
};

#endif
