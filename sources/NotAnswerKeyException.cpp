#include "headers/NotAnswerKeyException.h"

NotAnswerKeyException::NotAnswerKeyException(std::string message) {
    this->message = message;
}

const char *NotAnswerKeyException::what() const throw() {
    std::string ret = message + " is not an answer key";
    return ret.c_str();
}