#include "functions.h"
#include "freight.h"
#include <iostream>
using namespace std;

freight::freight(int idx, std::string dest, int t, FreightType type) : logistics(idx, dest, t), type_(type)
{
}


void freight::display(std::ostream& os) const 
{
    os << "Freight " << getIndex()
        << " (" << getTypeName() << ", cap " << getCapacity() << ")"
        << " -> Destination: " << getDestination()
        << " | Time: " << formatTime12h(getTime()) << '\n';
}

freight::FreightType freight::getType() const
{
    return type_;
}

void freight::setType(FreightType t)
{
    type_ = t;
}

int freight::getCapacity() const
{
    switch (getType())
    {
    case FreightType::MiniMover: return 2;
    case FreightType::CargoCruiser: return 6;    // Kept at 6 as requested
    case FreightType::MegaCarrier: return 12;    // Kept at 12 as requested
    }
    return 2;
}

string freight::getTypeName() const
{
    switch (getType())
    {
    case FreightType::MiniMover: return "MiniMover";
    case FreightType::CargoCruiser: return "CargoCruiser";
    case FreightType::MegaCarrier: return "MegaCarrier";
    }
    return "Unknown";
}

freight::FreightType freight::parseTypeName(const std::string& s)
{
    std::string lower;
    lower.reserve(s.size());
    for (char c : s) lower.push_back(std::tolower(static_cast<unsigned char>(c)));
    if (lower.find("minim") != std::string::npos) return FreightType::MiniMover;
    if (lower.find("cruis") != std::string::npos) return FreightType::CargoCruiser;
    if (lower.find("mega") != std::string::npos) return FreightType::MegaCarrier;
    return FreightType::MiniMover;
}
