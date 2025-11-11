#include "functions.h"
#include <cstdio>
#include <cctype>
#include <sstream>
using namespace std;

int askInt(const string& prompt)
{
    cout << prompt;
    int v;

    // Input validation loop
    while (!(cin >> v)) {
        cin.clear(); // clear error 
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // remove bad input
        cout << "Invalid number. Try again: ";
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // newline
    return v;
}

string askLine(const string& prompt) 
{
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}



bool isValidDestination(const string& dest)
{
    for (char c : dest)
    {
        if (isdigit(static_cast<unsigned char>(c)))  // numeric character found
            return false;
    }
    return !dest.empty(); // must not be empty
}

// convert integer time (e.g., 930 or 1430) to 12-hour AM/PM format
string formatTime12h(int timeVal) 
{
    int hour = timeVal / 100;
    int minute = timeVal % 100;

    // validate that time is within 0000â2359
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59)
        return "Invalid time";

    string ampm = (hour >= 12) ? "PM" : "AM";
    if (hour == 0) hour = 12;
    else if (hour > 12) hour -= 12;

    // Format as "H:MM AM"
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%d:%02d %s", hour, minute, ampm.c_str());
    return string(buffer);
}



bool isValidTime(int t)
{
    if (t < 0 || t > 2359) return false;
    int minutes = t % 100;
    int hours = t / 100;
    if (minutes >= 60) return false;
    if (hours >= 24) return false;
    return true;
}

int parseTime(const string& s)
{
    if (s.empty()) return -1;
    // trim
    size_t i = 0, j = s.size();
    while (i < j && isspace(static_cast<unsigned char>(s[i]))) ++i;
    while (j > i && isspace(static_cast<unsigned char>(s[j-1]))) --j;
    if (i >= j) return -1;
    string t = s.substr(i, j - i);

    // check for presence of am/pm
    string lower;
    lower.reserve(t.size());
    for (char c : t) lower.push_back(std::tolower(static_cast<unsigned char>(c)));

    bool hasAm = false, hasPm = false;
    if (lower.size() >= 2) 
    {
        if (lower.find("am") != string::npos) hasAm = true;
        if (lower.find("pm") != string::npos) hasPm = true;
    }

    int hour = 0, minute = 0;

    // remove am/pm from the string for parsing numbers
    if (hasAm || hasPm)
    {
        // find where am/pm starts
        size_t pos = lower.find("am");
        if (pos == string::npos) pos = lower.find("pm");
        string core = t.substr(0, pos);
        // trim trailing spaces and punctuation
        size_t a = 0, b = core.size();
        while (a < b && isspace(static_cast<unsigned char>(core[a]))) ++a;
        while (b > a && isspace(static_cast<unsigned char>(core[b-1]))) --b;
        core = core.substr(a, b-a);

        // parse core as H:MM or HMM
        size_t colon = core.find(':');
        try 
        {
            if (colon != string::npos) 
            {
                hour = stoi(core.substr(0, colon));
                minute = stoi(core.substr(colon + 1));
            } 
            else
                {
                int val = stoi(core);
                if (val < 0) return -1;
                if (val <= 99)
                { 
                    return -1;
                }
                hour = val / 100;
                minute = val % 100;
            }
        } 
            catch (...) 
            {
            return -1;
             }

        if (hour < 1 || hour > 12 || minute < 0 || minute > 59) return -1;
        if (hasPm)
        {
            if (hour != 12) hour += 12;
        } else 
        { 
            if (hour == 12) hour = 0; //AM
        }
    } else 
    {
        // no am/pm present; accept H:MM or HHMM or HMM
        size_t colon = t.find(':');
        try 
            {
            if (colon != string::npos) 
                {
                hour = stoi(t.substr(0, colon));
                minute = stoi(t.substr(colon + 1));
            } 
            else
            {
                int val = stoi(t);
                hour = val / 100;
                minute = val % 100;
            }
        } catch (...) 
        {
            return -1;
        }
        if (hour < 0 || hour > 23 || minute < 0 || minute > 59) return -1;
    }

    return hour * 100 + minute;
}
