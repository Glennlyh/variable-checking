#include "CargoList.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "functions.h"
using namespace std;


const vector<Cargo>& CargoList::all() const
{
    return cargos;
}


size_t CargoList::size() const
{
    return cargos.size();
}


bool CargoList::add(const Cargo& cargo)
{
    
    for (const auto& c : cargos)
    {
        if (c.getIndex() == cargo.getIndex())
        {
            return false;
        }
    }

    cargos.push_back(cargo);
    return true;
}


bool CargoList::remove(int idx) {
    for (auto it = cargos.begin(); it != cargos.end(); ++it)
    {
        if (it->getIndex() == idx)
        {
            cargos.erase(it);
            return true;
        }
    }

    return false;
}


bool CargoList::edit(int idx, const string& newDest, int newTime)
{
    for (auto& c : cargos)
    {
        if (c.getIndex() == idx)
        {
            c.setDestination(newDest);
            c.setTime(newTime);
            return true;
        }
    }

    return false;
}


void CargoList::displayAll(std::ostream& os) const
{
    for (const auto& c : cargos)
    {
        c.display(os);
    }

}


bool CargoList::saveToFile(const string& filename) const
{
    ofstream file(filename);
    if (!file.good()) return false;
    for (const auto& c : cargos)
    {
        // write capacity as fourth field to preserve it
        file << "C" << c.getIndex() << ","
            << c.getDestination() << ","
            << c.getTime() << ","
            << c.getCapacity() << endl;
    }

    return true;
}


bool CargoList::loadFromFile(const string& filename)
{
    ifstream in(filename);
    string line;
    if (!in) return false;
    vector<Cargo> loaded;
    int id, t;
    string id_, dest, t_, cap_;
    while (getline(in, line))
    {
        stringstream ss(line);
        string value;
                                                            // read each comma as delimiter
        if (!getline(ss, id_, ',')) continue;
        if (!getline(ss, dest, ',')) continue;
        if (!getline(ss, t_, ',')) continue;
        // try to read capacity if present
        if (!getline(ss, cap_, ',')) {
            // maybe cap was last field without trailing comma
            // attempt to read remainder
            if (!getline(ss, cap_)) cap_.clear();
        }

        try
        {
            id = stoi(id_.substr(1));
        }
        catch (...)
        {
            continue;
        }

        int parsed = parseTime(t_);
        if (parsed < 0) continue;
        t = parsed;

        int capacity = 1; // default
        if (!cap_.empty()) {
            try { capacity = stoi(cap_); }
            catch (...) { capacity = 1; }
            if (capacity <= 0) capacity = 1;
        }

        // reads the string as number ignoring the C
        Cargo c(id, dest, t, capacity);

        if (c.getDestination().empty() || !isValidTime(c.getTime()))  //sanity check
            continue;
        if (none_of(loaded.begin(), loaded.end(),
            [id](const Cargo& x)
            {
                return x.getIndex() == id;              // another sanity check
            }))
            loaded.push_back(c);                    //next set of cargo
    }
    cargos.swap(loaded);
    return true;
}


void CargoList::displayAll() const { displayAll(std::cout); }


// added findById implementations
const Cargo* CargoList::findById(int idx) const {
    for (const auto& c : cargos) {
        if (c.getIndex() == idx) {
            return &c;
        }
    }
    return nullptr;
}

Cargo* CargoList::findById(int idx) {
    for (auto& c : cargos) {
        if (c.getIndex() == idx) {
            return &c;
        }
    }
    return nullptr;
}
