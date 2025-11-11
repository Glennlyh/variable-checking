#pragma once

#include <vector>
#include <unordered_map>
#include "Shipment.h"
#include "freight.h"
#include "Cargo.h"

// handles analysis operations on schedules
class ScheduleAnalyzer 
{
public:
    static std::vector<int> unmatchedFreightIds(const std::vector<Shipment>& plans, const std::vector<freight>& freights);
    static std::vector<int> unmatchedCargoIds(const std::vector<Shipment>& plans, const std::vector<Cargo>& cargos);
    static std::vector<int> freightsWithSpaceLeft(const std::unordered_map<int, std::vector<int>>& groups, const std::vector<freight>& freights, const std::vector<Cargo>& cargos);
    static std::vector<int> unassignedCargoIdsFromGroups(const std::unordered_map<int, std::vector<int>>& groups, const std::vector<Cargo>& cargos);
};
