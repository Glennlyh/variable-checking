#pragma once

#include "freight.h"

// Observer interface for freight changes
class FreightObserver 
{
public:
    virtual ~FreightObserver() = default;
    
    // Called when a freight is about to be removed
    virtual void onFreightRemoved(const freight& f) = 0;
};
