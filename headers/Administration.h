#ifndef ADMINISTRATION_H
#define ADMINISTRATION_H

#include "Page.h"
#include <string>
#include <vector>

class Lecturer;
class StudentRegistration;

namespace ui
{
    class Administration : public Page
    {
    private:
        bool loggedIn;
        std::string adminUserName;
        std::string adminPass;
        bool login();
        void createCourse();
        void removeCourse();
        void createModule();
        void removeModule();
        void createStudent();
        void removeStudent();
        /**
         * Registers a student on a module
         */
        void registerStudent();
        void resetStudentPassword();
        void calculateStudentQCA();
        void createLecturer();
        void removeLecturer();
        void resetLecturerPassword();
        /**
         * @brief This provides the entry point into the createX functions
         */
        void create();
        /**
         * @brief This provides the entry point into the removeX functions
         */
        void remove();
        /**
         * @brief This provides the entry point into the resetXPassword functions
         */
        void resetPassword();
        bool editModule();
        bool editLecturer();
        void search();

    public:
        Administration(StudentSystem &system, std::string adminUserName, std::string adminPass);
        virtual ~Administration() = default;
        virtual void show() override;
        /**
         * Allows student registration code be shared among multiple classes
         * This method is also used by the internals.
         * @param module The module code to register student on
         * @param system the reference to the student system object
         */
        static void registerStudentOnModule(std::string module, StudentSystem &system);
        friend class ModuleSettingsPage;
    };

} // namespace ui

#endif