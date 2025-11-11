#include "functions.h"
#include "Cargo.h"
#include <iostream>

Cargo::Cargo(int idx, std::string dest, int t, int capacity)
    : logistics(idx, dest, t), capacity_(capacity)
{
}

void Cargo::display(std::ostream& os) const 
{
    os << "Cargo " << getIndex()
        << " (cap " << getCapacity() << ")"
        << " -> Destination: " << getDestination()
        << " | Time: " << formatTime12h(getTime()) << std::endl;
}

int Cargo::getCapacity() const 
{ 
    return capacity_; 
}
void Cargo::setCapacity(int cap) 
{ 
    capacity_ = cap; 
}


