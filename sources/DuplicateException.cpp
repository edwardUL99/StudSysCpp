#include "headers/DuplicateException.h"
#include <string>

DuplicateException::DuplicateException(std::string description) : entityDescription(description) {
}

const char *DuplicateException::what() const throw() {
    std::string s = entityDescription + " already exists in the database";
    return s.c_str();
}