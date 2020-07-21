#ifndef MODULE_HOME_PAGE_H
#define MODULE_HOME_PAGE_H

#include "Page.h"
#include "Account.h"
#include "Module.h"
#include "Announcement.h"

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
        void viewAnnouncement(const Announcement &announcement);
        void createAnnouncement();
        void editAnnouncement(const Announcement &announcement);
        //This method can be used to extract inlined commands like <!append> or <!prepend>, it expects them to be found at the start of the line
        std::string extractCommand(std::string &line);

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