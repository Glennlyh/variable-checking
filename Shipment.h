#pragma once

#include "freight.h"
#include "Cargo.h"
#include <string>

class Shipment
{
    freight Freight;
    Cargo cargo;
    int scheduleNo{ 0 };
    int cargoQuantity{ 0 };  // number of cargo items in this shipment
public:
    Shipment() = default;
    Shipment(const freight& f, const Cargo& c, int schedNo = 0, int quantity = 0);

    bool IsMatching() const;

    int getFreightIndex() const;
    int getCargoIndex() const;

    string getFreightDes() const;
    string getCargoDes() const;

    int getFreightTime() const;
    int getCargoTime() const;

    // new accessors
    int getScheduleNo() const { return scheduleNo; }
    const freight& getFreight() const { return Freight; }
    const Cargo& getCargo() const { return cargo; }

    int getCargoQuantity() const { return cargoQuantity; }  
    void setCargoQuantity(int qty) { cargoQuantity = qty; } 

    string getFreightName() const { return "F" +  to_string(Freight.getIndex()); }
    string getCargoName() const { return "C" +  to_string(cargo.getIndex()); }
};