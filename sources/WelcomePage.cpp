#include "headers/WelcomePage.h"
#include "headers/LoginPage.h"
#include "headers/AccountPage.h"
#include "headers/Administration.h"

using std::string;

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

        if (choice == "A" || choice == "a") {
            Administration admin(this->system);
            admin.show();
        } else if (choice == "L" || choice == "l") {
            loginPage = new LoginPage(this->system, false);
        } else if (choice == "S" || choice == "s") {
            loginPage = new LoginPage(this->system, true);
        } else if (choice == "C" || choice == "c") {
            run = false;
        } else if (choice == "Q" || choice == "q") {
            ui::quit();
        }



        if (loginPage) {
            loginPage->show(); //show the login screen

            if (loginPage->login()) {
                if (loginPage->isStudentLogin()) {
                    StudentAccount studentAccount = loginPage->getStudentAccount(); //this will always be in scope for any pages since WelcomePage is the parent page and if WelcomePage whule loop ends and leaves show, no other page should be open
                    AccountPage accountPage(system, studentAccount);
                    
                    accountPage.show(); //WelcomePage is parent page, shows accountPage, which then shows other pages and those other pages may show other pages etc, and then when finished will eventually return back here
                } else {
                    LecturerAccount lecturerAccount = loginPage->getLecturerAccount(); //this will always be in scope for any pages since WelcomePage is the parent page and if WelcomePage whule loop ends and leaves show, no other page should be open
                    AccountPage accountPage(system, lecturerAccount);

                    accountPage.show(); //WelcomePage is parent page, shows accountPage, which then shows other pages and those other pages may show other pages etc, and then when finished will eventually return back here
                }
            }
        }

        delete loginPage;
    }
}