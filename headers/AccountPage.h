#ifndef ACCOUNT_PAGE_H
#define ACCOUNT_PAGE_H

#include "Page.h"
#include "studsys/Account.h"

namespace ui
{

    //This class is the first page arrived on after login
    class AccountPage : public Page
    {
    private:
        Account *account;

    public:
        AccountPage(StudentSystem &system, Account *account);
        virtual ~AccountPage() = default;
        const Account *getAccount();
        virtual void show() override;
    };

} // namespace ui

#endif