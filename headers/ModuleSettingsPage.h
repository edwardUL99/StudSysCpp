#ifndef MODULE_SETTINGS_PAGE_H
#define MODULE_SETTINGS_PAGE_H

#include "studsys/Module.h"
#include "studsys/LecturerAccount.h"
#include "Page.h"

namespace ui
{
    class ModuleSettingsPage : public Page
    {
    private:
        Module *module;
        LecturerAccount *account;
        /**
         * @brief This method is used to (de)register a student on a module
         */
        void registerStudents();
        /**
         * @brief unregisters the student from the module
         * @param id the id of the student
         */
        void deRegisterStudent(int id);
        void editModule();

    public:
        ModuleSettingsPage(Module *module, LecturerAccount *account, StudentSystem &system);
        virtual ~ModuleSettingsPage() = default;
        virtual void show() override;
    };
} // namespace ui

#endif