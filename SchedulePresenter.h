#pragma once

#include "Shipment.h"
#include "freight.h"
#include "Cargo.h"
#include <vector>
#include <ostream>
#include <unordered_map>

using namespace std;

class SchedulePresenter {
public:
    static void tableByCargoArrival(const  vector<Shipment>& plans,  ostream& os);

    static void groupingByArrival(const  vector<Shipment>& plans,
        const vector<freight>& freights,
        const vector<Cargo>& cargos,
         ostream& os,
         unordered_map<int,  vector<int>>& outGroups);

    static void displayScheduleByCargoArrivalTime(const  vector<Shipment>& plans,  ostream& os);

    // overloaded versions that accept shipments for cargo splitting
    static void displayGroupingByArrival( ostream& os,
        const vector<Shipment>& plans,
        const vector<freight>& freights,
        const  vector<Cargo>& cargos);

    static void displayGroupingByLeastFreights( ostream& os,
        const vector<Shipment>& plans,
        const vector<freight>& freights,
        const vector<Cargo>& cargos);

 
    static void displayGroupingByArrival( ostream& os,
        const unordered_map<int, vector<int>>& groups,
        const vector<freight>& freights,
        const vector<Cargo>& cargos);
};
