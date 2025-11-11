#pragma once

#include <vector>
#include <unordered_map>
#include "freight.h"
#include "Cargo.h"

// abstract base class for grouping strategies
class GroupingStrategy 
{
public:
    virtual ~GroupingStrategy() = default;
    virtual std::unordered_map<int, std::vector<int>> group(const std::vector<freight>& freights, const std::vector<Cargo>& cargos) const = 0;
};



// sort by time
class PerfectArrivalGrouping : public GroupingStrategy 
{
public:
    std::unordered_map<int, std::vector<int>> group(const std::vector<freight>& freights, const std::vector<Cargo>& cargos) const override;
};



// sort by min freight groupings
class LeastFreightsGrouping : public GroupingStrategy 
{
public:
    std::unordered_map<int, std::vector<int>> group(const std::vector<freight>& freights, const std::vector<Cargo>& cargos) const override;
};
