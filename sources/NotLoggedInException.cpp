#include "headers/NotLoggedInException.h"

using std::string;

NotLoggedInException::NotLoggedInException(string message) {
    this->message = message;
}

const char* NotLoggedInException::what() const throw() {
    static string s = this->message;
    return s.c_str();
}