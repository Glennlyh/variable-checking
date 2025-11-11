#pragma once

// abstract base class for menu handler
class MenuHandler 
{
public:
    virtual ~MenuHandler() = default;
    virtual void run() = 0;
protected:
    MenuHandler() = default;
};
