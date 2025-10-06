#include "../inc/Interface.h"
#include <iostream>

void Interface::addOption(const std::string& option) {
    if (!options.find(option)) {
        options.add(option);
    }
}

void Interface::showOptions() const {
    std::cout << "=== "+ InterfaceName + " ===\n";
    Node* cur = options.get_head();  
    int idx = 1;
    while (cur) {
        std::cout << idx++ << ") " << cur->data << "\n";
        cur = cur->next;
    }
    std::cout << "=========================\n";
}

int Interface::get_input() const {
    int choice;
    std::cout << "Enter choice number: ";
    std::cin >> choice;
    return choice;
}
