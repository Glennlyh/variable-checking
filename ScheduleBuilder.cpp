#include "ScheduleBuilder.h"
#include <algorithm>
#include <unordered_map>

using namespace std;

vector<Shipment> ScheduleBuilder::buildFromArrival(
    const unordered_map<int, vector<int>>& groups,
    const vector<freight>& freights,
    const vector<Cargo>& cargos)
{
    vector<Shipment> shipments;

    unordered_map<int, const freight*> freightMap;
    for (const auto& f : freights) freightMap[f.getIndex()] = &f;

    unordered_map<int, const Cargo*> cargoMap;
    for (const auto& c : cargos) cargoMap[c.getIndex()] = &c;

    unordered_map<int, int> cargoRemaining;
    for (const auto& c : cargos) cargoRemaining[c.getIndex()] = c.getCapacity();

    vector<int> sortedFreightIds;
    for (const auto& groupPair : groups) sortedFreightIds.push_back(groupPair.first);
    sort(sortedFreightIds.begin(), sortedFreightIds.end());

    int scheduleNo = 1;

    for (int freightId : sortedFreightIds) 
    {
        auto fit = freightMap.find(freightId);
        if (fit == freightMap.end()) continue;
        const freight* f = fit->second;
        int freightCapacityLeft = f->getCapacity();

        auto git = groups.find(freightId);
        if (git == groups.end()) continue;
        const vector<int>& cargoIds = git->second;

        for (size_t i = 0; i < cargoIds.size(); ++i)
        {
            int cargoId = cargoIds[i];
            auto cit = cargoMap.find(cargoId);
            if (cit == cargoMap.end()) continue;

            const Cargo* c = cit->second;
            int cargoNeeded = cargoRemaining[cargoId];
            if (cargoNeeded <= 0) continue;

            int quantityToAssign = min(freightCapacityLeft, cargoNeeded);
            if (quantityToAssign > 0) 
            {
                Shipment s(*f, *c, scheduleNo, quantityToAssign);
                shipments.push_back(s);
                freightCapacityLeft -= quantityToAssign;
                cargoRemaining[cargoId] -= quantityToAssign;
            }
            if (freightCapacityLeft <= 0) break;
        }

        scheduleNo++;
    }

    return shipments;
}

vector<Shipment> ScheduleBuilder::buildFromLeastFreights(
    const unordered_map<int, vector<int>>& groups,
    const vector<freight>& freights,
    const vector<Cargo>& cargos)
{
    vector<Shipment> shipments;

    unordered_map<int, const freight*> freightMap;
    for (const auto& f : freights) freightMap[f.getIndex()] = &f;

    unordered_map<int, const Cargo*> cargoMap;
    for (const auto& c : cargos) cargoMap[c.getIndex()] = &c;

    unordered_map<string, vector<const freight*>> freightsByDest;
    for (const auto& f : freights) freightsByDest[f.getDestination()].push_back(&f);

    for (auto& pair : freightsByDest)
    {
        sort(pair.second.begin(), pair.second.end(),
            [](const freight* a, const freight* b) {
                if (a->getCapacity() != b->getCapacity())
                    return a->getCapacity() > b->getCapacity();
                return a->getTime() < b->getTime();
            });
    }

    unordered_map<string, vector<const Cargo*>> cargosByDest;
    for (const auto& c : cargos) cargosByDest[c.getDestination()].push_back(&c);

    for (auto& pair : cargosByDest) 
    {
        sort(pair.second.begin(), pair.second.end(),
            [](const Cargo* a, const Cargo* b) {
                return a->getCapacity() > b->getCapacity();
            });
    }

    int scheduleNo = 1;

    for (auto& destPair : cargosByDest) 
    {
        const string& destination = destPair.first;
        const vector<const Cargo*>& destCargos = destPair.second;

        auto freightIt = freightsByDest.find(destination);
        if (freightIt == freightsByDest.end()) continue;

        const vector<const freight*>& destFreights = freightIt->second;
        if (destFreights.empty()) continue;

        unordered_map<int, int> cargoRemaining;
        for (const auto* c : destCargos) cargoRemaining[c->getIndex()] = c->getCapacity();

        for (const auto* f : destFreights) 
        {
            int freightCapacityLeft = f->getCapacity();
            bool freightUsed = false;

            for (const auto* c : destCargos) 
            {
                int cargoNeeded = cargoRemaining[c->getIndex()];
                if (cargoNeeded <= 0) continue;

                int quantityToAssign = min(freightCapacityLeft, cargoNeeded);
                if (quantityToAssign > 0) 
                {
                    Shipment s(*f, *c, scheduleNo, quantityToAssign);
                    shipments.push_back(s);
                    freightCapacityLeft -= quantityToAssign;
                    cargoRemaining[c->getIndex()] -= quantityToAssign;
                    freightUsed = true;
                }
                if (freightCapacityLeft <= 0) break;
            }

            if (freightUsed) 
            {
                scheduleNo++;
            }

            bool allAssigned = true;
            for (const auto* c : destCargos) 
            {
                if (cargoRemaining[c->getIndex()] > 0) { allAssigned = false; break; }
            }

            if (allAssigned) break;
        }
    }

    return shipments;
}
