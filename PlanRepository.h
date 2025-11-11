#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "Shipment.h"
#include "freight.h"
#include "Cargo.h"

struct PlanRepository {
    static bool saveCSV(const std::vector<Shipment>& plans, const std::string& filepath);

    // save grouping based schedule
    static bool saveGrouping(const std::unordered_map<int, std::vector<int>>& groups,
        const std::vector<freight>& freights,
        const std::vector<Cargo>& cargos,
        const std::string& filename);
};
