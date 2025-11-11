#pragma once

#include <string>
#include <ostream>
using namespace std;

class logistics
{
private:
    int index;
    string Destination;
    int time;

public:
    logistics(int idx = 0, string dest = "", int t = 0);
    virtual ~logistics() = default;

    int getIndex() const;
    void setIndex(int idx);

    string getDestination() const;
    void setDestination(string dest);

    int getTime() const;
    void setTime(int t);

    // Runtime polymorphism hook
    virtual void display(ostream& os) const = 0;
};

