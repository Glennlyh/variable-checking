#pragma once

#include "Shipment.h"
#include "freight.h"
#include "Cargo.h"
#include <vector>
#include <ostream>
#include <unordered_map>

class SchedulePresenter {
public:
    static void tableByCargoArrival(const std::vector<Shipment>& plans, std::ostream& os);

    static void groupingByArrival(const std::vector<Shipment>& plans,
        const std::vector<freight>& freights,
        const std::vector<Cargo>& cargos,
        std::ostream& os,
        std::unordered_map<int, std::vector<int>>& outGroups);

    static void displayScheduleByCargoArrivalTime(const std::vector<Shipment>& plans, std::ostream& os);

    // overloaded versions that accept shipments for cargo splitting
    static void displayGroupingByArrival(std::ostream& os,
        const std::vector<Shipment>& plans,
        const std::vector<freight>& freights,
        const std::vector<Cargo>& cargos);

    static void displayGroupingByLeastFreights(std::ostream& os,
        const std::vector<Shipment>& plans,
        const std::vector<freight>& freights,
        const std::vector<Cargo>& cargos);

 
    static void displayGroupingByArrival(std::ostream& os,
        const std::unordered_map<int, std::vector<int>>& groups,
        const std::vector<freight>& freights,
        const std::vector<Cargo>& cargos);
};
