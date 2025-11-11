#include "Route.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

Route::Route() 
    : origin_(""), destination_(""), totalCost_(0.0), totalDistance_(0.0), transferCount_(0)
{
}

Route::Route(const string& origin, const string& destination)
    : origin_(origin), destination_(destination), totalCost_(0.0), totalDistance_(0.0), transferCount_(0)
{
}

void Route::setOrigin(const string& origin) 
{
    origin_ = origin;
}

void Route::setDestination(const string& destination) 
{
    destination_ = destination;
}

void Route::addWaypoint(const string& waypoint) 
{
    waypoints_.push_back(waypoint);
}

void Route::addFreightId(int freightId) 
{
    freightIds_.push_back(freightId);
}

void Route::setTotalCost(double cost) 
{
    totalCost_ = cost;
}

void Route::setTotalDistance(double distance) 
{
    totalDistance_ = distance;
}

void Route::setTransferCount(int count) 
{
    transferCount_ = count;
}

string Route::getOrigin() const 
{
    return origin_;
}

string Route::getDestination() const 
{
    return destination_;
}

const vector<string>& Route::getWaypoints() const 
{
    return waypoints_;
}

const vector<int>& Route::getFreightIds() const 
{
    return freightIds_;
}

double Route::getTotalCost() const 
{
    return totalCost_;
}

double Route::getTotalDistance() const 
{
    return totalDistance_;
}

int Route::getTransferCount() const 
{
    return transferCount_;
}

bool Route::isDirect() const 
{
    return waypoints_.empty();
}

void Route::display() const 
{
    cout << "\n--- Route Details ---\n";
    cout << "Origin: " << origin_ << "\n";
    cout << "Destination: " << destination_ << "\n";
    
    if (isDirect()) 
    {
        cout << "Type: Direct Route\n";
    } 
    else 
    {
        cout << "Type: Multi-hop Route (" << transferCount_ << " transfer(s))\n";
        cout << "Waypoints: ";
        for (size_t i = 0; i < waypoints_.size(); i++) 
        {
            cout << waypoints_[i];
            if (i < waypoints_.size() - 1) cout << " -> ";
        }
        cout << "\n";
    }
    
    cout << "Freights used: ";
    for (size_t i = 0; i < freightIds_.size(); i++) 
    {
        cout << "F" << freightIds_[i];
        if (i < freightIds_.size() - 1) cout << " -> ";
    }
    cout << "\n";
    
    cout << fixed << setprecision(2);
    cout << "Total Distance: " << totalDistance_ << " km\n";
    cout << "Total Cost: " << totalCost_ << " units\n";
}

string Route::getSummary() const 
{
    stringstream ss;
    ss << origin_;
    for (const auto& waypoint : waypoints_) 
    {
        ss << " -> " << waypoint;
    }
    ss << " -> " << destination_;
    return ss.str();
}
