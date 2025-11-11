#pragma once
#include <vector>
#include "freight.h"
#include "Cargo.h"
#include "Shipment.h"

struct MatchStrategy 
{
    virtual ~MatchStrategy() = default;
    virtual std::vector<Shipment> match(const std::vector<freight>& F, const std::vector<Cargo>& C) = 0;
};

struct MatchAllPairs : MatchStrategy 
{
    std::vector<Shipment> match(const std::vector<freight>& F, const std::vector<Cargo>& C) override;
};
