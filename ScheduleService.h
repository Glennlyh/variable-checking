#pragma once
#include <vector>
#include "Schduler.h"
#include "MatchingEngine.h"
#include "freight.h"
#include "Cargo.h"

class ScheduleService {
    Schduler scheduler_;
    MatchStrategy& strategy_;
public:
    explicit ScheduleService(MatchStrategy& s) : strategy_(s) {}
    void runMatch(const std::vector<freight>& F, const std::vector<Cargo>& C);
    std::vector<Shipment> plans() const { return scheduler_.getPlans(); }
    Schduler& scheduler() { return scheduler_; }
    const Schduler& scheduler() const { return scheduler_; }
};
