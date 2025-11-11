#include "ScheduleService.h"


void ScheduleService::runMatch(const vector<freight>& F, const vector<Cargo>& C) 
{
    auto computed = strategy_.match(F, C);
    scheduler_.setPlans(move(computed));
}
