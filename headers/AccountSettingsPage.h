#ifndef ACCOUNT_SETTINGS_PAGE_H
#define ACCOUNT_SETTINGS_PAGE_H

#include "Page.h"
#include "studsys/Account.h"
#include <boost/optional.hpp>

class LecturerAccount;
class StudentAccount;

//think of other settings as the system develops
namespace ui
{

    class AccountSettingsPage : public Page
    {
    private:
        Account *account;
        boost::optional<LecturerAccount> accToLectAcc();
        boost::optional<StudentAccount> accToStudAcc();
        void changeName();
        void changePassword();

    public:
        AccountSettingsPage(StudentSystem &system, Account *account);
        virtual ~AccountSettingsPage() = default;
        virtual void show() override;
    };

} // namespace ui

#endif