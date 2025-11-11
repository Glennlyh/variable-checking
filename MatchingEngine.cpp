#include "MatchingEngine.h"

std::vector<Shipment> MatchAllPairs::match(const std::vector<freight>& F, const std::vector<Cargo>& C) 
{
    std::vector<Shipment> out;
    for (const auto& f : F) 
    {
        for (const auto& c : C) 
        {
            Shipment s(f, c);
            if (s.IsMatching()) out.push_back(s);
        }
    }
    return out;
}
