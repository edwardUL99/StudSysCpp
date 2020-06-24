#ifndef MODULE_HOME_PAGE_H
#define MODULE_HOME_PAGE_H

#include "Page.h"
#include "Account.h"
#include "Module.h"

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