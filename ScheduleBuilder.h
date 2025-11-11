#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include "Shipment.h"
#include "freight.h"
#include "Cargo.h"
using namespace std;

// convert grouping results (map freight to cargo ids)
// into concrete shipment objects using different strategies

class ScheduleBuilder {
public:
    static vector<Shipment> buildFromArrival(
        const unordered_map<int, vector<int>>& groups,
        const vector<freight>& freights,
        const vector<Cargo>& cargos);

    static vector<Shipment> buildFromLeastFreights(
        const unordered_map<int, vector<int>>& groups,
        const vector<freight>& freights,
        const vector<Cargo>& cargos);
};
