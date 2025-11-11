#include "SchedulePresenter.h"
#include "functions.h"
#include <algorithm>
#include <iomanip>
#include <map>
#include <vector>
#include <unordered_map>
#include <unordered_set>

static bool cmpByCargoArrival(const Shipment& a, const Shipment& b)
{
    return a.getCargoTime() < b.getCargoTime();
}

void SchedulePresenter::tableByCargoArrival(const vector<Shipment>& plans, ostream& os)
{
    vector<Shipment> sorted = plans;
    sort(sorted.begin(), sorted.end(), cmpByCargoArrival);

    os << "\n=== Schedule (by Cargo Arrival Time) ===\n";
    os << left
        << setw(10) << "Sched#"
        << setw(10) << "Freight"
        << setw(10) << "Cargo"
        << setw(16) << "Freight ETA"
        << setw(16) << "Cargo ETA"
        << "Destination\n";

    for (const auto& s : sorted) 
    {
        os << left
            << setw(10) << s.getScheduleNo()
            << setw(10) << s.getFreightName()
            << setw(10) << s.getCargoName()
            << setw(16) << formatTime12h(s.getFreightTime())
            << setw(16) << formatTime12h(s.getCargoTime())
            << s.getFreightDes() << "\n";
    }
}

void SchedulePresenter::groupingByArrival(const vector<Shipment>& plans,
    const vector<freight>& freights,
    const vector<Cargo>& cargos,
    ostream& os,
    unordered_map<int, vector<int>>& outGroups)
{
    outGroups.clear();

    map<int, vector<const Shipment*>> buckets;
    for (const auto& s : plans) 
    {
        buckets[s.getFreightIndex()].push_back(&s);
    }
    for (auto& kv : buckets)
    {
        auto& vec = kv.second;
        sort(vec.begin(), vec.end(),
            [](const Shipment* a, const Shipment* b) { return a->getCargoTime() < b->getCargoTime(); });
        auto& dst = outGroups[kv.first];
        dst.reserve(vec.size());
        for (auto* p : vec) dst.push_back(p->getCargoIndex());
    }

    os << "\n=== Grouping by Freight (Cargo arrival order) ===\n";
    for (const auto& f : freights) 
    {
        int fidx = f.getIndex();
        auto it = outGroups.find(fidx);
        if (it == outGroups.end() || it->second.empty()) continue;

        os << "F" << fidx << " (" << f.getDestination()
            << ", " << formatTime12h(f.getTime()) << "): ";

        bool first = true;
        for (int cidx : it->second)
        {
            if (!first) os << ", ";
            first = false;
            auto cIt = find_if(cargos.begin(), cargos.end(),
                [cidx](const Cargo& c) { return c.getIndex() == cidx; });
            if (cIt != cargos.end())
            {
                os << "C" << cidx << "@" << formatTime12h(cIt->getTime());
            }
            else 
            {
                os << "C" << cidx;
            }
        }
        os << "\n";
    }
}

void SchedulePresenter::displayScheduleByCargoArrivalTime(const vector<Shipment>& plans, ostream& os)
{
    if (plans.empty()) {
        os << "No schedules found.\n";
        return;
    }

    using std::left;
    using std::setw;

    os << left
        << setw(8) << "Sched#"
        << setw(8) << "Freight"
        << setw(16) << "Destination"
        << setw(12) << "Fr Time"
        << setw(14) << "Type"
        << setw(8) << "OrigCap"
        << setw(10) << "Outstanding"
        << setw(8) << "Cargo"
        << setw(16) << "Cargo Dest"
        << setw(12) << "Cargo Time"
        << setw(10) << "Cargo/Grp"
        << setw(12) << "Unfulfilled"
        << '\n';

    int scheduleNo = 1;

    // track cumulative capacity usage per freight
    unordered_map<int, int> freightUsedCapacity;

    for (const auto& s : plans) {
        const freight& f = s.getFreight();
        const Cargo& c = s.getCargo();

        int originalCap = f.getCapacity();

        // add current cargo QUANTITY to this freight's cumulative usage
        int cargoQty = s.getCargoQuantity() > 0 ? s.getCargoQuantity() : c.getCapacity();
        freightUsedCapacity[f.getIndex()] += cargoQty;

        // calculate outstanding as original 
        int outstanding = max(0, originalCap - freightUsedCapacity[f.getIndex()]);

        int groupCargo = count_if(plans.begin(), plans.end(),
            [&](const Shipment& sp) { return sp.getCargoDes() == c.getDestination(); });
        int unfulfilledCargo = max(0, 10 - groupCargo);

        os << left
            << setw(8) << scheduleNo++
            << setw(8) << s.getFreightName()
            << setw(16) << f.getDestination()
            << setw(12) << formatTime12h(f.getTime())
            << setw(14) << f.getTypeName()
            << setw(8) << originalCap
            << setw(10) << outstanding
            << setw(8) << s.getCargoName()
            << setw(16) << c.getDestination()
            << setw(12) << formatTime12h(c.getTime())
            << setw(10) << groupCargo
            << setw(12) << unfulfilledCargo
            << '\n';
    }
}

void SchedulePresenter::displayGroupingByArrival(ostream& os, const unordered_map<int, vector<int>>& groups, const vector<freight>& freights, const vector<Cargo>& cargos)
{
    //non overloaded ver of displayGroupingByArrival
    os << "\nERROR: displayGroupingByArrival needs to be called with Shipments for cargo splitting support\n";
}

// overloaded version that accepts shipments
void SchedulePresenter::displayGroupingByArrival(ostream& os, const vector<Shipment>& plans, const vector<freight>& freights, const vector<Cargo>& cargos)
{
    using std::left;
    using std::setw;
    using std::string;

    // build lookup maps
    unordered_map<int, const freight*> fmap;
    for (const auto& f : freights) fmap[f.getIndex()] = &f;
    unordered_map<int, const Cargo*> cmap;
    for (const auto& c : cargos) cmap[c.getIndex()] = &c;

    // group shipments by freight ID
    map<int, vector<const Shipment*>> freightGroups;
    for (const auto& s : plans) {
        freightGroups[s.getFreightIndex()].push_back(&s);
    }

    // sort freight IDs in ASCENDING order
    vector<int> sortedFreightIds;
    for (const auto& p : freightGroups) {
        sortedFreightIds.push_back(p.first);
    }
    sort(sortedFreightIds.begin(), sortedFreightIds.end());

    // Column widths
    const int w1 = 12;  // schedule number
    const int w2 = 15;  // freight name
    const int w3 = 18;  // freight destination
    const int w4 = 18;  // freight refuel time
    const int w5 = 18;  // freight capacity name
    const int w6 = 18;  // freight: original capacity values
    const int w7 = 20;  // freight: outstanding capacity 
    const int w8 = 12;  // cargo name
    const int w9 = 18;  // cargo destination
    const int w10 = 18; // cargo time to reach
    const int w11 = 18; // cargo original number of cargo per grp
    const int w12 = 18; // cargo outstanding number of cargo

    // title
    os << "\nScheduling: By Cargo Arrival Time\n\n";

    // top border
    os << "+" << string(w1, '-') << "+" << string(w2, '-') << "+" << string(w3, '-') << "+"
        << string(w4, '-') << "+" << string(w5, '-') << "+" << string(w6, '-') << "+"
        << string(w7, '-') << "+" << string(w8, '-') << "+" << string(w9, '-') << "+"
        << string(w10, '-') << "+" << string(w11, '-') << "+" << string(w12, '-') << "+\n";

    // header rows
    os << "|" << left << setw(w1) << " Schedule"
        << "|" << setw(w2) << " Freight"
        << "|" << setw(w3) << " Freight"
        << "|" << setw(w4) << " Freight"
        << "|" << setw(w5) << " Freight"
        << "|" << setw(w6) << " Freight:"
        << "|" << setw(w7) << " Freight:"
        << "|" << setw(w8) << " Cargo"
        << "|" << setw(w9) << " Cargo"
        << "|" << setw(w10) << " Cargo"
        << "|" << setw(w11) << " Cargo:"
        << "|" << setw(w12) << " Cargo:"
        << "|\n";

    os << "|" << left << setw(w1) << " #"
        << "|" << setw(w2) << " Name"
        << "|" << setw(w3) << " Destination"
        << "|" << setw(w4) << " Refuel"
        << "|" << setw(w5) << " Capacity"
        << "|" << setw(w6) << " Original"
        << "|" << setw(w7) << " Outstanding"
        << "|" << setw(w8) << " Name"
        << "|" << setw(w9) << " Destination"
        << "|" << setw(w10) << " Time"
        << "|" << setw(w11) << " Original"
        << "|" << setw(w12) << " Outstanding"
        << "|\n";

    os << "|" << left << setw(w1) << " "
        << "|" << setw(w2) << " "
        << "|" << setw(w3) << " "
        << "|" << setw(w4) << " Time"
        << "|" << setw(w5) << " Name"
        << "|" << setw(w6) << " Capacity"
        << "|" << setw(w7) << " Capacity or"
        << "|" << setw(w8) << " "
        << "|" << setw(w9) << " "
        << "|" << setw(w10) << " to"
        << "|" << setw(w11) << " Number"
        << "|" << setw(w12) << " number of"
        << "|\n";

    os << "|" << left << setw(w1) << " "
        << "|" << setw(w2) << " "
        << "|" << setw(w3) << " "
        << "|" << setw(w4) << " "
        << "|" << setw(w5) << " "
        << "|" << setw(w6) << " Values"
        << "|" << setw(w7) << " Unfulfilled"
        << "|" << setw(w8) << " "
        << "|" << setw(w9) << " "
        << "|" << setw(w10) << " Reach"
        << "|" << setw(w11) << " of Cargo per"
        << "|" << setw(w12) << " Cargo"
        << "|\n";

    os << "|" << left << setw(w1) << " "
        << "|" << setw(w2) << " "
        << "|" << setw(w3) << " "
        << "|" << setw(w4) << " "
        << "|" << setw(w5) << " "
        << "|" << setw(w6) << " "
        << "|" << setw(w7) << " Capacity"
        << "|" << setw(w8) << " "
        << "|" << setw(w9) << " "
        << "|" << setw(w10) << " Destination"
        << "|" << setw(w11) << " Group"
        << "|" << setw(w12) << " without freight"
        << "|\n";

    // header bottom border
    os << "+" << string(w1, '-') << "+" << string(w2, '-') << "+" << string(w3, '-') << "+"
        << string(w4, '-') << "+" << string(w5, '-') << "+" << string(w6, '-') << "+"
        << string(w7, '-') << "+" << string(w8, '-') << "+" << string(w9, '-') << "+"
        << string(w10, '-') << "+" << string(w11, '-') << "+" << string(w12, '-') << "+\n";

    int scheduleNo = 1;

    // track assigned quantity per cargo as we process each freight in order
    unordered_map<int, int> assignedSoFar;

    // iterate through sorted freight IDs
    for (int fid : sortedFreightIds) {
        const freight* f = nullptr;
        auto fit = fmap.find(fid);
        if (fit != fmap.end()) f = fit->second;

        const auto& shipments = freightGroups[fid];
        if (shipments.empty()) continue;

        int origCap = f ? f->getCapacity() : 0;

        // calculate "Cargo: Original Number of Cargo per Group"
        // show the original total quantity of the FIRST cargo in this freight
        int groupCargoCount = 0;
        if (!shipments.empty()) {
            const Shipment* firstShipment = shipments[0];
            int firstCargoId = firstShipment->getCargoIndex();
            auto cit = cmap.find(firstCargoId);
            if (cit != cmap.end()) {
                groupCargoCount = cit->second->getCapacity();
            }
        }

        // track cumulative capacity used as we display each cargo
        int cumulativeUsedCap = 0;
        bool first = true;

        for (const auto* s : shipments) {
            const Cargo* c = nullptr;
            auto cit = cmap.find(s->getCargoIndex());
            if (cit != cmap.end()) c = cit->second;

            // add current cargo's quantity to cumulative total for this freight
            int cargoQty = s->getCargoQuantity() > 0 ? s->getCargoQuantity() : (c ? c->getCapacity() : 1);
            cumulativeUsedCap += cargoQty;

            // track assignment across all freights processed so far
            assignedSoFar[s->getCargoIndex()] += cargoQty;

            // calculate outstanding freight capacity after this cargo is assigned
            int outstanding = max(0, origCap - cumulativeUsedCap);

            // calculate "Cargo: Outstanding number of Cargo without freight"
            // show remaining quantity of the first cargo AFTER THIS FREIGHT
            int unfulfilled = 0;
            if (first && !shipments.empty()) {
                const Shipment* firstShipment = shipments[0];
                int firstCargoId = firstShipment->getCargoIndex();
                auto ccit = cmap.find(firstCargoId);
                if (ccit != cmap.end()) {
                    int totalCapacity = ccit->second->getCapacity();
                    int assigned = assignedSoFar[firstCargoId];
                    unfulfilled = max(0, totalCapacity - assigned);
                }
            }

            if (first) {
                os << "|" << left << setw(w1) << (" " + to_string(scheduleNo++))
                    << "|" << setw(w2) << (f ? (" F" + to_string(f->getIndex())) : " ")
                    << "|" << setw(w3) << (f ? (" " + f->getDestination()) : " ")
                    << "|" << setw(w4) << (f ? (" " + formatTime12h(f->getTime())) : " ")
                    << "|" << setw(w5) << (f ? (" " + f->getTypeName()) : " ")
                    << "|" << setw(w6) << (" " + to_string(origCap))
                    << "|" << setw(w7) << (" " + to_string(outstanding))
                    << "|" << setw(w8) << (c ? (" C" + to_string(c->getIndex())) : " -")
                    << "|" << setw(w9) << (c ? (" " + c->getDestination()) : " ")
                    << "|" << setw(w10) << (c ? (" " + formatTime12h(c->getTime())) : " ")
                    << "|" << setw(w11) << (" " + to_string(groupCargoCount))
                    << "|" << setw(w12) << (" " + to_string(unfulfilled))
                    << "|\n";
                first = false;
            }
            else {
                os << "|" << left << setw(w1) << " "
                    << "|" << setw(w2) << " "
                    << "|" << setw(w3) << " "
                    << "|" << setw(w4) << " "
                    << "|" << setw(w5) << " "
                    << "|" << setw(w6) << " "
                    << "|" << setw(w7) << (" " + to_string(outstanding))
                    << "|" << setw(w8) << (c ? (" C" + to_string(c->getIndex())) : " -")
                    << "|" << setw(w9) << (c ? (" " + c->getDestination()) : " ")
                    << "|" << setw(w10) << (c ? (" " + formatTime12h(c->getTime())) : " ")
                    << "|" << setw(w11) << " "
                    << "|" << setw(w12) << " "
                    << "|\n";
            }
        }

        // Row separator
        os << "+" << string(w1, '-') << "+" << string(w2, '-') << "+" << string(w3, '-') << "+"
            << string(w4, '-') << "+" << string(w5, '-') << "+" << string(w6, '-') << "+"
            << string(w7, '-') << "+" << string(w8, '-') << "+" << string(w9, '-') << "+"
            << string(w10, '-') << "+" << string(w11, '-') << "+" << string(w12, '-') << "+\n";
    }
}

void SchedulePresenter::displayGroupingByLeastFreights(ostream& os, const vector<Shipment>& plans, const vector<freight>& freights, const vector<Cargo>& cargos)
{
    using std::left;
    using std::setw;
    using std::string;

    // build lookup maps
    unordered_map<int, const freight*> fmap;
    for (const auto& f : freights) fmap[f.getIndex()] = &f;
    unordered_map<int, const Cargo*> cmap;
    for (const auto& c : cargos) cmap[c.getIndex()] = &c;

    // group shipments by freight ID
    map<int, vector<const Shipment*>> freightGroups;
    for (const auto& s : plans) {
        freightGroups[s.getFreightIndex()].push_back(&s);
    }

    // sort freight IDs in ASCENDING order
    vector<int> sortedFreightIds;
    for (const auto& p : freightGroups) {
        sortedFreightIds.push_back(p.first);
    }
    sort(sortedFreightIds.begin(), sortedFreightIds.end());

    // Column widths
    const int w1 = 12;
    const int w2 = 15;
    const int w3 = 18;
    const int w4 = 18;
    const int w5 = 18;
    const int w6 = 18;
    const int w7 = 20;
    const int w8 = 12;
    const int w9 = 18;
    const int w10 = 18;
    const int w11 = 18;
    const int w12 = 18;

    // title
    os << "\nScheduling: By Minimum Freight\n\n";

    // top border
    os << "+" << string(w1, '-') << "+" << string(w2, '-') << "+" << string(w3, '-') << "+"
        << string(w4, '-') << "+" << string(w5, '-') << "+" << string(w6, '-') << "+"
        << string(w7, '-') << "+" << string(w8, '-') << "+" << string(w9, '-') << "+"
        << string(w10, '-') << "+" << string(w11, '-') << "+" << string(w12, '-') << "+\n";

    // header rows (same as displayGroupingByArrival)
    os << "|" << left << setw(w1) << " Schedule #"
        << "|" << setw(w2) << " Freight"
        << "|" << setw(w3) << " Freight"
        << "|" << setw(w4) << " Freight"
        << "|" << setw(w5) << " Freight"
        << "|" << setw(w6) << " Freight:"
        << "|" << setw(w7) << " Freight:"
        << "|" << setw(w8) << " Cargo"
        << "|" << setw(w9) << " Cargo"
        << "|" << setw(w10) << " Cargo"
        << "|" << setw(w11) << " Cargo:"
        << "|" << setw(w12) << " Cargo:"
        << "|\n";

    os << "|" << left << setw(w1) << " "
        << "|" << setw(w2) << " Name"
        << "|" << setw(w3) << " Destination"
        << "|" << setw(w4) << " Refuel"
        << "|" << setw(w5) << " Capacity"
        << "|" << setw(w6) << " Original"
        << "|" << setw(w7) << " Outstanding"
        << "|" << setw(w8) << " Name"
        << "|" << setw(w9) << " Destination"
        << "|" << setw(w10) << " Time"
        << "|" << setw(w11) << " Original"
        << "|" << setw(w12) << " Outstanding"
        << "|\n";

    os << "|" << left << setw(w1) << " "
        << "|" << setw(w2) << " "
        << "|" << setw(w3) << " "
        << "|" << setw(w4) << " Time"
        << "|" << setw(w5) << " Name"
        << "|" << setw(w6) << " Capacity"
        << "|" << setw(w7) << " Capacity or"
        << "|" << setw(w8) << " "
        << "|" << setw(w9) << " "
        << "|" << setw(w10) << " to"
        << "|" << setw(w11) << " Number"
        << "|" << setw(w12) << " number of"
        << "|\n";

    os << "|" << left << setw(w1) << " "
        << "|" << setw(w2) << " "
        << "|" << setw(w3) << " "
        << "|" << setw(w4) << " "
        << "|" << setw(w5) << " "
        << "|" << setw(w6) << " Values"
        << "|" << setw(w7) << " Unfulfilled"
        << "|" << setw(w8) << " "
        << "|" << setw(w9) << " "
        << "|" << setw(w10) << " Reach"
        << "|" << setw(w11) << " of Cargo per"
        << "|" << setw(w12) << " Cargo"
        << "|\n";

    os << "|" << left << setw(w1) << " "
        << "|" << setw(w2) << " "
        << "|" << setw(w3) << " "
        << "|" << setw(w4) << " "
        << "|" << setw(w5) << " "
        << "|" << setw(w6) << " "
        << "|" << setw(w7) << " Capacity"
        << "|" << setw(w8) << " "
        << "|" << setw(w9) << " "
        << "|" << setw(w10) << " Destination"
        << "|" << setw(w11) << " Group"
        << "|" << setw(w12) << " without freight"
        << "|\n";

    os << "+" << string(w1, '-') << "+" << string(w2, '-') << "+" << string(w3, '-') << "+"
        << string(w4, '-') << "+" << string(w5, '-') << "+" << string(w6, '-') << "+"
        << string(w7, '-') << "+" << string(w8, '-') << "+" << string(w9, '-') << "+"
        << string(w10, '-') << "+" << string(w11, '-') << "+" << string(w12, '-') << "+\n";

    int scheduleNo = 1;

    // track assigned quantity per cargo as we process each freight in order
    unordered_map<int, int> assignedSoFar;

    // iterate through sorted freight IDs
    for (int fid : sortedFreightIds) {
        const freight* f = nullptr;
        auto fit = fmap.find(fid);
        if (fit != fmap.end()) f = fit->second;

        const auto& shipments = freightGroups[fid];
        if (shipments.empty()) continue;

        int origCap = f ? f->getCapacity() : 0;

        // calculate "Cargo: Original Number of Cargo per Group"
        // show the original total quantity of the FIRST cargo in this freight
        int groupCargoCount = 0;
        if (!shipments.empty()) {
            const Shipment* firstShipment = shipments[0];
            int firstCargoId = firstShipment->getCargoIndex();
            auto cit = cmap.find(firstCargoId);
            if (cit != cmap.end()) {
                groupCargoCount = cit->second->getCapacity();
            }
        }

        // track cumulative capacity used
        int cumulativeUsedCap = 0;
        bool first = true;

        for (const auto* s : shipments) {
            const Cargo* c = nullptr;
            auto cit = cmap.find(s->getCargoIndex());
            if (cit != cmap.end()) c = cit->second;

            // add current cargo quantity to cumulative total for this freight
            int cargoQty = s->getCargoQuantity() > 0 ? s->getCargoQuantity() : (c ? c->getCapacity() : 1);
            cumulativeUsedCap += cargoQty;

            // track assignment across all freights processed so far
            assignedSoFar[s->getCargoIndex()] += cargoQty;

            // calculate outstanding freight capacity after this cargo is assigned
            int outstanding = max(0, origCap - cumulativeUsedCap);

            // calculate "Cargo: Outstanding number of Cargo without freight"
            // show remaining quantity of the first cargo AFTER THIS FREIGHT
            int unfulfilled = 0;
            if (first && !shipments.empty()) {
                const Shipment* firstShipment = shipments[0];
                int firstCargoId = firstShipment->getCargoIndex();
                auto ccit = cmap.find(firstCargoId);
                if (ccit != cmap.end()) {
                    int totalCapacity = ccit->second->getCapacity();
                    int assigned = assignedSoFar[firstCargoId];
                    unfulfilled = max(0, totalCapacity - assigned);
                }
            }

            if (first) {
                os << "|" << left << setw(w1) << (" " + to_string(scheduleNo++))
                    << "|" << setw(w2) << (f ? (" F" + to_string(f->getIndex())) : " ")
                    << "|" << setw(w3) << (f ? (" " + f->getDestination()) : " ")
                    << "|" << setw(w4) << (f ? (" " + formatTime12h(f->getTime())) : " ")
                    << "|" << setw(w5) << (f ? (" " + f->getTypeName()) : " ")
                    << "|" << setw(w6) << (" " + to_string(origCap))
                    << "|" << setw(w7) << (" " + to_string(outstanding))
                    << "|" << setw(w8) << (c ? (" C" + to_string(c->getIndex())) : " -")
                    << "|" << setw(w9) << (c ? (" " + c->getDestination()) : " ")
                    << "|" << setw(w10) << (c ? (" " + formatTime12h(c->getTime())) : " ")
                    << "|" << setw(w11) << (" " + to_string(groupCargoCount))
                    << "|" << setw(w12) << (" " + to_string(unfulfilled))
                    << "|\n";
                first = false;
            }
            else {
                os << "|" << left << setw(w1) << " "
                    << "|" << setw(w2) << " "
                    << "|" << setw(w3) << " "
                    << "|" << setw(w4) << " "
                    << "|" << setw(w5) << " "
                    << "|" << setw(w6) << " "
                    << "|" << setw(w7) << (" " + to_string(outstanding))
                    << "|" << setw(w8) << (c ? (" C" + to_string(c->getIndex())) : " -")
                    << "|" << setw(w9) << (c ? (" " + c->getDestination()) : " ")
                    << "|" << setw(w10) << (c ? (" " + formatTime12h(c->getTime())) : " ")
                    << "|" << setw(w11) << " "
                    << "|" << setw(w12) << " "
                    << "|\n";
            }
        }

        os << "+" << string(w1, '-') << "+" << string(w2, '-') << "+" << string(w3, '-') << "+"
            << string(w4, '-') << "+" << string(w5, '-') << "+" << string(w6, '-') << "+"
            << string(w7, '-') << "+" << string(w8, '-') << "+" << string(w9, '-') << "+"
            << string(w10, '-') << "+" << string(w11, '-') << "+" << string(w12, '-') << "+\n";
    }
}