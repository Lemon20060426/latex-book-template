#include "../inc/System.h"
#include <iostream>
#include <cstdlib>
#include <string>

void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
    std::cin.get();
}

void run(const std::string& target) {
    static const std::string go_path = "cd ../../.. && ";
    std::system((go_path + target).c_str());
}
