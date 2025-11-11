#include "SchedulerMenuHandler.h"
#include "functions.h"
#include "ScheduleService.h"
#include "MatchingEngine.h"
#include "SchedulePresenter.h"
#include "ScheduleAnalyzer.h"
#include "GroupingStrategy.h"
#include "PlanRepository.h"
#include "ScheduleBuilder.h"
#include <iostream>
#include <limits>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

using namespace std;

static string promptLine(const string& prompt)
{
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

static void clearInput() 
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

SchedulerMenuHandler::SchedulerMenuHandler(freightlist& fList, CargoList& cList, Schduler& sched)
    : freightList(fList), cargoList(cList), scheduler(sched), scheduleService(matchStrategy)
{
}

void SchedulerMenuHandler::printMenu() 
{
    cout << "\n=== Scheduler Menu ===\n"
        << " 1) Create Schedule - By Cargo Arrival Time\n"
        << " 2) Create Schedule - Minimize Freights (Maximize Load)\n"
        << " 3) Save scheduling plan to file\n"
        << " 4) Display scheduling plan sorted by cargo arrival time\n"
        << " 5) Display scheduling plan sorted by freight load capacity\n"
        << " 6) Display freights not at full capacity\n"
        << " 7) Display cargos not yet assigned to any freight\n"
        << " 0) Return to main menu\n"
        << "Choice: ";
}

void SchedulerMenuHandler::run() 
{
    while (true) 
    {
        printMenu();
        int choice{};
        cin >> choice;
        clearInput();

        switch (choice) 
        {
        case 1: 
        {
            // create schedule by cargo arrival time
            PerfectArrivalGrouping strategy;
            lastGroupsArrival = strategy.group(freightList.all(), cargoList.all());

            // convert to shipments using ScheduleBuilder (arrival)
            lastShipmentsArrival = ScheduleBuilder::buildFromArrival(lastGroupsArrival, freightList.all(), cargoList.all());

            cout << "\n=== Scheduling: By Cargo Arrival Time ===\n";
            cout << "Schedule created successfully!\n";

            // count unique freights used
            unordered_set<int> uniqueFreights;
            for (const auto& s : lastShipmentsArrival) {
                uniqueFreights.insert(s.getFreightIndex());
            }
            cout << "Total freights used: " << uniqueFreights.size() << "\n";

            int totalAssigned = 0;
            for (const auto& s : lastShipmentsArrival) {
                totalAssigned += s.getCargoQuantity();
            }
            cout << "Total cargo items assigned: " << totalAssigned << "\n";
            break;
        }

        case 2: {
            // create Schedule to minimize freights
            LeastFreightsGrouping strategy;
            lastGroupsLeast = strategy.group(freightList.all(), cargoList.all());

            // convert to shipments using ScheduleBuilder (least freights)
            lastShipmentsLeast = ScheduleBuilder::buildFromLeastFreights(lastGroupsLeast, freightList.all(), cargoList.all());

            cout << "\n=== Scheduling: Minimize Number of Freights ===\n";
            cout << "Schedule created successfully!\n";

            // count unique freights used
            unordered_set<int> uniqueFreights;
            for (const auto& s : lastShipmentsLeast) {
                uniqueFreights.insert(s.getFreightIndex());
            }
            cout << "Total freights used: " << uniqueFreights.size() << "\n";

            int totalAssigned = 0;
            for (const auto& s : lastShipmentsLeast) {
                totalAssigned += s.getCargoQuantity();
            }
            cout << "Total cargo items assigned: " << totalAssigned << "\n";
            break;
        }

        case 3: {
            // save scheduling plan
            cout << "Which schedule to save?\n";
            cout << "1) Cargo Arrival Time Schedule\n";
            cout << "2) Minimize Freights Schedule\n";
            cout << "Choice: ";
            int saveChoice;
            cin >> saveChoice;
            clearInput();

            string path = promptLine("Enter output filename (.txt): ");

            if (saveChoice == 1 && !lastGroupsArrival.empty()) {
                if (PlanRepository::saveGrouping(lastGroupsArrival, freightList.all(), cargoList.all(), path)) {
                    cout << "Saved Cargo Arrival schedule to " << path << '\n';
                }
                else {
                    cout << "Failed to save to " << path << '\n';
                }
            }
            else if (saveChoice == 2 && !lastGroupsLeast.empty()) {
                if (PlanRepository::saveGrouping(lastGroupsLeast, freightList.all(), cargoList.all(), path)) {
                    cout << "Saved Minimize Freights schedule to " << path << '\n';
                }
                else {
                    cout << "Failed to save to " << path << '\n';
                }
            }
            else {
                cout << "No schedule available or invalid choice. Create a schedule first.\n";
            }
            break;
        }

        case 4: {
            // display by Cargo Arrival Time
            if (lastShipmentsArrival.empty()) {
                cout << "No cargo arrival schedule created yet. Run option 1 first.\n";
            }
            else {
                SchedulePresenter::displayGroupingByArrival(
                    cout,
                    lastShipmentsArrival,
                    freightList.all(),
                    cargoList.all());
            }
            break;
        }

        case 5: {
            // display by freight Load Capacity (min Freights) 
            if (lastShipmentsLeast.empty()) {
                cout << "No minimize-freights schedule created yet. Run option 2 first.\n";
            }
            else {
                SchedulePresenter::displayGroupingByLeastFreights(
                    cout,
                    lastShipmentsLeast,
                    freightList.all(),
                    cargoList.all());
            }
            break;
        }

        case 6: {
            // display freights not at full capacity
            cout << "Which schedule to analyze?\n";
            cout << "1) Cargo Arrival Time Schedule\n";
            cout << "2) Minimize Freights Schedule\n";
            cout << "Choice: ";
            int analyzeChoice;
            cin >> analyzeChoice;
            clearInput();

            unordered_map<int, vector<int>>* selectedGroups = nullptr;
            if (analyzeChoice == 1 && !lastGroupsArrival.empty()) {
                selectedGroups = &lastGroupsArrival;
            }
            else if (analyzeChoice == 2 && !lastGroupsLeast.empty()) {
                selectedGroups = &lastGroupsLeast;
            }

            if (!selectedGroups) {
                cout << "No schedule available. Create a schedule first.\n";
                break;
            }

            auto freeFreights = ScheduleAnalyzer::freightsWithSpaceLeft(*selectedGroups, freightList.all(), cargoList.all());
            cout << "\nFreights with available space:\n";
            if (freeFreights.empty()) {
                cout << "(none - all freights at full capacity)\n";
            }
            else {
                for (int fid : freeFreights) {
                    auto pf = freightList.findById(fid);
                    if (pf) {
                        int used = 0;
                        auto git = selectedGroups->find(fid);
                        if (git != selectedGroups->end()) {
                            for (int cid : git->second) {
                                auto pc = cargoList.findById(cid);
                                if (pc) used += pc->getCapacity(); else used += 1;
                            }
                        }
                        int remaining = pf->getCapacity() - used;
                        cout << "  F" << pf->getIndex() << " (" << pf->getTypeName() << ")"
                            << " Dest: " << pf->getDestination()
                            << " Time: " << formatTime12h(pf->getTime())
                            << " | Capacity: " << used << "/" << pf->getCapacity()
                            << " (Space left: " << remaining << ")\n";
                    }
                }
            }
            break;
        }

        case 7: {
            // display unassigned cargos
            cout << "Which schedule to analyze?\n";
            cout << "1) Cargo Arrival Time Schedule\n";
            cout << "2) Minimize Freights Schedule\n";
            cout << "Choice: ";
            int analyzeChoice;
            cin >> analyzeChoice;
            clearInput();

            unordered_map<int, vector<int>>* selectedGroups = nullptr;
            if (analyzeChoice == 1 && !lastGroupsArrival.empty()) {
                selectedGroups = &lastGroupsArrival;
            }
            else if (analyzeChoice == 2 && !lastGroupsLeast.empty()) {
                selectedGroups = &lastGroupsLeast;
            }

            if (!selectedGroups) {
                cout << "No schedule available. Create a schedule first.\n";
                break;
            }

            auto unassigned = ScheduleAnalyzer::unassignedCargoIdsFromGroups(*selectedGroups, cargoList.all());
            cout << "\nUnassigned cargos:\n";
            if (unassigned.empty()) {
                cout << "(none - all cargos assigned to freights)\n";
            }
            else {
                for (int cid : unassigned) {
                    auto it = cargoList.findById(cid);
                    if (it) {
                        cout << "  C" << it->getIndex()
                            << " Dest: " << it->getDestination()
                            << " Time: " << formatTime12h(it->getTime())
                            << " Capacity: " << it->getCapacity() << '\n';
                    }
                }
            }
            break;
        }

        case 0:
            return; // return to main menu

        default:
            cout << "Invalid choice.\n";
            break;
        }
    }
}