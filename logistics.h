#pragma once

#include <string>
#include <ostream>

class logistics
{
private:
    int index;
    std::string destination;
    int time;

public:
    logistics(int idx = 0, std::string dest = "", int t = 0);
    virtual ~logistics() = default;

    int getIndex() const;
    void setIndex(int idx);

    std::string getDestination() const;
    void setDestination(std::string dest);

    int getTime() const;
    void setTime(int t);

    // Runtime polymorphism hook
    virtual void display(std::ostream& os) const = 0;
};

