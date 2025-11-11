#pragma once

#include "Route.h"
#include "RouteOptimizer.h"
#include "freightlist.h"
#include "CargoList.h"
#include <vector>
#include <string>

using namespace std;

// Strategy interface for route planning algorithms
class RoutePlanningStrategy 
{
public:
    virtual ~RoutePlanningStrategy() = default;
    virtual vector<Route> planRoutes(const string& origin, 
                                      const string& destination,
                                      const freightlist& freights,
                                      const RouteOptimizer& optimizer) = 0;
};

// Direct route planning strategy
class DirectRouteStrategy : public RoutePlanningStrategy 
{
public:
    vector<Route> planRoutes(const string& origin, 
                            const string& destination,
                            const freightlist& freights,
                            const RouteOptimizer& optimizer) override;
};

// Multi-hop route planning strategy
class MultiHopRouteStrategy : public RoutePlanningStrategy 
{
private:
    int maxHops_;  // Maximum number of hops allowed
    
public:
    explicit MultiHopRouteStrategy(int maxHops = 2);
    
    vector<Route> planRoutes(const string& origin, 
                            const string& destination,
                            const freightlist& freights,
                            const RouteOptimizer& optimizer) override;
                            
private:
    // Helper function to find routes with BFS-like approach
    void findRoutesRecursive(const string& current,
                            const string& destination,
                            vector<string>& path,
                            vector<Route>& results,
                            const RouteOptimizer& optimizer,
                            double currentDistance,
                            int hopsRemaining);
};

// Main RoutePlanner class that uses strategies
class RoutePlanner 
{
private:
    RouteOptimizer optimizer_;
    
public:
    RoutePlanner();
    
    // Calculate optimal routes for cargo from origin to destination
    vector<Route> findOptimalRoutes(const string& origin,
                                    const string& destination,
                                    const freightlist& freights);
    
    // Find the best route (lowest cost)
    Route findBestRoute(const string& origin,
                       const string& destination,
                       const freightlist& freights);
    
    // Compare direct vs multi-hop routes
    void compareRouteOptions(const string& origin,
                           const string& destination,
                           const freightlist& freights);
    
    // Get the optimizer instance
    const RouteOptimizer& getOptimizer() const;
};
