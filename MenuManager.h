#pragma once
#include <iostream>
#include <string>
#include "freightlist.h"
#include "CargoList.h"
#include "Schduler.h"
#include "FreightMenuHandler.h"
#include "CargoMenuHandler.h"
#include "SchedulerMenuHandler.h"

// controls which menu is active
enum MenuState {  MENU_FREIGHT, MENU_CARGO, MENU_SCHEDULER, MENU_EXIT };

class MenuManager 
{
private:
    freightlist freightList;
    CargoList cargoList;
    Schduler scheduler;
    MenuState state;

    FreightMenuHandler freightMenuHandler;
    CargoMenuHandler cargoMenuHandler;
    SchedulerMenuHandler schedulerMenuHandler;

public:
    MenuManager();
    void run();                // main controller
};

