#ifndef MODULE_HOME_PAGE_H
#define MODULE_HOME_PAGE_H

#include "Page.h"
#include "studsys/Account.h"
#include "studsys/Module.h"
#include "studsys/Announcement.h"
#include <vector>

namespace ui
{

    /**
 * This page is the first page you "land" on after selecting a Module
 */
    class ModuleHomePage : public Page
    {
    private:
        Account &account;
        Module &module;
        /**
         * @brief This method is used to (de)register a student on a module
         */
        void registerStudents();
        /**
         * @brief unregisters the student from the module
         * @param id the id of the student
         */
        void deRegisterStudent(int id);
        /**
         * @brief This method displayes students that are registered on this oparticular module
         */
        void displayRegisteredStudents();
        void displayLecturerDetails();
    public:
        /**
         * @param account the account that logged into this ModuleHomePage (i.e. account that is logged in)
         * @param module the module that this home page is for
         * @param system the backing system this UI is running on
         */
        ModuleHomePage(Account &account, Module &module, StudentSystem &system);
        virtual ~ModuleHomePage() = default;
        virtual void show() override;
    };

} // namespace ui

#endif