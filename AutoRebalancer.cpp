#include "AutoRebalancer.h"
#include "functions.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

AutoRebalancer::AutoRebalancer(freightlist& fList, CargoList& cList)
    : freightList_(fList), cargoList_(cList), hasBackup_(false)
{
}

void AutoRebalancer::onFreightRemoved(const freight& f) 
{
    cout << "\n=== AutoRebalancer: Freight F" << f.getIndex() << " removed ===\n";
    cout << "Checking for affected cargo items...\n";
}

vector<int> AutoRebalancer::findOrphanedCargo(
    int freightId, 
    const unordered_map<int, vector<int>>& currentGrouping)
{
    vector<int> orphaned;
    
    auto it = currentGrouping.find(freightId);
    if (it != currentGrouping.end()) 
    {
        orphaned = it->second;
    }
    
    return orphaned;
}

unordered_map<int, vector<int>> AutoRebalancer::rebalanceCargo(
    const vector<int>& orphanedCargoIds,
    const unordered_map<int, vector<int>>& currentGrouping)
{
    // Create a copy of current grouping without the removed freight
    unordered_map<int, vector<int>> newGrouping = currentGrouping;
    
    // Get all available freights
    const auto& allFreights = freightList_.all();
    const auto& allCargos = cargoList_.all();
    
    if (orphanedCargoIds.empty()) 
    {
        cout << "No orphaned cargo found.\n";
        return newGrouping;
    }
    
    cout << "\nAttempting to reassign " << orphanedCargoIds.size() << " orphaned cargo item(s)...\n";
    
    // Try to reassign each orphaned cargo
    for (int cargoId : orphanedCargoIds) 
    {
        auto cargo = cargoList_.findById(cargoId);
        if (!cargo) continue;
        
        bool assigned = false;
        
        // Try to find a freight with matching destination and available capacity
        for (const auto& f : allFreights) 
        {
            // Check if destination matches
            if (f.getDestination() != cargo->getDestination()) 
                continue;
            
            // Check if time is valid (freight departs after cargo arrives)
            if (f.getTime() < cargo->getTime())
                continue;
            
            // Calculate current capacity usage for this freight
            int usedCapacity = 0;
            auto it = newGrouping.find(f.getIndex());
            if (it != newGrouping.end()) 
            {
                for (int cid : it->second) 
                {
                    auto c = cargoList_.findById(cid);
                    if (c) usedCapacity += c->getCapacity();
                }
            }
            
            // Check if there's enough space
            if (usedCapacity + cargo->getCapacity() <= f.getCapacity()) 
            {
                newGrouping[f.getIndex()].push_back(cargoId);
                assigned = true;
                cout << "  Reassigned C" << cargoId << " to F" << f.getIndex() << "\n";
                break;
            }
        }
        
        if (!assigned) 
        {
            cout << "  WARNING: Could not reassign C" << cargoId 
                 << " (no compatible freight with available capacity)\n";
        }
    }
    
    return newGrouping;
}

void AutoRebalancer::showRebalancingResults(
    const vector<int>& orphanedCargoIds,
    const unordered_map<int, vector<int>>& oldGrouping,
    const unordered_map<int, vector<int>>& newGrouping)
{
    cout << "\n=== Rebalancing Results ===\n";
    cout << "Total orphaned cargo items: " << orphanedCargoIds.size() << "\n\n";
    
    if (orphanedCargoIds.empty()) 
    {
        cout << "No cargo was affected by the freight removal.\n";
        return;
    }
    
    cout << "Cargo reassignments:\n";
    cout << setw(10) << "Cargo ID" 
         << setw(15) << "Destination" 
         << setw(15) << "Old Freight" 
         << setw(15) << "New Freight" << "\n";
    cout << string(55, '-') << "\n";
    
    for (int cargoId : orphanedCargoIds) 
    {
        auto cargo = cargoList_.findById(cargoId);
        if (!cargo) continue;
        
        // Find old freight assignment
        int oldFreightId = -1;
        for (const auto& pair : oldGrouping) 
        {
            if (find(pair.second.begin(), pair.second.end(), cargoId) != pair.second.end()) 
            {
                oldFreightId = pair.first;
                break;
            }
        }
        
        // Find new freight assignment
        int newFreightId = -1;
        for (const auto& pair : newGrouping) 
        {
            if (find(pair.second.begin(), pair.second.end(), cargoId) != pair.second.end()) 
            {
                newFreightId = pair.first;
                break;
            }
        }
        
        cout << setw(10) << ("C" + to_string(cargoId))
             << setw(15) << cargo->getDestination()
             << setw(15) << (oldFreightId != -1 ? "F" + to_string(oldFreightId) : "N/A")
             << setw(15) << (newFreightId != -1 ? "F" + to_string(newFreightId) : "Unassigned") << "\n";
    }
    
    cout << "\n";
}
