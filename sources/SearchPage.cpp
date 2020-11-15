#include "headers/SearchPage.h"
#include "headers/studsys/Module.h"
#include "headers/studsys/Lecturer.h"
#include "headers/studsys/Student.h"
#include "headers/studsys/StudentRegistration.h"
#include <cstring>

using std::string;
using ui::SearchableType;
using ui::SearchPage;

/**
 * PROCESS OF DEFINING A NEW SEARCHABLE TYPE
 * 
 * In SearchPage.h, add an enum value for the type to SearchType enum, e.g. if the type was Exam, you could add an enum value EXAM
 * In the createMap function, then define a vector of the fields you want to search of that type.
 * A field is defined as follows: Field Name (optional-hint, Type: String/Numeric). If hint is missing, it would be just (Type: String/Numeric)
 * These fields must be able to be logically mapped to a field (and accessor method) in the type, i.e. does not make sense to have a field Phone Number if the type does not have a Phone Number field)
 * Add that vector with the enum value to the returned initializer list.
 * 
 * You should then create a method vector<Type> (where type is a class, like Exam) getMatchingXXX(Predicate<Type> &predicate). E.g vector<Exam> getMatchingExams(Predicate<Exam> &predicate)
 * This method should get all instances of that type from the database. Those objects should then be iterated to and, if predicate(object) returns true, add it to the returned list
 * 
 * Add a human readable name for the type to the predicate in processChosenType, i.e. add another && clause with s != type, e.g. s != "Exams" and in the rtry message, add the same name on the RHS of !=.
 * The name on RHS of != and the enum type defined should also be added to displaySearchableTypes following the existing format
 * 
 * A method called processXXXSearch(string field, string value) should be created. XXX is the type name, e.g. Exam
 * This method should check which field is being searched and depending on the field, e.g. Id means you should have a predicate XXX.getID() == value and then display the results of the getMatchingXXX with that predicate
 * See existing methods, for how this predicate and field checks should be created and carried out
 * 
 * You then need to add in the processChosenType and processChosenField if statements for the new type. In processChosenType you should check for the new string type, e.g. Exams and if so, assign EXAM to the searchable type value.
 * Call processChosenField with this type then. processChosenField should have an if statement for the type to call processXXXSearch, e.g. processExamSearch
 *
 * If you want to change how the new type is displayed, create a template specialization for it and define how it is displayed there. See the below specialization for StudentRegistration for reference.
 * This specialization must be directly before or after the StudentRegistration specialization to avoid compilation errors
 * 
 * See the existing examples
 */

/**
 * PROCESS OF DEFINING A NEW SEARCHABLE FIELD
 * 
 * Assuming you have a SearchableType already defined and the logic for it defined as following the above process, if you want to add an extra field to search, do the follows.
 * In the createMap method, find the vector defined for that type and add the new field following the field definition syntax and add it to the list.
 * In the appropriate processXXXSearch method for that type, add an if statement as follows:
 *      else if (chosenField == <new-field-name without brackets>)
 *      {
 *      }
 * In that if block, you will want to define a Predicate for that type on matching the new field with the given search value.
 * Call getMatchingXXX method with that predicate and assign the result to results
 * 
 * The associated type must have a field and an accessing method related to the field name in order to map logically. E.g. if you have a field Code, it is assumed the type should have a field called something like code and then getCode(), e.g with module
 * While you could map any field name to an associated method name in the predicate, it may not make logical sense. Why search for a field called phone number and match it to the field Id in Student? Make sure the fields you define make sense
 * 
 * See existing code for reference
 */

map<SearchableType, vector<string>> SearchPage::createMap()
{
    vector<string> moduleFields = {"Code (Type: String)", "Name (Type: String)", "Lecturer (Name, Type: String)",};
    vector<string> lecturerFields = {"E-mail (Type: String)", "Name (Type: String)", "Department (Type: String)"};
    vector<string> studentFields = {"Id (Type: Numeric)", "Name (Type: String)", "Course (Code, Type: String)"};
    vector<string> registrationFields = {"Student (Id, Type: Numeric)", "Module (Code, Type: String)"};

    return {{MODULE, moduleFields},
            {LECTURER, lecturerFields},
            {STUDENT, studentFields},
            {REGISTRATION, registrationFields}};
}

const map<SearchableType, vector<string>> SearchPage::searchableFields = SearchPage::createMap();

SearchPage::SearchPage(StudentSystem &system) : Page(system) {}

// need to have the specialization here before any call to displayResults is made, or else a compilation error occurs

/**
 * A specialisation for StudentRegistration as you don't want to display whole description of student and module
 */
template <>
void SearchPage::displayResults<StudentRegistration>(vector<StudentRegistration> results)
{
    cout << "Search Results: " << endl;

    if (results.size() > 0)
    {
        int i = 1;
        for (StudentRegistration result : results)
        {
            Student student = result.getStudent();
            Module module = result.getModule();
            cout << i++ << ") Student ID: " << student.getID() << " Name: " << student.getName() << " ; Module Code: " << module.getCode() << " Name: " << module.getName() << endl;
        }

        cout << (i - 1) << " Search Results" << endl;
    }
    else
    {
        cout << "\tNo results to display" << endl;
    }
}

vector<Module> SearchPage::getMatchingModules(Predicate<Module> &predicate)
{
    vector<Module> matchedModules;
    vector<Module> allModules = system.getModules();

    for (const Module &module : allModules)
    {
        if (predicate(module))
        {
            matchedModules.push_back(module);
        }
    }

    return matchedModules;
}

vector<Lecturer> SearchPage::getMatchingLecturers(Predicate<Lecturer> &predicate)
{
    vector<Lecturer> matchedLecturers;
    vector<Lecturer> allLecturers = system.getLecturers();

    for (const Lecturer &lecturer : allLecturers)
    {
        if (predicate(lecturer))
        {
            matchedLecturers.push_back(lecturer);
        }
    }

    return matchedLecturers;
}

vector<Student> SearchPage::getMatchingStudents(Predicate<Student> &predicate)
{
    vector<Student> matchedStudents;
    vector<Student> allStudents = system.getStudents();

    for (const Student &student : allStudents)
    {
        if (predicate(student))
        {
            matchedStudents.push_back(student);
        }
    }

    return matchedStudents;
}

vector<StudentRegistration> SearchPage::getMatchingStudentRegistrations(Predicate<StudentRegistration> &predicate)
{
    vector<StudentRegistration> matchedRegistrations;

    for (const Module &module : system.getModules())
    {
        for (const Student &student : system.getStudentsRegisteredOnModule(module))
        {
            StudentRegistration registration(student, module);
            if (predicate(registration))
            {
                matchedRegistrations.push_back(registration);
            }
        }
    }

    return matchedRegistrations;
}

void SearchPage::displayPossibleFieldOptions(SearchableType searchable)
{
    string type = "";
    switch (searchable)
    {
    case MODULE:
        type = "Module";
        break;
    case LECTURER:
        type = "Lecturer";
        break;
    case STUDENT:
        type = "Student";
        break;
    case REGISTRATION:
        type = "StudentRegistration";
        break;
    }

    cout << "Choose one of the following " << type << " fields to search:" << endl;

    for (const string &field : searchableFields.at(searchable))
    {
        cout << "~ " << field << endl;
    }

    cout << "~ All (List all " << type << "s in system)" << endl;
}

void SearchPage::displaySearchableTypes()
{
    cout << "Choose from the following to search:\n~ Modules\n~ Lecturers\n~ Students\n~ StudentRegistrations" << endl;
}

void SearchPage::processModuleSearch(string chosenField, string value)
{
    vector<Module> results;
    if (chosenField == "Code")
    {
        Predicate<Module> predicate([value](const Module &module) -> bool { return module.getCode() == value; });
        results = getMatchingModules(predicate);
    }
    else if (chosenField == "Name")
    {
        Predicate<Module> predicate([value](const Module &module) -> bool { return module.getName() == value; });
        results = getMatchingModules(predicate);
    }
    else if (chosenField == "Lecturer")
    {
        Predicate<Module> predicate([value](const Module &module) -> bool { return module.getLecturer().getName() == value; });
        results = getMatchingModules(predicate);
    } else if (chosenField == "All")
    {
        results = system.getModules(); // no need to iterate through each object if there is already a method to do it
    }

    displayResults(results);
}

void SearchPage::processLecturerSearch(string chosenField, string value)
{
    vector<Lecturer> results;
    if (chosenField == "E-mail")
    {
        Predicate<Lecturer> predicate([value](const Lecturer &lecturer) -> bool { return lecturer.getEmail() == value; });
        results = getMatchingLecturers(predicate);
    }
    else if (chosenField == "Name")
    {
        Predicate<Lecturer> predicate([value](const Lecturer &lecturer) -> bool { return lecturer.getName() == value; });
        results = getMatchingLecturers(predicate);
    }
    else if (chosenField == "Department")
    {
        Predicate<Lecturer> predicate([value](const Lecturer &lecturer) -> bool { return lecturer.getDepartment() == value; });
        results = getMatchingLecturers(predicate);
    } else if (chosenField == "All") 
    {
        // StudentSystem has a method getLecturers but I need a predicate to remove the Sentinel NOT FOUND lecturer from results
        Predicate<Lecturer> predicate([](const Lecturer &lecturer) -> bool { return lecturer.getEmail() != Lecturer::NOT_FOUND.getEmail(); });
        results = getMatchingLecturers(predicate);
    }

    displayResults(results);
}

void SearchPage::processStudentSearch(string chosenField, string value)
{
    vector<Student> results;
    if (chosenField == "Id")
    {
        Predicate<Student> predicate([value](const Student &student) -> bool { return student.getID() == atoi(value.c_str()); });
        results = getMatchingStudents(predicate);
    }
    else if (chosenField == "Name")
    {
        Predicate<Student> predicate([value](const Student &student) -> bool { return student.getName() == value; });
        results = getMatchingStudents(predicate);
    }
    else if (chosenField == "Course")
    {
        Predicate<Student> predicate([value](const Student &student) -> bool { return student.getCourse().getID() == value; });
        results = getMatchingStudents(predicate);
    } else if (chosenField == "All")
    {
        results = system.getStudents(); // no need to iterate through each object if there is already a method to do it
    }

    displayResults(results);
}

void SearchPage::processStudentRegistrationSearch(string chosenField, string value)
{
    vector<StudentRegistration> results;
    if (chosenField == "Student")
    {
        Predicate<StudentRegistration> predicate([value](const StudentRegistration &registration) -> bool { return registration.getStudent().getID() == atoi(value.c_str()); });
        results = getMatchingStudentRegistrations(predicate);
    }
    else if (chosenField == "Module")
    {
        Predicate<StudentRegistration> predicate([value](const StudentRegistration &registration) -> bool { return registration.getModule().getCode() == value; });
        results = getMatchingStudentRegistrations(predicate);
    } else if (chosenField == "All")
    {
        // need to pass in a predicate that is always true here as there is no method in StudentSystem to do this for us
        Predicate<StudentRegistration> predicate([](const StudentRegistration &registration) -> bool { return true; }); // no need to capture value here
        results = getMatchingStudentRegistrations(predicate);
    }

    displayResults(results);
}

void SearchPage::processChosenField(SearchableType searchableType)
{
    vector<string> fields = searchableFields.at(searchableType);
    Predicate<string> choicePredicate([fields](const string &s) -> bool { // for a predicate for getting user input, you must return false if matches to exit the while loop
        for (int i = 0; i < fields.size(); i++)
        {
            string str = fields.at(i);
            std::size_t bracket_pos = str.find("(");

            if (bracket_pos != std::string::npos)
            {
                str = str.substr(0, bracket_pos - 1); // if there is a hint (i.e. in brackets like Course (Code), strip it off)
            }

            if (str == s || s == "All")
                return false;
        }

        return true;
    });

    string field = ui::getString(choicePredicate, "Undefined field, please try again (case-sensitive): ");
    string value = "";

    if (field != "All")
    {
        cout << "Enter the search value: " << endl;
        value = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);
    }

    if (searchableType == MODULE)
    {
        processModuleSearch(field, value);
    }
    else if (searchableType == LECTURER)
    {
        processLecturerSearch(field, value);
    }
    else if (searchableType == STUDENT)
    {
        processStudentSearch(field, value);
    }
    else if (searchableType == REGISTRATION)
    {
        processStudentRegistrationSearch(field, value);
    }
}

void SearchPage::processChosenType()
{
    SearchableType type;
    string chosenType = ui::getString(Predicate<string>([](const string &s) -> bool { return s != "Modules" && s != "Lecturers" && s != "Students" && s != "StudentRegistrations"; }), "Please enter a valid type (Modules/Lecturers/Students/StudentRegistrations):");

    if (chosenType == "Modules")
    {
        type = MODULE;
    }
    else if (chosenType == "Lecturers")
    {
        type = LECTURER;
    }
    else if (chosenType == "Students")
    {
        type = STUDENT;
    }
    else if (chosenType == "StudentRegistrations")
    {
        type = REGISTRATION;
    }

    displayPossibleFieldOptions(type);
    processChosenField(type);
}

void SearchPage::doSearch()
{
    cout << "For all input, after each list, input the field (case-sensitive) (without any terms in brackets)" << endl;
    displaySearchableTypes();
    processChosenType();
}

void SearchPage::printHelp()
{
    cout << "Search Help: " << endl;
    cout << "\tYou will be first presented with a list of the objects in the system you can search" << endl;
    cout << "\tAfter these are listed, type in the full name (case-sensitive) and press enter" << endl;
    cout << "\tYou will then be presented with a list of the fields that can be searched" << endl;
    cout << "\tThese fields may have a hint on what to be entered, all have the type of the value to be entered for them (except All)" << endl;
    cout << "\tAll's hint is not input value, just advising using All will list all objects of that type in the system" << endl;
    cout << "\tEnter the field name, case-sensitive, without spaces at the end and no brackets" << endl;
    cout << "\tAfter entering the field name, (asides from all), you will be asked to enter the search value. Ensure the type entered matches the expected type" << endl;
    cout << "\tYou will then be presented with the results, if any" << endl;
}

void SearchPage::show()
{
    cout << "Welcome to the search page\n"
         << endl;

    bool run = true;

    while (run)
    {
        cout << "(S)earch, (H)elp, (C)ancel, (Q)uit" << endl;

        string choice = ui::getChoice();

        if (choice == "S")
        {
            doSearch();
        }
        else if (choice == "H")
        {
            printHelp();
        }
        else if (choice == "C")
        {
            ui::pageManager.popCurrentPage();
            run = false;
        }
        else if (choice == "Q")
        {
            ui::quit();
        }
    }
}