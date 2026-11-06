// Utils.h
// Purpose: Small string helper functions shared by all the modules
//          (trim, upper/lower case, split on '|', ID format check).
// Author: Bobby Oliver

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

// Removes spaces, tabs, \r and \n from both ends. Spaces inside are kept.
std::string trim(const std::string& s);

// Turns the letters a-z into A-Z (ASCII only is fine for this project).
std::string toUpper(const std::string& s);

// Turns the letters A-Z into a-z.
std::string toLower(const std::string& s);

// True if the two strings match when you ignore capitalization.
bool equalsIgnoreCase(const std::string& a, const std::string& b);

// Splits a line on '|' and keeps empty fields (so "a||b" gives 3 fields).
std::vector<std::string> splitFields(const std::string& line);

// True if the line is blank or its first non-whitespace character is #.
bool isBlankOrComment(const std::string& line);

// True if id is F followed by exactly three digits. The id should already
// be trimmed and uppercase before you call this.
bool isValidFacilityId(const std::string& id);

// Puts a folder and a file name together, adding a / if needed.
std::string joinPath(const std::string& dir, const std::string& file);

#endif
