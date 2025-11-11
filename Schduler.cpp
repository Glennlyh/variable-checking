#include "Schduler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <iomanip>
#include <climits>
#include "functions.h"

Schduler::Schduler() {}

Shipment Schduler::mergePlans(const freight& f, const Cargo& c)
{
    return Shipment(f, c);
}

size_t Schduler::matchAllPlans(const std::vector<freight>& freights, const std::vector<Cargo>& cargos)
{
    // clear previous plans to avoid duplicates when matching multiple times
    plans.clear();

    size_t matches = 0;
    for (const auto& f : freights) 
    {
        for (const auto& c : cargos)
        {
            Shipment plan(f, c);
            if (plan.IsMatching()) { plans.push_back(plan); ++matches; }
        }
    }
    return matches;
}

std::vector<Shipment> Schduler::getPlans() const 
{
    return plans;
}

void Schduler::setPlans(std::vector<Shipment> newPlans)
{
    plans = std::move(newPlans);
}
