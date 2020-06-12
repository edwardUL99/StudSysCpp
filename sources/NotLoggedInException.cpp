#include "headers/NotLoggedInException.h"

using std::string;

NotLoggedInException::NotLoggedInException(string message) {
    this->message = message;
}

const char* NotLoggedInException::what() const throw() {
    return this->message.c_str();
}