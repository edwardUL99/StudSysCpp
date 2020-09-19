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
        bool login();
        void createCourse();
        void removeCourse();
        void createModule();
        void removeModule();
        void createStudent();
        void removeStudent();
        /**
         * Registers a student on a module
         * @param module if this is blank, the method asks for the module from stdin
         */
        void registerStudent(std::string module="");
        void resetStudentPassword();
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

    public:
        Administration(StudentSystem &system);
        virtual ~Administration() = default;
        virtual void show() override;
        friend class ModuleSettingsPage;
    };

} // namespace ui

#endif