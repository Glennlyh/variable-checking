#include "GroupingStrategy.h"
#include <algorithm>
#include <climits>

static int toMinutes(int hhmm) {
    return (hhmm / 100) * 60 + (hhmm % 100);
}

 unordered_map<int,  vector<int>> PerfectArrivalGrouping::group(const  vector<freight>& freights, const  vector<Cargo>& cargos) const
{
     unordered_map<int,  vector<int>> groups;

    // sort freights by time (earliest first)
     vector<const freight*> sortedFreights;
    for (const auto& f : freights) sortedFreights.push_back(&f);
    sort(sortedFreights.begin(), sortedFreights.end(),
        [](const freight* a, const freight* b)
        {
            return a->getTime() < b->getTime();
        });

    // sort cargos by time (earliest first)
     vector<const Cargo*> sortedCargos;
    for (const auto& c : cargos) sortedCargos.push_back(&c);
    sort(sortedCargos.begin(), sortedCargos.end(),
        [](const Cargo* a, const Cargo* b)
        {
            return a->getTime() < b->getTime();
        });

    // for each cargo, assign it to all freights with matching des
    // the conversion function will handle splitting based on capacity
    for (const auto* pc : sortedCargos) 
    
    {
        int cid = pc->getIndex();

        // add this cargo to all freights with same des
        for (const auto* pf : sortedFreights) 
        {
            if (pf->getDestination() == pc->getDestination()) 
            {
                groups[pf->getIndex()].push_back(cid);
            }
        }
    }

    return groups;
}

 unordered_map<int,  vector<int>> LeastFreightsGrouping::group(const  vector<freight>& freights, const  vector<Cargo>& cargos) const
{
     unordered_map<int,  vector<int>> groups;

    // sort freights by capacity (largest first) , then by time
     vector<const freight*> sortedFreights;
    for (const auto& f : freights) sortedFreights.push_back(&f);
    sort(sortedFreights.begin(), sortedFreights.end(),
        [](const freight* a, const freight* b) {
            if (a->getCapacity() != b->getCapacity())
                return a->getCapacity() > b->getCapacity(); // Largest first
            return a->getTime() < b->getTime();
        });

    // group cargos by des
     unordered_map< string,  vector<const Cargo*>> cargosByDestination;
    for (const auto& c : cargos) 
    {
        cargosByDestination[c.getDestination()].push_back(&c);
    }

    // sort cargos within each des by capacity (largest first for better packing)
    for (auto& pair : cargosByDestination) 
    {
        sort(pair.second.begin(), pair.second.end(),
            [](const Cargo* a, const Cargo* b) {
                return a->getCapacity() > b->getCapacity();
            });
    }

    // for each des, pack cargos into freights efficiently
    for (auto& destPair : cargosByDestination) {
        const  string& destination = destPair.first;
        const  vector<const Cargo*>& destCargos = destPair.second;

        // get all freights going to this des, sorted by capacity (largest first)
         vector<const freight*> destFreights;
        for (const auto* pf : sortedFreights) {
            if (pf->getDestination() == destination) {
                destFreights.push_back(pf);
            }
        }

        if (destFreights.empty()) continue;

        // fill each freight to capacity before moving to next
        size_t currentFreightIndex = 0;

        for (const auto* pc : destCargos) {
            int cid = pc->getIndex();

            // add this cargo to all available freights for this des
            // the splitting will be handled by convertGroupsToShipments
            for (const auto* pf : destFreights) {
                groups[pf->getIndex()].push_back(cid);
            }
        }
    }

    return groups;
}
