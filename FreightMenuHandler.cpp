#include "FreightMenuHandler.h"
#include "functions.h"
#include "freight.h"
#include <iostream>
#include <limits>
#include <string>

using namespace std;

static  string promptLine(const  string& prompt) 
{
    cout << prompt;
    string s;
    getline( cin, s);
    return s;
}

static int promptInt(const  string& prompt) 
{
    cout << prompt;
    int x{};
    cin >> x;
    cin.ignore( numeric_limits< streamsize>::max(), '\n');
    return x;
}

static void clearInput() 
{
    cin.clear();
    cin.ignore( numeric_limits< streamsize>::max(), '\n');
}

static freight::FreightType askFreightType() 
{
    cout << "Select freight type:\n";
    cout << "  1) MiniMover (cap 2)\n";
    cout << "  2) CargoCruiser (cap 6)\n";
    cout << "  3) MegaCarrier (cap 12)\n";
    cout << "Enter choice: ";
    int t = 0;
    cin >> t;
    cin.ignore( numeric_limits< streamsize>::max(), '\n');
    switch (t) 
    {
    case 1: return freight::FreightType::MiniMover;
    case 2: return freight::FreightType::CargoCruiser;
    case 3: return freight::FreightType::MegaCarrier;
    default: return freight::FreightType::MiniMover;
    }
}

FreightMenuHandler::FreightMenuHandler(freightlist& fList)
    : freightList(fList) {}

void FreightMenuHandler::printMenu() 
{
    cout << "\n=== Freight Menu ===\n"
              << " 1) Load freight from file\n"
              << " 2) Save freight to file\n"
              << " 3) Add freight\n"
              << " 4) List all freight\n"
              << " 0) Return to main menu\n"
              << "Choice: ";
}

void FreightMenuHandler::run() 
{
    while (true) {
        printMenu();
        int choice{};
         cin >> choice;
        clearInput();

        switch (choice) 
        {
        case 1: 
        {
             string path = promptLine("Enter input filename (.txt): ");
            if (freightList.loadFromFile(path))
            {
                 cout << "Loaded from " << path << '\n';
            }
            else 
            {
                 cout << "Failed to load " << path << '\n';
            }
            break;
        }

        case 2: 
        {
             string path = promptLine("Enter output filename (.txt): ");
            if (freightList.saveToFile(path)) {
                cout << "Saved to " << path << '\n';
            }
            else 
            {
                cout << "Failed to save to " << path << '\n';
            }
            break;
        }

        case 3: 
        {
            int id   = promptInt("Enter freight index (int): ");
            int time = promptInt("Enter freight time (HHMM 0000..2359): ");
             string dest = promptLine("Enter destination (e.g. SIN/HKG/NRT...): ");

            if (!isValidTime(time))
            {
                 cout << "Invalid time. Use HHMM (0000..2359 with MM<60)\n";
                break;
            }
            if (!isValidDestination(dest))
            {
                 cout << "Invalid destination.\n";
                break;
            }
            auto type = askFreightType();
            freight f(id, dest, time, type);
            if (freightList.add(f))
            {
                 cout << "Freight added.\n";
            }
            else
            {
                 cout << "Failed to add freight (maybe duplicate ID?).\n";
            }
            break;
        }

        case 4:
        {
            auto all = freightList.all();
            if (all.empty()) 
            {
                cout << "(no freight loaded)\n";
            } else 
            {
                cout << "Index  Type         Destination   Time\n";
                for (const auto& f : all) 
                {
                    cout << "F" << f.getIndex() << "     "
                              << f.getTypeName() << "   "
                              << f.getDestination() << "          "
                              << formatTime12h(f.getTime()) << '\n';
                }
            }
            break;
        }

        case 0:
            return; // return to main menu

        default:
            cout << "Invalid choice.\n";
            break;
        }
    }
}
