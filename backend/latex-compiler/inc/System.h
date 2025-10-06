#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>

// Clear the terminal screen
void clearScreen();

// Wait for user to press Enter
void waitForEnter();

// Run a make target from project root (../../..)
void run(const std::string& target);

#endif // SYSTEM_H
