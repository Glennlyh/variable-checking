#pragma once

#include "freightlist.h"
#include "CargoList.h"
#include "Schduler.h"
#include "Shipment.h"
#include <unordered_map>
#include <vector>

class SchedulerMenuHandler {
public:
    SchedulerMenuHandler(freightlist& fList, CargoList& cList, Schduler& sched);
    void run();

private:
    void printMenu();

    freightlist& freightList;
    CargoList& cargoList;
    Schduler& scheduler;

    // store both the old groups format (for saving/analysis) and new shipments format (for display)
    std::unordered_map<int, std::vector<int>> lastGroupsArrival;
    std::unordered_map<int, std::vector<int>> lastGroupsLeast;

    // store shipments with cargo quantities for display
    std::vector<Shipment> lastShipmentsArrival;
    std::vector<Shipment> lastShipmentsLeast;
};
