#include "../inc/LinkedList.h"

LinkedList::LinkedList() : head(nullptr) {}

LinkedList::~LinkedList() {
    Node* cur = head;
    while (cur) {
        Node* tmp = cur;
        cur = cur->next;
        delete tmp;
    }
}

void LinkedList::add(const std::string& value) {
    Node* newNode = new Node(value);
    if (!head) {
        head = newNode;
        return;
    }
    Node* cur = head;
    while (cur->next) cur = cur->next;
    cur->next = newNode;
}

void LinkedList::print() const {
    Node* cur = head;
    while (cur) {
        std::cout << cur->data << " -> ";
        cur = cur->next;
    }
    std::cout << "NULL\n";
}

bool LinkedList::find(const std::string& value) const {
    Node* cur = head;
    while (cur) {
        if (cur->data == value) return true;
        cur = cur->next;
    }
    return false;
}

Node* LinkedList::get_head() const {
    return head;
}
