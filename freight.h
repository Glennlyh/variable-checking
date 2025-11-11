#pragma once

#include <string>
#include <ostream>
#include "logistics.h"


class freight : public logistics 
{


public:
    enum class FreightType 
    { 
        MiniMover, CargoCruiser, MegaCarrier                    // have to declare here or else constructor will error at "FreightType::MiniMover"
    };                                                          // tried using forward declare but didnt work

    freight(int idx = 0, std::string dest = "", int t = 0, FreightType type = FreightType::MiniMover);
    void display(std::ostream& os) const override;

    FreightType getType() const;
    void setType(FreightType t);
    int getCapacity() const;
    std::string getTypeName() const;
    static FreightType parseTypeName(const std::string& s);

private:
    FreightType type_;                                      // declaring private here instead due to FreightType being not yet defined if we put at the top


};
