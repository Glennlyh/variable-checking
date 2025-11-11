#include "PlanRepository.h"
#include "PlanExporter.h"
#include "functions.h"
#include <fstream>
#include <algorithm>

using namespace std;

bool PlanRepository::saveCSV(const vector<Shipment>& plans, const string& filepath) 
{
    ofstream ofs(filepath.c_str());
    if (!ofs.good()) return false;
    PlanExporter::toCSV(plans, ofs);
    return true;
}

bool PlanRepository::saveGrouping(const unordered_map<int, vector<int>>& groups,
    const vector<freight>& freights,
    const vector<Cargo>& cargos,
    const string& filename)
{
    ofstream out(filename);
    if (!out.is_open()) return false;

    // create lookup maps
    unordered_map<int, const freight*> fmap;
    for (const auto& f : freights) fmap[f.getIndex()] = &f;
    unordered_map<int, const Cargo*> cmap;
    for (const auto& c : cargos) cmap[c.getIndex()] = &c;

    // sort freight ID
    vector<int> sortedFreightIds;
    for (const auto& p : groups)
    {
        sortedFreightIds.push_back(p.first);
    }
    sort(sortedFreightIds.begin(), sortedFreightIds.end());

    // write header
    out << "Schedule#,FreightID,FreightDest,FreightTime,FreightType,FreightCapacity,CargoID,CargoDest,CargoTime,CargoCapacity\n";

    int scheduleNo = 1;
    for (int fid : sortedFreightIds)
    {
        auto fit = fmap.find(fid);
        if (fit == fmap.end()) continue;
        const freight* f = fit->second;

        auto git = groups.find(fid);
        if (git == groups.end() || git->second.empty())
        {
            // Freight with no cargo
            out << scheduleNo++ << ",F" << f->getIndex() << ","
                << f->getDestination() << ","
                << formatTime12h(f->getTime()) << ","
                << f->getTypeName() << ","
                << f->getCapacity() << ","
                << "-,-,-,-\n";
            continue;
        }

        // sort cargos
        vector<int> sortedCargoIds = git->second;
        sort(sortedCargoIds.begin(), sortedCargoIds.end());

        for (int cid : sortedCargoIds)
        {
            auto cit = cmap.find(cid);
            if (cit == cmap.end()) continue;
            const Cargo* c = cit->second;

            out << scheduleNo++ << ",F" << f->getIndex() << ","
                << f->getDestination() << ","
                << formatTime12h(f->getTime()) << ","
                << f->getTypeName() << ","
                << f->getCapacity() << ","
                << "C" << c->getIndex() << ","
                << c->getDestination() << ","
                << formatTime12h(c->getTime()) << ","
                << c->getCapacity() << "\n";
        }
    }

    out.close();
    return true;
}