#include "ScheduleRepository.h"
#include <fstream>

bool ScheduleRepository::saveToFile(const std::vector<Shipment>& plans, const std::string& filename)
{
    std::ofstream out(filename);
    if (!out) return false;
    int i = 1;
    for (const auto& shipment : plans) {
        out << "Plan " << i++ << " Shipment: F" << shipment.getFreightIndex()
            << " " << shipment.getFreightDes()
            << " " << shipment.getFreightTime()
            << " C" << shipment.getCargoIndex()
            << " " << shipment.getCargoDes()
            << " " << shipment.getCargoTime() << '\n';
    }
    return true;
}
