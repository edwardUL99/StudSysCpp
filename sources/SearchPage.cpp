#include "headers/SearchPage.h"
#include "headers/studsys/Module.h"
#include "headers/studsys/Lecturer.h"
#include "headers/studsys/Student.h"
#include "headers/studsys/StudentRegistration.h"
#include <cstring>

using ui::SearchPage;
using ui::SearchableType;
using std::string;

map<SearchableType, vector<string>> SearchPage::createMap() {
    vector<string> moduleFields = {"Code", "Name"};
    vector<string> lecturerFields = {"E-mail", "Name", "Department"};
    vector<string> studentFields = {"Id", "Name", "Course (Code)"};
    vector<string> registrationFields = {"Student (Id)", "Module (Code)"};

    return {{MODULE, moduleFields},
            {LECTURER, lecturerFields},
            {STUDENT, studentFields},
            {REGISTRATION, registrationFields}
           };
}

const map<SearchableType, vector<string>> SearchPage::searchableFields = SearchPage::createMap();

SearchPage::SearchPage(StudentSystem &system) : Page(system) {}

vector<Module> SearchPage::getMatchingModules(Predicate<Module> &predicate) {
    vector<Module> matchedModules;
    vector<Module> allModules = system.getModules();

    for (const Module &module : allModules) {
        if (predicate(module)) {
            matchedModules.push_back(module);
        }
    }

    return matchedModules;
}

vector<Lecturer> SearchPage::getMatchingLecturers(Predicate<Lecturer> &predicate) {
    vector<Lecturer> matchedLecturers;
    vector<Lecturer> allLecturers = system.getLecturers();

    for (const Lecturer &lecturer : allLecturers) {
        if (predicate(lecturer)) {
            matchedLecturers.push_back(lecturer);
        }
    }

    return matchedLecturers;
}

vector<Student> SearchPage::getMatchingStudents(Predicate<Student> &predicate) {
    vector<Student> matchedStudents;
    vector<Student> allStudents = system.getStudents();

    for (const Student &student : allStudents) {
        if (predicate(student)) {
            matchedStudents.push_back(student);
        }
    }

    return matchedStudents;
}

vector<StudentRegistration> SearchPage::getMatchingStudentRegistrations(Predicate<StudentRegistration> &predicate) {
    vector<StudentRegistration> matchedRegistrations;

    for (const Module &module : system.getModules()) {
        for (const Student &student : system.getStudentsRegisteredOnModule(module)) {
            StudentRegistration registration(student, module);
            if (predicate(registration)) {
                matchedRegistrations.push_back(registration);
            }
        }
    }

    return matchedRegistrations;
}

void SearchPage::displayPossibleFieldOptions(SearchableType searchable) {
    string type = "";
    switch (searchable) {
        case MODULE: type = "Module";
                        break;
        case LECTURER: type = "Lecturer";
                        break;
        case STUDENT: type = "Student";
                        break;
        case REGISTRATION: type = "StudentRegistration";
                            break;
    }

    cout << "Choose one of the following " << type << " fields to search:" << endl;

    for (const string &field : searchableFields.at(searchable)) {
        cout << "~ " << field << endl;
    }
}

void SearchPage::displaySearchableTypes() {
    cout << "Choose from the following to search:\n~ Modules\n~ Lecturers\n~ Students\n~ StudentRegistrations" << endl;
}

void SearchPage::processModuleSearch(string chosenField, string value) {
    vector<Module> results;
    if (chosenField == "Code") {
        Predicate<Module> predicate([value](const Module &module) -> bool { return module.getCode() == value; });
        results = getMatchingModules(predicate);
    } else if (chosenField == "Name") {
        Predicate<Module> predicate([value](const Module &module) -> bool { return module.getName() == value; });
        results = getMatchingModules(predicate);
    }

    displayResults(results);
}

void SearchPage::processLecturerSearch(string chosenField, string value) {
    vector<Lecturer> results;
    if (chosenField == "E-mail") {
        Predicate<Lecturer> predicate([value](const Lecturer &lecturer) -> bool { return lecturer.getEmail() == value; });
        results = getMatchingLecturers(predicate);
    } else if (chosenField == "Name") {
        Predicate<Lecturer> predicate([value](const Lecturer &lecturer) -> bool { return lecturer.getName() == value; });
        results = getMatchingLecturers(predicate);
    } else if (chosenField == "Department") {
        Predicate<Lecturer> predicate([value](const Lecturer &lecturer) -> bool { return lecturer.getDepartment() == value; });
        results = getMatchingLecturers(predicate);
    }

    displayResults(results);
}

void SearchPage::processStudentSearch(string chosenField, string value) {
    vector<Student> results;
    if (chosenField == "Id") {
        Predicate<Student> predicate([value](const Student &student) -> bool { return student.getID() == atoi(value.c_str()); });
        results = getMatchingStudents(predicate);
    } else if (chosenField == "Name") {
        Predicate<Student> predicate([value](const Student &student) -> bool { return student.getName() == value; });
        results = getMatchingStudents(predicate);
    } else if (chosenField == "Course") {
        Predicate<Student> predicate([value](const Student &student) -> bool { return student.getCourse().getID() == value; });
        results = getMatchingStudents(predicate);
    }

    displayResults(results);
}

void SearchPage::processStudentRegistrationSearch(string chosenField, string value) {
    vector<StudentRegistration> results;
    if (chosenField == "Student") {
        Predicate<StudentRegistration> predicate([value](const StudentRegistration &registration) -> bool { return registration.getStudent().getID() == atoi(value.c_str()); });
        results = getMatchingStudentRegistrations(predicate);
    } else if (chosenField == "Module") {
        Predicate<StudentRegistration> predicate([value](const StudentRegistration &registration) -> bool { return registration.getModule().getCode() == value; });
        results = getMatchingStudentRegistrations(predicate);
    }

    displayResults(results);
}

void SearchPage::processChosenField(SearchableType searchableType) {
    vector<string> fields = searchableFields.at(searchableType);
    Predicate<string> choicePredicate([fields](const string &s) -> bool {  // for a predicate for getting user input, you must return false if matches to exit the while loop
        for (int i = 0; i < fields.size(); i++) {
            string str = fields.at(i);
            std::size_t bracket_pos = str.find("(");

            if (bracket_pos != std::string::npos) {
                str = str.substr(0, bracket_pos - 1); // if there is a hint (i.e. in brackets like Course (Code), strip it off)
            }

            if (str == s) 
                return false;
        }

        return true;
    });

    string field = ui::getString(choicePredicate, "Undefined field, please try again (case-sensitive): ");
    cout << "Enter the search value: " << endl;
    string value = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    if (searchableType == MODULE) {
        processModuleSearch(field, value);
    } else if (searchableType == LECTURER) {
        processLecturerSearch(field, value);
    } else if (searchableType == STUDENT) {
        processStudentSearch(field, value);
    } else if (searchableType == REGISTRATION) {
        processStudentRegistrationSearch(field, value);
    }
}

void SearchPage::processChosenType() {
    SearchableType type;
    string chosenType = ui::getString(Predicate<string>([](const string &s) -> bool { return s != "Modules" && s != "Lecturers" && s != "Students" && s != "StudentRegistrations"; }), "Please enter a valid type (Modules/Lecturers/Students/StudentRegistrations):");

    if (chosenType == "Modules") {
        type = MODULE;
    } else if (chosenType == "Lecturers") {
        type = LECTURER;
    } else if (chosenType == "Students") {
        type = STUDENT;
    } else if (chosenType == "StudentRegistrations") {
        type = REGISTRATION;
    }

    displayPossibleFieldOptions(type);
    processChosenField(type);
}

void SearchPage::doSearch() {
    cout << "For all input, after each list, input the field (case-sensitive) (without any terms in brackets)" << endl;
    displaySearchableTypes();
    processChosenType();
}

void SearchPage::show() {
    cout << "Welcome to the search page\n" << endl;

    bool run = true;

    while (run) {
        cout << "(S)earch, (C)ancel, (Q)uit" << endl;

        string choice = ui::getChoice();

        if (choice == "S") {
            doSearch();
        } else if (choice == "C") {
            ui::pageManager.popCurrentPage();
            run = false;
        } else if (choice == "Q") {
            ui::quit();
        }
    }
}