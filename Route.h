#pragma once

#include <string>
#include <vector>

using namespace std;

// Represents a route for cargo delivery
class Route 
{
private:
    string origin_;
    string destination_;
    vector<string> waypoints_;  // intermediate stops (for multi-hop routes)
    vector<int> freightIds_;    // freight IDs used at each leg
    double totalCost_;
    double totalDistance_;
    int transferCount_;         // number of transfers required

public:
    Route();
    Route(const string& origin, const string& destination);
    
    // Setters
    void setOrigin(const string& origin);
    void setDestination(const string& destination);
    void addWaypoint(const string& waypoint);
    void addFreightId(int freightId);
    void setTotalCost(double cost);
    void setTotalDistance(double distance);
    void setTransferCount(int count);
    
    // Getters
    string getOrigin() const;
    string getDestination() const;
    const vector<string>& getWaypoints() const;
    const vector<int>& getFreightIds() const;
    double getTotalCost() const;
    double getTotalDistance() const;
    int getTransferCount() const;
    
    // Check if this is a direct route (no transfers)
    bool isDirect() const;
    
    // Display route information
    void display() const;
    
    // Calculate route summary
    string getSummary() const;
};
