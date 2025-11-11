#pragma once
#include <string>
#include "CargoList.h"

struct CargoRepository {
    static CargoList loadFromFile(const std::string& path) 
    {
        CargoList list;
        list.loadFromFile(path);
        return list;
    }
};
