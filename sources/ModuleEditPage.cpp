#include "headers/ModuleEditPage.h"
#include "headers/studsys/NotFoundException.h"
#include "headers/studsys/Lecturer.h"

using ui::ModuleEditPage;
using std::string;

ModuleEditPage::ModuleEditPage(const Module &module, StudentSystem &system) : Page(system), oldModule(module), updatedModule(module) {}

void ModuleEditPage::editName() {
    cout << "Please enter the new name of the module: " << endl;
    string name = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);
    updatedModule.setName(name);
}

void ModuleEditPage::editCredits() {
    cout << "Please enter the new credits for the module: " << endl;
    int credits = ui::getInt(ui::intltezeropred, ui::ltezeroretrymsg);
    updatedModule.setCredits(credits);
}

void ModuleEditPage::changeLecturer() {
    cout << "Please enter the e-mail of the new lecturer: (first.lastname@staff.mail.ie) " << endl;
    string email = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    try {
        Lecturer lecturer = system.getLecturer(email);
        updatedModule.setLecturer(lecturer);
    } catch (NotFoundException &nf) {
        cout << "Lecturer with e-mail address " << email << " not found in system, aborting..." << endl;
    }
}

bool ModuleEditPage::saveUpdates() {
    cout << "Confirm the following changes: " << endl;
    cout << "\tCode: " << updatedModule.getCode() << endl;
    cout << "\tName: " << updatedModule.getName() << endl;
    cout << "\tCredits: " << updatedModule.getCredits() << endl;
    cout << "\tLecturer: " << updatedModule.getLecturer().getEmail() << endl;

    while (true) {
        cout << "\nDo you wish to continue? (Y/N)" << endl;

        string choice = ui::getChoice();

        if (choice == "Y") {
            string code = oldModule.getCode();
            if (system.updateModule(code, updatedModule)) {
                cout << "Module " << code << " has been updated successfully, returning to the previous page" << endl;
                ui::pageManager.popCurrentPage();
                return true;
            } else {
                cout << "Module " << code << " has not been updated successfully, please try again later" << endl;
                return false;
            }
        } else if (choice == "N") {
            cout << "Aborting changes..." << endl;
            return false;
        }
    }
}

void ModuleEditPage::show() {
    cout << "Welcome to the edit page for Module " << oldModule.getCode() << "\n" << endl;

    bool run = true;

    while (run) {
        cout << "Edit (N)ame, (C)redits; Change (L)ecturer; (S)ave updates, Cancel (U)pdates, (Q)uit" << endl;

        string choice = ui::getChoice();

        if (choice == "N") {
            editName();
        } else if (choice == "C") {
            editCredits();
        } else if (choice == "L") {
            changeLecturer();
        } else if (choice == "S") {
            run = !saveUpdates();
        } else if (choice == "U") {
            cout << "Cancelling, returning to previous page" << endl;
            run = false;
            ui::pageManager.popCurrentPage();
        } else if (choice == "Q") {
            ui::quit();
        }
    }
}