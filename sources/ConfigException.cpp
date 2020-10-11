#include "headers/ConfigException.h"

ConfigException::ConfigException(std::string message) {
    this->message = message;
}

const char * ConfigException::what() const throw() {
    return message.c_str();
}