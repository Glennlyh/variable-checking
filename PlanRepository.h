#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "Shipment.h"
#include "freight.h"
#include "Cargo.h"

using namespace std;

struct PlanRepository {
    static bool saveCSV(const vector<Shipment>& plans, const string& filepath);

    // save grouping based schedule
    static bool saveGrouping(const std::unordered_map<int, vector<int>>& groups,
        const vector<freight>& freights,
        const vector<Cargo>& cargos,
        const string& filename);
};
