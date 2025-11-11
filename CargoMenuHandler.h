#pragma once

#include "MenuHandler.h"
#include "CargoList.h"


class CargoMenuHandler : public MenuHandler {
private:
    CargoList& cargoList;
    
    void printMenu();
    
public:
    explicit CargoMenuHandler(CargoList& cList);
    void run() override;
};
