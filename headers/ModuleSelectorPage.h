#ifndef MODULE_SELECTOR_PAGE_H
#define MODULE_SELECTOR_PAGE_H

#include "Page.h"
#include "studsys/Account.h"
#include <vector>

class Module;

namespace ui
{

    class ModuleSelectorPage : public Page
    {
    private:
        Account *account;
        bool isStudentAccount() const;
        std::vector<Module> getModuleList() const; //gets the module list for either the lecturer teaching them or the student registered with them
        /**
         * @brief displays the modules the account is registered on
         */       
        void displayRegisteredModules();
        /**
         * @brief opens the ModuleHomePage for a module selected by the user
         * @return true if tou are to exit the calling while loop
         */
        bool viewModule();

    public:
        ModuleSelectorPage(Account *account, StudentSystem &system);
        virtual ~ModuleSelectorPage() = default;
        virtual void show() override;
    };

} // namespace ui

#endif