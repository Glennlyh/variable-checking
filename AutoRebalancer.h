#pragma once

#include "FreightObserver.h"
#include "freightlist.h"
#include "CargoList.h"
#include "GroupingStrategy.h"
#include <vector>
#include <unordered_map>
#include <string>

// AutoRebalancer implements the observer pattern to automatically
// reassign cargo when a freight is removed
class AutoRebalancer : public FreightObserver 
{
private:
    freightlist& freightList_;
    CargoList& cargoList_;
    
    // Store backup state for undo functionality
    struct BackupState 
    {
        freight removedFreight;
        std::unordered_map<int, std::vector<int>> oldGrouping;
        std::unordered_map<int, std::vector<int>> newGrouping;
        std::vector<int> affectedCargoIds;
    };
    
    BackupState lastBackup_;
    bool hasBackup_;

public:
    AutoRebalancer(freightlist& fList, CargoList& cList);
    
    // Observer interface implementation
    void onFreightRemoved(const freight& f) override;
    
    // Find cargo items that were assigned to the removed freight
    std::vector<int> findOrphanedCargo(int freightId, 
                                        const std::unordered_map<int, std::vector<int>>& currentGrouping);
    
    // Trigger automatic rescheduling for orphaned cargo
    std::unordered_map<int, std::vector<int>> rebalanceCargo(
        const std::vector<int>& orphanedCargoIds,
        const std::unordered_map<int, std::vector<int>>& currentGrouping);
    
    // Display before/after comparison
    void showRebalancingResults(const std::vector<int>& orphanedCargoIds,
                                 const std::unordered_map<int, std::vector<int>>& oldGrouping,
                                 const std::unordered_map<int, std::vector<int>>& newGrouping);
    
    // Access backup state
    bool hasBackup() const { return hasBackup_; }
    const BackupState& getBackup() const { return lastBackup_; }
};
