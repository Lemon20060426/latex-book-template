#include "../inc/Interface.h"
#include "../inc/System.h"
#include "../inc/Git.h"
#include "../inc/fileio.h"

#include <iostream>
#include <string>

std::string filename = "../mode.txt";

// ===== Global variable controlling commit mode =====
bool commit_all = std::stoi(FileIO::read(filename));

// ===== Git Setting Menu =====
int gitSettingMenu() {
    Interface gs("Git Setting Menu");
    gs.addOption("Save whole project (LaTeX + PDF)");
    gs.addOption("Save output (PDF only)");
    gs.addOption("Back");
    gs.addOption("Quit");

    bool running = true;
    while (running) {
        clearScreen();
        gs.showOptions();
        int choice = gs.get_input();

        switch (choice) {
        case 1:
            commit_all = true;
            FileIO::write(filename, "1");
            std::cout << "[git setting] Mode set to: Save whole project (LaTeX + PDF)\n";
            waitForEnter();
            break;
        case 2:
            commit_all = false;
            FileIO::write(filename, "0");
            std::cout << "[git setting] Mode set to: Save output only (PDF)\n";
            waitForEnter();
            break;
        case 3:
            return 0; // back
        case 4:
            return 1; // quit entire program
        default:
            std::cout << "Invalid choice.\n";
            waitForEnter();
            break;
        }
    }
    return 0;
}

int main() {
    Interface ui("Latex Compiler Menu");

    ui.addOption("Compile");
    ui.addOption("Recompile");
    ui.addOption("Clean");
    ui.addOption("View");
    ui.addOption("Git");
    ui.addOption("Git Setting");   // <-- new option here
    ui.addOption("View on VSCode");
    ui.addOption("Publish PDF");
    ui.addOption("Quit");

    bool running = true;

    while (running) {
        clearScreen();
        ui.showOptions();
        int choice = ui.get_input();

        switch (choice) {
        case 1:
            std::cout << "[compile] Running: make all\n";
            run("make all");
            waitForEnter();
            break;
        case 2:
            std::cout << "[recompile] Running: make rebuild\n";
            run("make rebuild");
            waitForEnter();
            break;
        case 3:
            std::cout << "[clean] Running: make clean\n";
            run("make clean");
            waitForEnter();
            break;
        case 4:
            std::cout << "[view] Running: make open\n";
            run("make open");
            waitForEnter();
            break;
        case 5:
            std::cout << "[git] Opening Git menu...\n";
            if (gitMenu() != 0) {
                running = false; // exit if gitMenu signals quit
                break;
            }
            waitForEnter();
            break;
        case 6:
            std::cout << "[git setting] Opening Git Setting menu...\n";
            if (gitSettingMenu() != 0) {
                running = false; // quit program from gitSettingMenu
                break;
            }
            break;
        case 7:
            std::cout << "[vscode] Opening VSCode...\n";
            run("code .");
            waitForEnter();
            break;
        case 8: {
            std::cout << "[publish] Exporting PDF...\n";
            std::cout << "Enter filename (without .pdf): ";
            std::string name;
            std::cin >> name;
            if (name.empty()) name = "main";
            run("make publish NAME=" + name);
            waitForEnter();
            break;
        }
        case 9:
            std::cout << "[quit] Exiting program.\n";
            running = false;
            break;
        default:
            std::cout << "Invalid choice, try again.\n";
            waitForEnter();
            break;
        }
    }

    return 0;
}
