#pragma once  

#include "Cargo.h"
#include <vector>
#include <string>
#include <ostream>

using namespace std;

class CargoList
{
private:
    vector<Cargo> cargos;
public:
    const vector<Cargo>& all() const;
    size_t size() const;

    bool add(const Cargo& cargo);
    bool remove(int idx);
    bool edit(int idx, const std::string& newDest, int newTime);


    const Cargo* findById(int idx) const;
    Cargo* findById(int idx);

    void displayAll(std::ostream& os) const;
    void displayAll() const;

    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
};
