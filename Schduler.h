#pragma once   

#include <vector>
#include <ostream>
#include <unordered_map>
#include "Shipment.h"
#include "freight.h"
#include "Cargo.h"
#include <iomanip>

using namespace std;

class Schduler
{
    vector<Shipment> plans;
public:
    Schduler();

    Shipment mergePlans(const freight& f, const Cargo& c);
    size_t matchAllPlans(const vector<freight>& freights, const vector<Cargo>& cargos);

    vector<Shipment> getPlans() const;                                                          // returns a copy (renamed from plans())

    void setPlans(vector<Shipment> newPlans);
};

