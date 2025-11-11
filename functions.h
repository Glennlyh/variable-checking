#pragma once

#include <iostream>
#include <string>
#include <limits>

int askInt(const std::string& prompt);
std::string askLine(const std::string& prompt);

bool isValidDestination(const std::string& dest);
std::string formatTime12h(int timeVal);
bool isValidTime(int t);

int parseTime(const std::string& s);


