#include "logistics.h"

logistics::logistics(int idx, std::string dest, int t)
    : index(idx), Destination(dest), time(t) 
{
}


int logistics::getIndex() const
{
    return index;
}


void logistics::setIndex(int idx) 
{
    index = idx;
}


std::string logistics::getDestination() const 
{
    return Destination;
}


void logistics::setDestination(std::string dest) 
{
    Destination = dest;
}


int logistics::getTime() const 
{
    return time;
}


void logistics::setTime(int t) 
{
    time = t;
}
