#include "headers/AccountPage.h"
#include "headers/AccountSettingsPage.h"
#include "headers/UI.h"

using std::string;

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

        if (choice == "V" || choice == "v") {
            //show module choice page here
            cout << "Not implemented" << endl;
        } else if (choice == "S" || choice == "s") {
            AccountSettingsPage settings(system, account);
            settings.show();
        } else if (choice == "L" || choice == "l") {
            cout << "Logging out..." << endl;
            run = false;
        } else if (choice == "Q" || choice == "q") {
            ui::quit();
        }
    }
}