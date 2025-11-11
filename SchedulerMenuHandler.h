#pragma once

#include "freightlist.h"
#include "CargoList.h"
#include "Schduler.h"
#include "Shipment.h"
#include <unordered_map>
#include <vector>

using namespace std;

// Forward declaration
class FreightMenuHandler;

class SchedulerMenuHandler {
public:
    SchedulerMenuHandler(freightlist& fList, CargoList& cList, Schduler& sched);
    void run();
    
    // Allow FreightMenuHandler to access current grouping
    void setFreightMenuHandler(FreightMenuHandler* handler);

private:
    void printMenu();

    freightlist& freightList;
    CargoList& cargoList;
    Schduler& scheduler;
    
    FreightMenuHandler* freightMenuHandler_;

    // store both the old groups format (for saving/analysis) and new shipments format (for display)
     unordered_map<int,  vector<int>> lastGroupsArrival;
     unordered_map<int,  vector<int>> lastGroupsLeast;

    // store shipments with cargo quantities for display
     vector<Shipment> lastShipmentsArrival;
     vector<Shipment> lastShipmentsLeast;
};
