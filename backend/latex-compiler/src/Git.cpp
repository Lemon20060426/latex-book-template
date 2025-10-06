#include "../inc/Git.h"
#include "../inc/System.h"
#include "../inc/Interface.h"
#include <iostream>
#include <string>

// Show tag -> commit mapping history
static void showHistory()
{
    std::cout << "=== History ===\n";
    run(
        "git tag -l --sort=-creatordate "
        "--format=\"%(refname:short) -> %(objectname:short) %(subject) (%(creatordate:format:%Y-%m-%d %H:%M:%S))\""
    );
    std::cout << "================\n";
}

int gitMenu()
{
    Interface gitUi("Git Menu");
    gitUi.addOption("Initialize/Reinitialize locally"); // 1
    gitUi.addOption("Initialize GitHub");               // 2
    gitUi.addOption("Save changes locally");            // 3
    gitUi.addOption("Save changes on GitHub");          // 4
    gitUi.addOption("History");                         // 5
    gitUi.addOption("Back");                            // 6
    gitUi.addOption("Quit");                            // 7

    bool inGit = true;
    while (inGit)
    {
        clearScreen();
        gitUi.showOptions();
        int choice = gitUi.get_input();

        switch (choice)
        {
        case 1:
        { // Initialize/Reinitialize locally
#ifdef _WIN32
            run("rmdir /s /q .git");
#else
            run("rm -rf .git");
#endif
            run("git init");
            if (commit_all) run("git add .");
            else run("git add output/main.pdf");
            run("git commit -m \"first commit\"");
            run("git branch -M main");
            run("git tag v0.0.1-alpha"); // create initial local tag
            waitForEnter();
            break;
        }

        case 2:
        { // Initialize GitHub (requires local init first)
            std::cout << "Do you save locally? (Y/N): ";
            char ans;
            std::cin >> ans;

            if (ans != 'Y' && ans != 'y')
            {
                std::cout << "Please save locally first.\n";
                waitForEnter();
                break;
            }

            std::string user, repo;
            std::cout << "Enter GitHub username: ";
            std::cin >> user;
            std::cout << "Enter repository name: ";
            std::cin >> repo;

            run("git remote remove origin");
            run("git remote add origin https://github.com/" + user + "/" + repo + ".git");
            run("git push -u origin main");
            run("git push --tags"); // push all local tags

            waitForEnter();
            break;
        }

        case 3:
        { // Save changes locally
            std::cout << "Do you save locally? (Y/N): ";
            char ans;
            std::cin >> ans;

            if (ans != 'Y' && ans != 'y')
            {
                std::cout << "Please save locally first.\n";
                waitForEnter();
                break;
            }

            showHistory();

            std::cin.ignore();
            std::string msg;
            std::cout << "Enter commit message: ";
            std::getline(std::cin, msg);

            std::string tag;
            std::cout << "Enter tag for this commit: ";
            std::cin >> tag;

            run("git add .");
            run("git commit -m \"" + msg + "\"");
            run("git tag " + tag); // lightweight

            waitForEnter();
            break;
        }

        case 4:
        { // Save changes on GitHub
            std::cout << "Do you save on GitHub? (Y/N): ";
            char ans;
            std::cin >> ans;

            if (ans != 'Y' && ans != 'y')
            {
                std::cout << "Please save locally first.\n";
                waitForEnter();
                break;
            }

            run("git push origin main");
            run("git push --tags"); // push all existing local tags

            waitForEnter();
            break;
        }

        case 5:
        { // History
            showHistory();
            waitForEnter();
            break;
        }

        case 6: // Back
            inGit = false;
            break;

        case 7: // Quit
            std::cout << "[quit] Exiting program.\n";
            return -1;

        default:
            std::cout << "Invalid choice.\n";
            waitForEnter();
            break;
        }
    }
    return 0;
}
