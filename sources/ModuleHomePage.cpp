#include "headers/ModuleHomePage.h"
#include "headers/ExamSelectorPage.h"
#include "headers/UIUtils.h"
#include "headers/Administration.h"
#include "headers/NotFoundException.h"

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
                cout << "\t" << i++ << ") " << student.getName() << " - " << student.getID() << " - " << student.getEmail() << endl;
            }

            bool lecturer = false;

            try {
                LecturerAccount &lecturerAccount = dynamic_cast<LecturerAccount&>(account);
                lecturer = true; //if the cast succeeded, it is a lecturer account
            } catch (std::bad_cast &e) {
            }

            if (lecturer) {
                while (true) {
                    cout << "Would you like to register a student on the module? (R)egister, (D)e-register, (N)o" << endl;

                    choice = ui::getChoice();

                    if (choice == "R") {
                        Administration admin(this->system);
                        admin.registerStudent(module.getCode());
                        break;
                    } else if (choice == "D") {
                        cout << "Enter the Student ID for the student to de-register: " << endl;

                        int id = ui::getInt(ui::intltezeropred, ui::ltezeroretrymsg);

                        try {
                            Student student = this->system.getStudent(id);
                            
                            if (this->system.unregisterStudentModule(student, module)) {
                                cout << "Student " << id << " de-registered from module " << module.getCode() << " successfully" << endl;
                            } else {
                                cout << "Student " << id << " de-registered from module " << module.getCode() << " unsuccessfully, please try again later" << endl;
                            }
                        } catch (NotFoundException &nf) {
                            cout << "Student " << id << " does not exist" << endl;
                        }

                        break;
                    } else if (choice == "N") {
                        break;
                    }
                }
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