#include "headers/NotFoundException.h"

using std::string;

NotFoundException::NotFoundException(string description) {
    this->description = description;
}

const char* NotFoundException::what() const throw() {
    string s = description + " does not exist in the database";
    return s.c_str();
}