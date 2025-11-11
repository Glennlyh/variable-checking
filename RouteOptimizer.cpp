#include "RouteOptimizer.h"
#include <algorithm>

using namespace std;

RouteOptimizer::RouteOptimizer() 
{
    initializeDistanceMatrix();
}

void RouteOptimizer::initializeDistanceMatrix() 
{
    // Common destinations and their distances (in km)
    // SIN = Singapore, HKG = Hong Kong, NRT = Tokyo, BKK = Bangkok
    // PVG = Shanghai, ICN = Seoul, MNL = Manila, KUL = Kuala Lumpur
    
    // From SIN (Singapore)
    distanceMatrix_[{"SIN", "HKG"}] = 2590.0;
    distanceMatrix_[{"SIN", "NRT"}] = 5320.0;
    distanceMatrix_[{"SIN", "BKK"}] = 1430.0;
    distanceMatrix_[{"SIN", "PVG"}] = 4120.0;
    distanceMatrix_[{"SIN", "ICN"}] = 4680.0;
    distanceMatrix_[{"SIN", "MNL"}] = 2380.0;
    distanceMatrix_[{"SIN", "KUL"}] = 320.0;
    
    // From HKG (Hong Kong)
    distanceMatrix_[{"HKG", "SIN"}] = 2590.0;
    distanceMatrix_[{"HKG", "NRT"}] = 2900.0;
    distanceMatrix_[{"HKG", "BKK"}] = 1710.0;
    distanceMatrix_[{"HKG", "PVG"}] = 1220.0;
    distanceMatrix_[{"HKG", "ICN"}] = 2080.0;
    distanceMatrix_[{"HKG", "MNL"}] = 1130.0;
    distanceMatrix_[{"HKG", "KUL"}] = 2560.0;
    
    // From NRT (Tokyo)
    distanceMatrix_[{"NRT", "SIN"}] = 5320.0;
    distanceMatrix_[{"NRT", "HKG"}] = 2900.0;
    distanceMatrix_[{"NRT", "BKK"}] = 4610.0;
    distanceMatrix_[{"NRT", "PVG"}] = 1770.0;
    distanceMatrix_[{"NRT", "ICN"}] = 1160.0;
    distanceMatrix_[{"NRT", "MNL"}] = 3010.0;
    distanceMatrix_[{"NRT", "KUL"}] = 5250.0;
    
    // From BKK (Bangkok)
    distanceMatrix_[{"BKK", "SIN"}] = 1430.0;
    distanceMatrix_[{"BKK", "HKG"}] = 1710.0;
    distanceMatrix_[{"BKK", "NRT"}] = 4610.0;
    distanceMatrix_[{"BKK", "PVG"}] = 2920.0;
    distanceMatrix_[{"BKK", "ICN"}] = 3560.0;
    distanceMatrix_[{"BKK", "MNL"}] = 2340.0;
    distanceMatrix_[{"BKK", "KUL"}] = 1200.0;
    
    // From PVG (Shanghai)
    distanceMatrix_[{"PVG", "SIN"}] = 4120.0;
    distanceMatrix_[{"PVG", "HKG"}] = 1220.0;
    distanceMatrix_[{"PVG", "NRT"}] = 1770.0;
    distanceMatrix_[{"PVG", "BKK"}] = 2920.0;
    distanceMatrix_[{"PVG", "ICN"}] = 860.0;
    distanceMatrix_[{"PVG", "MNL"}] = 1960.0;
    distanceMatrix_[{"PVG", "KUL"}] = 3820.0;
    
    // From ICN (Seoul)
    distanceMatrix_[{"ICN", "SIN"}] = 4680.0;
    distanceMatrix_[{"ICN", "HKG"}] = 2080.0;
    distanceMatrix_[{"ICN", "NRT"}] = 1160.0;
    distanceMatrix_[{"ICN", "BKK"}] = 3560.0;
    distanceMatrix_[{"ICN", "PVG"}] = 860.0;
    distanceMatrix_[{"ICN", "MNL"}] = 2610.0;
    distanceMatrix_[{"ICN", "KUL"}] = 4520.0;
    
    // From MNL (Manila)
    distanceMatrix_[{"MNL", "SIN"}] = 2380.0;
    distanceMatrix_[{"MNL", "HKG"}] = 1130.0;
    distanceMatrix_[{"MNL", "NRT"}] = 3010.0;
    distanceMatrix_[{"MNL", "BKK"}] = 2340.0;
    distanceMatrix_[{"MNL", "PVG"}] = 1960.0;
    distanceMatrix_[{"MNL", "ICN"}] = 2610.0;
    distanceMatrix_[{"MNL", "KUL"}] = 2550.0;
    
    // From KUL (Kuala Lumpur)
    distanceMatrix_[{"KUL", "SIN"}] = 320.0;
    distanceMatrix_[{"KUL", "HKG"}] = 2560.0;
    distanceMatrix_[{"KUL", "NRT"}] = 5250.0;
    distanceMatrix_[{"KUL", "BKK"}] = 1200.0;
    distanceMatrix_[{"KUL", "PVG"}] = 3820.0;
    distanceMatrix_[{"KUL", "ICN"}] = 4520.0;
    distanceMatrix_[{"KUL", "MNL"}] = 2550.0;
}

double RouteOptimizer::getDistance(const string& from, const string& to) const 
{
    if (from == to) 
    {
        return 0.0;
    }
    
    auto it = distanceMatrix_.find({from, to});
    if (it != distanceMatrix_.end()) 
    {
        return it->second;
    }
    
    // If not found, return a large value to indicate no direct route
    return -1.0;
}

double RouteOptimizer::calculateCost(double distance, int transfers) const 
{
    return distance + (transfers * TRANSFER_PENALTY);
}

double RouteOptimizer::getTransferPenalty() const 
{
    return TRANSFER_PENALTY;
}

bool RouteOptimizer::hasDirectRoute(const string& from, const string& to) const 
{
    if (from == to) return true;
    return distanceMatrix_.find({from, to}) != distanceMatrix_.end();
}

vector<string> RouteOptimizer::getAllDestinations() const 
{
    vector<string> destinations;
    
    for (const auto& pair : distanceMatrix_) 
    {
        const string& from = pair.first.first;
        if (find(destinations.begin(), destinations.end(), from) == destinations.end()) 
        {
            destinations.push_back(from);
        }
    }
    
    return destinations;
}
