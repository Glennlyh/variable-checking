#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include "Shipment.h"
#include "freight.h"
#include "Cargo.h"

// convert grouping results (map freight to cargo ids)
// into concrete shipment objects using different strategies

class ScheduleBuilder {
public:
    static std::vector<Shipment> buildFromArrival(
        const std::unordered_map<int, std::vector<int>>& groups,
        const std::vector<freight>& freights,
        const std::vector<Cargo>& cargos);

    static std::vector<Shipment> buildFromLeastFreights(
        const std::unordered_map<int, std::vector<int>>& groups,
        const std::vector<freight>& freights,
        const std::vector<Cargo>& cargos);
};
