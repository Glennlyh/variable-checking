#pragma once

#include <vector>
#include <unordered_map>
#include "Shipment.h"
#include "freight.h"
#include "Cargo.h"

using namespace std;

// handles analysis operations on schedules
class ScheduleAnalyzer 
{
public:
    static vector<int> unmatchedFreightIds(const vector<Shipment>& plans, const vector<freight>& freights);
    static vector<int> unmatchedCargoIds(const  vector<Shipment>& plans, const vector<Cargo>& cargos);
    static vector<int> freightsWithSpaceLeft(const  unordered_map<int, vector<int>>& groups, const  vector<freight>& freights, const  vector<Cargo>& cargos);
    static vector<int> unassignedCargoIdsFromGroups(const  unordered_map<int,  vector<int>>& groups, const  vector<Cargo>& cargos);
};
