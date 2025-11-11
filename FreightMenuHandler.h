#pragma once

#include "MenuHandler.h"
#include "freightlist.h"


class FreightMenuHandler : public MenuHandler 
{
private:
    freightlist& freightList;
    
    void printMenu();
    
public:
    explicit FreightMenuHandler(freightlist& fList);
    void run() override;
};
