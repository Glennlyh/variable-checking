#include "PlanExporter.h"
#include "functions.h"

namespace PlanExporter 
{
    void toCSV(const std::vector<Shipment>& plans, std::ostream& os) 
    {
        os << "ScheduleNo,Freight,Cargo,FreightETA,CargoETA,Destination\n";
        for (const auto& s : plans) {
            os << s.getScheduleNo() << ","
               << s.getFreightName() << ","
               << s.getCargoName() << ","
               << formatTime12h(s.getFreightTime()) << ","
               << formatTime12h(s.getCargoTime()) << ","
               << s.getFreightDes() << "\n";
        }
    }
}
