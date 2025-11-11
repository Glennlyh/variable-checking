#include "MenuManager.h"
#include <fstream>
#include <iostream>
#include <limits>

using namespace std;

static bool file_exists(const  string& path)
{
     ifstream f(path.c_str());
    return f.good();
}

MenuManager::MenuManager()
    : state(MENU_SCHEDULER),
      freightMenuHandler(freightList),
      cargoMenuHandler(cargoList),
      schedulerMenuHandler(freightList, cargoList, scheduler)
{
    using std::cout;                                    // will not work without std:: even thought used namespace std at the top
    using std::endl;

    cout << "Preloading freight.txt and cargo.txt..." << endl << endl;

    if (file_exists("freight.txt")) {
        if (freightList.loadFromFile("freight.txt")) {
            cout << "Loaded freight.txt" << endl;
        } else
        {
           cout << "Failed to load freight.txt" << endl;
        }
    }
    if (file_exists("cargo.txt")) 
    {
        if (cargoList.loadFromFile("cargo.txt")) 
        {
           cout << "Loaded cargo.txt" << endl;
        } else 
        {
           cout << "Failed to load cargo.txt" << endl;
        }
    }
}

void MenuManager::run() 
{
    while (state != MENU_EXIT)
    {
        cout << "\n=== Main Menu ===\n"
                  << " 1) Freight Menu\n"
                  << " 2) Cargo Menu\n"
                  << " 3) Scheduler Menu\n"
                  << " 9) Exit\n"
                  << "Choice: ";
        
        int choice{};
        cin >> choice;
        cin.clear();
        cin.ignore( numeric_limits< streamsize>::max(), '\n');
        
        switch (choice) 
        {
        case 1:
            freightMenuHandler.run();
            break;
        case 2:
            cargoMenuHandler.run();
            break;
        case 3:
            schedulerMenuHandler.run();
            break;
        case 9:
            state = MENU_EXIT;
            break;
        default:
             cout << "Invalid choice.\n";
            break;
        }
    }
    cout << "Goodbye.\n";
}
