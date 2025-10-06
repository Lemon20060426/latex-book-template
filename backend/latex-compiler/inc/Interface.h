#ifndef INTERFACE_H
#define INTERFACE_H

#include "../inc/LinkedList.h"
#include <string>

class Interface {
private:
    std::string InterfaceName;
    LinkedList options;

public:
    Interface(std::string InterfaceName = "Available Options") : InterfaceName(InterfaceName), options() {};
    void addOption(const std::string& option);
    void showOptions() const;
    int get_input() const;
};

#endif
