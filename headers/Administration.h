#ifndef ADMINISTRATION_H
#define ADMINISTRATION_H

#include "Page.h"
#include <string>

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
        /**
         * This is a method for the workaround requested by issue #85
         * @see https://github.com/edwardUL99/StudSysCppCLI/issues/85
         */
        bool modulesRegisteredToLecturer(std::string email);
        bool editModule();

    public:
        Administration(StudentSystem &system);
        virtual ~Administration() = default;
        virtual void show() override;
        friend class ModuleSettingsPage;
    };

} // namespace ui

#endif