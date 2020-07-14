#include "headers/AccountPage.h"
#include "headers/AccountSettingsPage.h"
#include "headers/ModuleSelectorPage.h"
#include "headers/UIUtils.h"

using std::string;
using ui::AccountPage;

AccountPage::AccountPage(StudentSystem &system, Account &account) : Page(system), account(account) {}

Account &AccountPage::getAccount() {
    return account;
}

void AccountPage::show() {
    bool run = true;

    string name = account.getName();
    string email = account.getEmail();

    cout << "\t\tWelcome\t\t" << endl;
    cout << "\t" << name << "\t" << endl;
    cout << "\t" << email << "\t\n" << endl;

    while (run) {
        cout << "(V)iew module sites, (S)ettings, (L)ogout, (Q)uit" << endl;

        string choice = ui::getChoice();

        if (choice == "V") {
            ModuleSelectorPage *selectPage = new ModuleSelectorPage(account, system);
            ui::pageManager.setNextPage(selectPage);
            run = false;
        } else if (choice == "S") {
            AccountSettingsPage *settings = new AccountSettingsPage(system, account);
            ui::pageManager.setNextPage(settings);
            run = false;
        } else if (choice == "L") {
            cout << "Logging out..." << endl;
            run = false;
            ui::pageManager.popCurrentPage();
            ui::pageManager.popCurrentPage(); //want to return to welcome screen so pop twice
        } else if (choice == "Q") {
            ui::quit();
        }
    }
}