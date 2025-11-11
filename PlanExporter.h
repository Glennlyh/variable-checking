#pragma once
#include <vector>
#include <ostream>
#include "Shipment.h"

namespace PlanExporter 
{
    void toCSV(const std::vector<Shipment>& plans, std::ostream& os);
}
