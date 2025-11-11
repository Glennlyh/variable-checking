#include "RoutePlanner.h"
#include "functions.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <set>

using namespace std;

// DirectRouteStrategy implementation
vector<Route> DirectRouteStrategy::planRoutes(
    const string& origin, 
    const string& destination,
    const freightlist& freights,
    const RouteOptimizer& optimizer)
{
    vector<Route> routes;
    
    // Check if direct route exists in distance matrix
    double distance = optimizer.getDistance(origin, destination);
    if (distance < 0) 
    {
        return routes;  // No direct route available
    }
    
    // Find freights that match this route
    const auto& allFreights = freights.all();
    for (const auto& f : allFreights) 
    {
        if (f.getDestination() == destination) 
        {
            Route route(origin, destination);
            route.addFreightId(f.getIndex());
            route.setTotalDistance(distance);
            route.setTotalCost(optimizer.calculateCost(distance, 0));
            route.setTransferCount(0);
            routes.push_back(route);
        }
    }
    
    return routes;
}

// MultiHopRouteStrategy implementation
MultiHopRouteStrategy::MultiHopRouteStrategy(int maxHops)
    : maxHops_(maxHops)
{
}

vector<Route> MultiHopRouteStrategy::planRoutes(
    const string& origin, 
    const string& destination,
    const freightlist& freights,
    const RouteOptimizer& optimizer)
{
    vector<Route> routes;
    
    // Get all possible destinations to use as waypoints
    vector<string> allDestinations = optimizer.getAllDestinations();
    
    // Try 1-hop routes
    for (const auto& waypoint : allDestinations) 
    {
        if (waypoint == origin || waypoint == destination) continue;
        
        double dist1 = optimizer.getDistance(origin, waypoint);
        double dist2 = optimizer.getDistance(waypoint, destination);
        
        if (dist1 >= 0 && dist2 >= 0) 
        {
            Route route(origin, destination);
            route.addWaypoint(waypoint);
            route.setTotalDistance(dist1 + dist2);
            route.setTransferCount(1);
            route.setTotalCost(optimizer.calculateCost(dist1 + dist2, 1));
            
            // Find suitable freights for each leg
            const auto& allFreights = freights.all();
            bool leg1Found = false, leg2Found = false;
            
            for (const auto& f : allFreights) 
            {
                if (!leg1Found && f.getDestination() == waypoint) 
                {
                    route.addFreightId(f.getIndex());
                    leg1Found = true;
                }
                if (leg1Found && !leg2Found && f.getDestination() == destination) 
                {
                    route.addFreightId(f.getIndex());
                    leg2Found = true;
                    break;
                }
            }
            
            if (leg1Found && leg2Found) 
            {
                routes.push_back(route);
            }
        }
    }
    
    // For simplicity, we'll limit to 1-hop for now
    // Could extend to 2-hop or more if needed
    
    return routes;
}

void MultiHopRouteStrategy::findRoutesRecursive(
    const string& current,
    const string& destination,
    vector<string>& path,
    vector<Route>& results,
    const RouteOptimizer& optimizer,
    double currentDistance,
    int hopsRemaining)
{
    // This is a helper for potential future expansion to more complex routing
    // Currently not used as we limit to simple 1-hop routes
}

// RoutePlanner implementation
RoutePlanner::RoutePlanner()
    : optimizer_()
{
}

vector<Route> RoutePlanner::findOptimalRoutes(
    const string& origin,
    const string& destination,
    const freightlist& freights)
{
    vector<Route> allRoutes;
    
    // Try direct routes
    DirectRouteStrategy directStrategy;
    auto directRoutes = directStrategy.planRoutes(origin, destination, freights, optimizer_);
    allRoutes.insert(allRoutes.end(), directRoutes.begin(), directRoutes.end());
    
    // Try multi-hop routes
    MultiHopRouteStrategy multiHopStrategy(2);
    auto multiHopRoutes = multiHopStrategy.planRoutes(origin, destination, freights, optimizer_);
    allRoutes.insert(allRoutes.end(), multiHopRoutes.begin(), multiHopRoutes.end());
    
    // Sort by total cost (ascending)
    sort(allRoutes.begin(), allRoutes.end(), 
         [](const Route& a, const Route& b) 
         {
             return a.getTotalCost() < b.getTotalCost();
         });
    
    return allRoutes;
}

Route RoutePlanner::findBestRoute(
    const string& origin,
    const string& destination,
    const freightlist& freights)
{
    auto routes = findOptimalRoutes(origin, destination, freights);
    
    if (!routes.empty()) 
    {
        return routes[0];  // Return the route with lowest cost
    }
    
    // Return empty route if no route found
    return Route(origin, destination);
}

void RoutePlanner::compareRouteOptions(
    const string& origin,
    const string& destination,
    const freightlist& freights)
{
    cout << "\n=== Route Optimization Analysis ===\n";
    cout << "Origin: " << origin << "\n";
    cout << "Destination: " << destination << "\n\n";
    
    auto routes = findOptimalRoutes(origin, destination, freights);
    
    if (routes.empty()) 
    {
        cout << "No routes available for this origin-destination pair.\n";
        return;
    }
    
    // Separate direct and multi-hop routes
    vector<Route> directRoutes;
    vector<Route> multiHopRoutes;
    
    for (const auto& route : routes) 
    {
        if (route.isDirect()) 
        {
            directRoutes.push_back(route);
        } 
        else 
        {
            multiHopRoutes.push_back(route);
        }
    }
    
    // Display direct routes
    if (!directRoutes.empty()) 
    {
        cout << "--- Direct Routes ---\n";
        cout << setw(15) << "Freight ID" 
             << setw(20) << "Distance (km)" 
             << setw(20) << "Total Cost" << "\n";
        cout << string(55, '-') << "\n";
        
        for (const auto& route : directRoutes) 
        {
            const auto& freightIds = route.getFreightIds();
            for (int fid : freightIds) 
            {
                cout << setw(15) << ("F" + to_string(fid))
                     << setw(20) << fixed << setprecision(2) << route.getTotalDistance()
                     << setw(20) << fixed << setprecision(2) << route.getTotalCost() << "\n";
            }
        }
        cout << "\n";
    }
    
    // Display multi-hop routes
    if (!multiHopRoutes.empty()) 
    {
        cout << "--- Multi-Hop Routes ---\n";
        cout << setw(30) << "Route Path" 
             << setw(15) << "Transfers" 
             << setw(20) << "Distance (km)" 
             << setw(20) << "Total Cost" << "\n";
        cout << string(85, '-') << "\n";
        
        for (const auto& route : multiHopRoutes) 
        {
            cout << setw(30) << route.getSummary()
                 << setw(15) << route.getTransferCount()
                 << setw(20) << fixed << setprecision(2) << route.getTotalDistance()
                 << setw(20) << fixed << setprecision(2) << route.getTotalCost() << "\n";
        }
        cout << "\n";
    }
    
    // Show cost comparison and savings
    if (!directRoutes.empty() && !multiHopRoutes.empty()) 
    {
        double bestDirectCost = directRoutes[0].getTotalCost();
        double bestMultiHopCost = multiHopRoutes[0].getTotalCost();
        
        cout << "--- Cost Comparison ---\n";
        cout << "Best Direct Route Cost: " << fixed << setprecision(2) << bestDirectCost << " units\n";
        cout << "Best Multi-Hop Route Cost: " << fixed << setprecision(2) << bestMultiHopCost << " units\n";
        
        if (bestMultiHopCost < bestDirectCost) 
        {
            double savings = bestDirectCost - bestMultiHopCost;
            double savingsPercent = (savings / bestDirectCost) * 100.0;
            cout << "Potential Savings with Multi-Hop: " << savings << " units (" 
                 << fixed << setprecision(1) << savingsPercent << "%)\n";
            cout << "Recommendation: Use multi-hop route for cost efficiency\n";
        } 
        else 
        {
            double extraCost = bestMultiHopCost - bestDirectCost;
            cout << "Direct route is cheaper by: " << extraCost << " units\n";
            cout << "Recommendation: Use direct route\n";
        }
    } 
    else if (!directRoutes.empty()) 
    {
        cout << "Only direct routes available. Best cost: " 
             << fixed << setprecision(2) << directRoutes[0].getTotalCost() << " units\n";
    } 
    else 
    {
        cout << "Only multi-hop routes available. Best cost: " 
             << fixed << setprecision(2) << multiHopRoutes[0].getTotalCost() << " units\n";
    }
    
    cout << "\nNote: Transfer penalty is " << optimizer_.getTransferPenalty() 
         << " units per transfer.\n";
}

const RouteOptimizer& RoutePlanner::getOptimizer() const 
{
    return optimizer_;
}
