#pragma once   

#include <vector>
#include <ostream>
#include <unordered_map>
#include "Shipment.h"
#include "freight.h"
#include "Cargo.h"
#include <iomanip>

class Schduler
{
    std::vector<Shipment> plans;
public:
    Schduler();

    Shipment mergePlans(const freight& f, const Cargo& c);
    size_t matchAllPlans(const std::vector<freight>& freights, const std::vector<Cargo>& cargos);

    std::vector<Shipment> getPlans() const;                                                          // returns a copy (renamed from plans())

    void setPlans(std::vector<Shipment> newPlans);
};

