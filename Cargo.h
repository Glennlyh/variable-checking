#pragma once

#include "logistics.h"
#include <ostream>

class Cargo : public logistics 
{
public:

    Cargo(int idx = 0, std::string dest = "", int t = 0, int capacity = 1);
    void display(std::ostream& os) const override;

    int getCapacity() const;
    void setCapacity(int cap);

private:
    int capacity_;
};
