#include "Shipment.h"
#include <string>


Shipment::Shipment(const freight& f, const Cargo& c, int schedNo, int quantity)
    : Freight(f), cargo(c), scheduleNo(schedNo), cargoQuantity(quantity)
{
}


bool Shipment::IsMatching() const
{
    if (Freight.getDestination() == cargo.getDestination() && Freight.getTime() == cargo.getTime())
    {
        return true;
    }
    else
    {
        return false;
    }
}


int Shipment::getFreightIndex() const
{
    return Freight.getIndex();
}


int Shipment::getCargoIndex() const
{
    return cargo.getIndex();
}


std::string Shipment::getFreightDes() const
{
    return Freight.getDestination();
}


std::string Shipment::getCargoDes() const
{
    return cargo.getDestination();
}


int Shipment::getFreightTime() const
{
    return Freight.getTime();
}


int Shipment::getCargoTime() const
{
    return cargo.getTime();
}