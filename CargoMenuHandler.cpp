#include "CargoMenuHandler.h"
#include "functions.h"
#include "Cargo.h"
#include <iostream>
#include <limits>
#include <string>

static std::string promptLine(const std::string& prompt)   
{
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

static int promptInt(const std::string& prompt) 
{
    std::cout << prompt;
    int x{};
    std::cin >> x;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return x;
}

static void clearInput() 
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

CargoMenuHandler::CargoMenuHandler(CargoList& cList)
    : cargoList(cList) {}

void CargoMenuHandler::printMenu() 
{
    std::cout << "\n=== Cargo Menu ===\n"
              << " 1) Load cargo from file\n"
              << " 2) Save cargo to file\n"
              << " 3) Add cargo\n"
              << " 4) List all cargo\n"
              << " 0) Return to main menu\n"
              << "Choice: ";
}

void CargoMenuHandler::run() 
{
    while (true) {
        printMenu();
        int choice{};
        std::cin >> choice;
        clearInput();

        switch (choice) 
        {
        case 1: 
        {
            std::string path = promptLine("Enter input filename (.txt): ");
            if (cargoList.loadFromFile(path)) {
                std::cout << "Loaded from " << path << '\n';
            }
            else 
            {
                std::cout << "Failed to load " << path << '\n';
            }
            break;
        }

        case 2: 
        {
            std::string path = promptLine("Enter output filename (.txt): ");
            if (cargoList.saveToFile(path)) 
            {
                std::cout << "Saved to " << path << '\n';
            }
            else 
            {
                std::cout << "Failed to save to " << path << '\n';
            }
            break;
        }

        case 3: 
        {
            int id   = promptInt("Enter cargo index (int): ");
            int time = promptInt("Enter cargo arrival time (HHMM 0000..2359): ");
            std::string dest = promptLine("Enter destination (e.g. SIN/HKG/NRT...): ");

            if (!isValidTime(time)) 
            {
                std::cout << "Invalid time. Use HHMM (0000..2359 with MM<60)\n";
                break;
            }
            if (!isValidDestination(dest)) 
            {
                std::cout << "Invalid destination.\n";
                break;
            }
            Cargo c(id, dest, time);
            if (cargoList.add(c)) 
            {
                std::cout << "Cargo added.\n";
            }
            else 
            {
                std::cout << "Failed to add cargo (maybe duplicate ID?).\n";
            }
            break;
        }

        case 4: 
        {
            auto all = cargoList.all();
            if (all.empty()) 
            {
                std::cout << "(no cargo loaded)\n";
            } else 
            {
                std::cout << "Index  Destination   Time\n";
                for (const auto& c : all) 
                {
                    std::cout << "C" << c.getIndex() << "     "
                              << c.getDestination() << "          "
                              << formatTime12h(c.getTime()) << '\n';
                }
            }
            break;
        }

        case 0:
            return; // return to main menu

        default:
            std::cout << "Invalid choice.\n";
            break;
        }
    }
}
