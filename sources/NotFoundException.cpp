#include "headers/NotFoundException.h"
#include <iostream>

using std::string;

NotFoundException::NotFoundException(string description) {
    this->description = description;
}

const char* NotFoundException::what() const throw() {
    static string s = description + " does not exist in the database";
    std::cout << s << std::endl;
    return s.c_str();
}