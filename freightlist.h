#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <ostream>
#include "freight.h"

class freightlist
{
    std::vector<freight> items_;
public:
    bool add(const freight& f);                                 // list manipulation
    bool remove(int id);
    bool edit(int id, const std::string& newDest, int newTime);

    bool saveToFile(const std::string& filename);               // file access
    bool loadFromFile(const std::string& filename);

    void displayAll();                                          // existing (non-const)
    void displayAll(std::ostream& os) const;                    // added overload

    const std::vector<freight>& all() const;                    // accessor for read-only

    freight* findById(int id);
};

