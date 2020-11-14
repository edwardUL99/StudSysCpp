#ifndef SEARCH_PAGE_H
#define SEARCH_PAGE_H

#include "Page.h"
#include <map>
#include <vector>
#include <string>

using std::map;
using std::string;
using std::vector;

class Module;
class Lecturer;
class Student;
class StudentRegistration;

namespace ui
{
    /**
      * Represents the types that search is currently implemented for
      */
    enum SearchableType
    {
        MODULE,
        LECTURER,
        STUDENT,
        REGISTRATION
    };

    /**
     * This class provides a page for searching the system
     */
    class SearchPage : public Page
    {
    private:
        /**
             * A map of type and list of fields that can be searched
             */
        const static map<SearchableType, vector<string>> searchableFields;
        /**
             * Creates the map for searchableFields
             */
        static map<SearchableType, vector<string>> createMap();
        /**
             * Retrieves all modules matching the given predicate
             */
        vector<Module> getMatchingModules(Predicate<Module> &predicate);
        /**
             * Retrieves all lecturers matching the given predicate
             */
        vector<Lecturer> getMatchingLecturers(Predicate<Lecturer> &predicate);
        /**
             * Retrieves all students matching the given predicate
             */
        vector<Student> getMatchingStudents(Predicate<Student> &predicate);
        /**
             * Retrieves all student registrations matching the given predicate
             */
        vector<StudentRegistration> getMatchingStudentRegistrations(Predicate<StudentRegistration> &predicate);
        /**
             * Displays the possible fields to search for the given type
             */
        void displayPossibleFieldOptions(SearchableType searchable);
        /**
             * Displays a list of the objects that can be searched
             */
        void displaySearchableTypes();
        /**
         * Processes the search for module
         */
        void processModuleSearch(std::string chosenField, std::string value);
        /**
         * Processes the search for lecturer
         */
        void processLecturerSearch(std::string chosenField, std::string value);
        /**
         * Processes the search for student
         */
        void processStudentSearch(std::string chosenField, std::string value);
        /**
         * Processes the search for registrations
         */
        void processStudentRegistrationSearch(std::string chosenField, std::string value);
        /**
             * Processes the chosen field and validates if it is an acceptable field of the given searchable type.
             * Then creates the appropriate predicate and calls the appropriate getMatchingXXX method
             */
        void processChosenField(SearchableType searchableType);
        /**
             * Gets type from user and validates it and then does some processing with it
             */
        void processChosenType();
        /**
             * Displays the results of the search
             */
        template <class T>
        void displayResults(vector<T> results)
        {
          cout << "Search Results: " << endl;

          if (results.size() > 0) {
               int i = 1;
               for (T result : results)
               {
                    cout << i++ << ") " << result.getDescription() << endl;
               }
          } else {
               cout << "\tNo results to display" << endl;
          }
        }
        /**
             * Begins the search process
             */
        void doSearch();

    public:
        SearchPage(StudentSystem &system);
        virtual ~SearchPage() = default;
        virtual void show() override;
    };
} // namespace ui

#endif