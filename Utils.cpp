// Utils.cpp
// Purpose: Implementation of the shared string helper functions.
// Author: Bobby Oliver

#include "Utils.h"

// Removes whitespace (space, tab, \r, \n, etc.) from both ends of s.
std::string trim(const std::string& s) {
    const std::string whitespace = " \t\r\n\f\v";
    std::size_t start = s.find_first_not_of(whitespace);
    if (start == std::string::npos) {
        return "";   // the whole string was whitespace
    }
    std::size_t end = s.find_last_not_of(whitespace);
    return s.substr(start, end - start + 1);
}

// Returns a copy of s with every a-z letter changed to A-Z.
std::string toUpper(const std::string& s) {
    std::string result = s;
    for (std::size_t i = 0; i < result.size(); i++) {
        if (result[i] >= 'a' && result[i] <= 'z') {
            result[i] = (char)(result[i] - 'a' + 'A');
        }
    }
    return result;
}

// Returns a copy of s with every A-Z letter changed to a-z.
std::string toLower(const std::string& s) {
    std::string result = s;
    for (std::size_t i = 0; i < result.size(); i++) {
        if (result[i] >= 'A' && result[i] <= 'Z') {
            result[i] = (char)(result[i] - 'A' + 'a');
        }
    }
    return result;
}

// Compares two strings ignoring capitalization.
bool equalsIgnoreCase(const std::string& a, const std::string& b) {
    return toUpper(a) == toUpper(b);
}

// Splits line on '|'. Empty fields are kept so the field count is correct.
std::vector<std::string> splitFields(const std::string& line) {
    std::vector<std::string> fields;
    std::string current = "";
    for (std::size_t i = 0; i < line.size(); i++) {
        if (line[i] == '|') {
            fields.push_back(current);
            current = "";
        } else {
            current += line[i];
        }
    }
    fields.push_back(current);   // the last field (after the final |)
    return fields;
}

// A line is skipped if it is blank or the first real character is #.
// A # anywhere else in the line is just normal text.
bool isBlankOrComment(const std::string& line) {
    std::string t = trim(line);
    if (t.empty()) {
        return true;
    }
    return t[0] == '#';
}

// Checks for the pattern F### (F and exactly three digits).
bool isValidFacilityId(const std::string& id) {
    if (id.size() != 4) {
        return false;
    }
    if (id[0] != 'F') {
        return false;
    }
    for (int i = 1; i < 4; i++) {
        if (id[i] < '0' || id[i] > '9') {
            return false;
        }
    }
    return true;
}

// Builds a path like data/facilities.txt from a folder and a file name.
std::string joinPath(const std::string& dir, const std::string& file) {
    if (dir.empty()) {
        return file;
    }
    if (dir[dir.size() - 1] == '/') {
        return dir + file;
    }
    return dir + "/" + file;
}
