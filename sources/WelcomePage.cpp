#include "headers/WelcomePage.h"
#include "headers/LoginPage.h"
#include "headers/Administration.h"
#include "headers/UIUtils.h"

using std::string;
using ui::WelcomePage;

const string WelcomePage::welcomeText = "Welcome to the Student Management System.\nIf you do not have an account, please contact the administration office.\nOtherwise, login following the commands below\n";

WelcomePage::WelcomePage(StudentSystem &system) : Page(system) {}

void WelcomePage::show() {
    cout << WelcomePage::welcomeText << endl;

    bool run = true;

    string choice;

    while (run) {
        cout << "Login as: (L)ecturer or (S)tudent; (C)ancel; (Q)uit" << endl;

        string choice = ui::getChoice();

        LoginPage *loginPage = NULL;

        if (choice == "A") {
            Administration *admin = new Administration(this->system);
            ui::pageManager.setNextPage(admin);
            break;
        } else if (choice == "L") {
            loginPage = new LoginPage(this->system, false);
        } else if (choice == "S") {
            loginPage = new LoginPage(this->system, true);
        } else if (choice == "C") {
            run = false;
            ui::pageManager.popCurrentPage();
        } else if (choice == "Q") {
            ui::quit();
        }



        if (loginPage) {
            ui::pageManager.setNextPage(loginPage);
            break;
        }
    }
}