#include "headers/KeyMismatch.h"

using std::string;

KeyMismatch::KeyMismatch(string newKey, string oldKey) {
    this->newKey = newKey;
    this->oldKey = oldKey;
}

const char* KeyMismatch::what() const throw() {
    string s = "Key " + newKey + " does not match Key " + oldKey;
    return s.c_str();
}