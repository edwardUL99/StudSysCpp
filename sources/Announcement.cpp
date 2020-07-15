#include "headers/Announcement.h"

using std::string;

Announcement::Announcement(const Module &module, const Lecturer &lecturer, std::string announcement) : module(module), lecturer(lecturer) {
    this->announcement = announcement;
}

Module Announcement::getModule() const {
    return module;
}

void Announcement::setModule(const Module &module) {
    this->module = module;
}

Lecturer Announcement::getLecturer() const {
    return lecturer;
}

void Announcement::setLecturer(const Lecturer &lecturer) {
    this->lecturer = lecturer;
}

string Announcement::getAnnouncementText() const {
    return announcement;
}

void Announcement::setAnnouncementText(string announcement) {
    this->announcement = announcement;
}

string Announcement::getObjectType() const {
    return "Announcement";
}

Tables Announcement::getTable() const {
    return ANNOUNCEMENTS;
}

string Announcement::getDescription() const {
    return "Module: " + module.getDescription() + ", Lecturer: " + lecturer.getDescription() + ", Announcement: " + announcement;
}