#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <ostream>
#include "freight.h"

// Forward declaration
class FreightObserver;

class freightlist
{
    std::vector<freight> items_;
    std::vector<FreightObserver*> observers_;  // observer pattern support
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
    
    // Observer pattern methods
    void addObserver(FreightObserver* observer);
    void removeObserver(FreightObserver* observer);
    void notifyFreightRemoved(const freight& f);
};

