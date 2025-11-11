#pragma once

#include <vector>
#include <string>
#include "Shipment.h"

// does input and output for schedules
class ScheduleRepository {
public:
    static bool saveToFile(const std::vector<Shipment>& plans, const std::string& filename);
};
