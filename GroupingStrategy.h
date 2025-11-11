#pragma once

#include <vector>
#include <unordered_map>
#include "freight.h"
#include "Cargo.h"
using namespace std;

// abstract base class for grouping strategies
class GroupingStrategy 
{
public:
    virtual ~GroupingStrategy() = default;
    virtual unordered_map<int, vector<int>> group(const vector<freight>& freights, const vector<Cargo>& cargos) const = 0;
};



// sort by time
class PerfectArrivalGrouping : public GroupingStrategy 
{
public:
    unordered_map<int, vector<int>> group(const vector<freight>& freights, const vector<Cargo>& cargos) const override;
};



// sort by min freight groupings
class LeastFreightsGrouping : public GroupingStrategy 
{
public:
    unordered_map<int, vector<int>> group(const vector<freight>& freights, const vector<Cargo>& cargos) const override;
};
