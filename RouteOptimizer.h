#pragma once

#include <string>
#include <unordered_map>
#include <map>
#include <vector>

using namespace std;

// RouteOptimizer provides distance calculations and optimization utilities
class RouteOptimizer 
{
private:
    // Distance matrix: stores distances between common destinations
    // Using map of pairs for easier lookup
    map<pair<string, string>, double> distanceMatrix_;
    
    // Transfer penalty constant (added to cost for each transfer)
    static constexpr double TRANSFER_PENALTY = 50.0;
    
    void initializeDistanceMatrix();

public:
    RouteOptimizer();
    
    // Get distance between two destinations
    double getDistance(const string& from, const string& to) const;
    
    // Calculate cost (distance + penalties)
    double calculateCost(double distance, int transfers) const;
    
    // Get transfer penalty constant
    double getTransferPenalty() const;
    
    // Check if a route exists between two destinations
    bool hasDirectRoute(const string& from, const string& to) const;
    
    // Get all destinations that are available in the distance matrix
    vector<string> getAllDestinations() const;
};
