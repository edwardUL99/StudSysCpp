#include "headers/Lecturer.h"

using std::string;

Lecturer::Lecturer(int id, string name, int age, string email, string department) : Person(id, name, age, email) {
    this->department = department;
}

string Lecturer::getDepartment() const {
    return department;
}

void Lecturer::setDepartment(string department) {
    this->department = department;
}

string Lecturer::getObjectType() const {
    return "Lecturer";
}

string Lecturer::getDescription() const {
    string ret = "ID: " + std::to_string(id) + ", Name: " + name + ", Age: " + std::to_string(age) + ", E-mail: " + email + ", Department: " + department;
    return ret;
}

Tables Lecturer::getTable() const {
    return LECTURERS;
}