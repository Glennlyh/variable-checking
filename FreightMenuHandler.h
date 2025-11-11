#pragma once

#include "MenuHandler.h"
#include "freightlist.h"
#include "CargoList.h"
#include "AutoRebalancer.h"
#include <unordered_map>
#include <vector>


class FreightMenuHandler : public MenuHandler 
{
private:
    freightlist& freightList;
    CargoList& cargoList;
    AutoRebalancer* autoRebalancer;
    
    // Store current schedule groupings for rebalancing
    std::unordered_map<int, std::vector<int>> currentGrouping;
    
    void printMenu();
    
public:
    explicit FreightMenuHandler(freightlist& fList, CargoList& cList);
    ~FreightMenuHandler();
    void run() override;
    
    // Set the current schedule grouping (called from SchedulerMenuHandler)
    void setCurrentGrouping(const std::unordered_map<int, std::vector<int>>& grouping);
};
