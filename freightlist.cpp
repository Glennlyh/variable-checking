#include "freightlist.h"
#include "functions.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

static inline std::string trim(const std::string& s) 
{
    size_t a = 0, b = s.size();
    while (a < b && isspace(static_cast<unsigned char>(s[a]))) ++a;
    while (b > a && isspace(static_cast<unsigned char>(s[b-1]))) --b;
    return s.substr(a, b - a);
}

bool freightlist::add(const freight& f) 
{
    if (findById(f.getIndex()))
    {
        return false;
    }

    items_.push_back(f);
    return true;
}


bool freightlist::remove(int id) 
{
    auto it = find_if(items_.begin(), items_.end(),[id](const freight& x)
        { 
            return x.getIndex() == id; 
        }
    );

    if (it == items_.end())
    {
        return false;
    }

    items_.erase(it);
    return true;
}


freight* freightlist::findById(int id) 
{
    auto it = find_if(items_.begin(), items_.end(),[id](const freight& x)
        { 
            return x.getIndex() == id; 
        }
    );

    return it == items_.end() ? nullptr : &*it;
}


bool freightlist::edit(int id, const string& newDest, int newTime) 
{
    freight* f = findById(id);
    if (!f)
    {
        return false;
    }   

    f->setDestination(newDest);
    f->setTime(newTime);
    return true;
}


bool freightlist::saveToFile(const string& filename) 
{
    ofstream out(filename);
    if (!out)
    {
        return false;
    }

    for (auto& f : items_)
    {
        // saving format: F{id},{destination},{time},{type}\n (type last)
        out << "F" << f.getIndex() << ','
            << f.getDestination() << ','
            << f.getTime() << ','
            << f.getTypeName() << '\n';
    }
    return true;
}


bool freightlist::loadFromFile(const string& filename) 
{
    ifstream in(filename);
    string line;
    if (!in) return false;
    vector<freight> loaded;

    int id, t;                                      // id and time parsed
    string id_, dest, t_, type_;
    while (getline(in, line))
    {
        if (line.empty()) continue;
        stringstream ss(line);
        // expect exactly 4 fields: id,dest,time,type (type last)
        if (!getline(ss, id_, ',')) continue;
        if (!getline(ss, dest, ',')) continue;
        if (!getline(ss, t_, ',')) continue;
        if (!getline(ss, type_)) continue; // rest of line

        // trim
        id_ = trim(id_);
        type_ = trim(type_);
        dest = trim(dest);
        t_ = trim(t_);

        // basic validation
        if (id_.empty() || dest.empty() || t_.empty() || type_.empty()) continue;

        // id should start with a letter like 'F' followed by number
        if (id_.size() < 2) continue;

        try {
            id = stoi(id_.substr(1));
        }
        catch (...) { continue; }

        int parsed = parseTime(t_);
        if (parsed < 0) continue;
        t = parsed;

        // parse freight type (must be present)
        auto ftype = freight::parseTypeName(type_);

        freight f(id, dest, t, ftype);

        if (f.getDestination().empty() || !isValidTime(f.getTime()))
            continue;

        if (none_of(loaded.begin(), loaded.end(), [id](const freight& x) { return x.getIndex() == id; }))
            loaded.push_back(f);
    }
    items_.swap(loaded);
    return !items_.empty();
}


void freightlist::displayAll() 
{
    displayAll(std::cout);
}


void freightlist::displayAll(std::ostream& os) const 
{
    for (const auto& f : items_) {
        f.display(os);
    }
}


const vector<freight>& freightlist::all() const
{
    return items_; 
}
