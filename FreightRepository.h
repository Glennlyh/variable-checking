#pragma once
#include <string>
#include "freightlist.h"

struct FreightRepository 
{
    static freightlist loadFromFile(const std::string& path) 
    {
        freightlist list;
        list.loadFromFile(path);
        return list;
    }
};
