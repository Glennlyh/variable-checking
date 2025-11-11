#include "ScheduleAnalyzer.h"
#include <algorithm>
#include <unordered_set>

using namespace std;

vector<int> ScheduleAnalyzer::unmatchedFreightIds(const vector<Shipment>& plans, const vector<freight>& freights)
{
    vector<int> ids;
    for (const auto& f : freights) {
        bool used = false;
        for (const auto& m : plans) { 
            if (f.getIndex() == m.getFreightIndex()) { 
                used = true; 
                break; 
            } 
        }
        if (!used) ids.push_back(f.getIndex());
    }
    return ids;
}

vector<int> ScheduleAnalyzer::unmatchedCargoIds(const vector<Shipment>& plans, const vector<Cargo>& cargos)
{
    vector<int> ids;
    for (const auto& c : cargos) {
        bool used = false;
        for (const auto& m : plans) { 
            if (c.getIndex() == m.getCargoIndex()) { 
                used = true; 
                break; 
            } 
        }
        if (!used) ids.push_back(c.getIndex());
    }
    return ids;
}

vector<int> ScheduleAnalyzer::freightsWithSpaceLeft(const unordered_map<int, vector<int>>& groups, const vector<freight>& freights, const vector<Cargo>& cargos)
{
    vector<int> res;
    // build map cargoIndex -> Cargo for quick lookup
    unordered_map<int, const Cargo*> cargoMap;
    for (const auto& c : cargos) cargoMap[c.getIndex()] = &c;

    for (const auto& f : freights) {
        int limit = f.getCapacity();
        auto it = groups.find(f.getIndex());
        if (it == groups.end()) {
            // full capacity unused => entire freight capacity left
            res.push_back(f.getIndex());
            continue;
        }
        // compute used capacity as sum of assigned cargo capacities
        int usedCap = 0;
        for (int cid : it->second) {
            auto cit = cargoMap.find(cid);
            if (cit != cargoMap.end()) usedCap += cit->second->getCapacity();
            else usedCap += 1; // fallback if cargo not found
        }
        if (usedCap < limit) res.push_back(f.getIndex());
    }
    return res;
}

vector<int> ScheduleAnalyzer::unassignedCargoIdsFromGroups(const unordered_map<int, vector<int>>& groups, const vector<Cargo>& cargos)
{
    unordered_set<int> assigned;
    for (const auto& p : groups) for (int cid : p.second) assigned.insert(cid);
    vector<int> res;
    for (const auto& c : cargos) if (!assigned.count(c.getIndex())) res.push_back(c.getIndex());
    return res;
}
