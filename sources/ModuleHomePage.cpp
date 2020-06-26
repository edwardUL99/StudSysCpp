#include "headers/ModuleHomePage.h"
#include "headers/ExamSelectorPage.h"
#include "headers/UIUtils.h"

using ui::ModuleHomePage;
using std::string;

ModuleHomePage::ModuleHomePage(Account &account, Module &module, StudentSystem &system) : Page(system), account(account), module(module) {}

void ModuleHomePage::show() {
    string code = module.getCode();

    cout << "Welcome to the homepage for Module " << code << "\n" << endl;

    bool run = true;

    while (run) {
        cout << "View: (A)nnouncements, (T)asks, (E)xams, (S)tudent list, (L)ecturer details; Change (M)odule settings, (B)ack, (Q)uit" << endl;

        string choice = ui::getChoice();

        if (choice == "A") {
            cout << "Not implemented yet" << endl;
        } else if (choice == "T") {
            cout << "Not implemented yet" << endl;
        } else if (choice == "E") {
            ExamSelectorPage selectorPage(this->account, this->module, this->system);
            selectorPage.show();
        } else if (choice == "S") {
            cout << "Students registered on Module " << code << ":" << endl;
            
            int i = 1;

            for (const Student &student : this->system.getStudentsRegisteredOnModule(module)) {
                cout << "\t" << i << ") " << student.getName() << " - " << student.getID() << " - " << student.getEmail() << endl;
            }
        } else if (choice == "L") {
            Lecturer lecturer = module.getLecturer();

            cout << "The lecturer for this module is: " << endl;
            cout << "\tName: " << lecturer.getName() << endl;
            cout << "\tDepartment: " << lecturer.getDepartment() << endl;
            cout << "\tE-mail Address: " << lecturer.getEmail() << endl;
        } else if (choice == "M") {
            cout << "Not implemented yet" << endl;
        } else if (choice == "B") {
            cout << "Going back to module selector page\n" << endl;
            run = false;
        } else if (choice == "Q") {
            ui::quit();
        }
    }
}