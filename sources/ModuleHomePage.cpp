#include "headers/ModuleHomePage.h"
#include "headers/ExamSelectorPage.h"
#include "headers/UIUtils.h"
#include "headers/Administration.h"
#include "headers/NotFoundException.h"
#include "headers/DuplicateException.h"

#include <vector>
using ui::ModuleHomePage;
using std::string;
using std::vector;

ModuleHomePage::ModuleHomePage(Account &account, Module &module, StudentSystem &system) : Page(system), account(account), module(module) {}

void ModuleHomePage::viewAnnouncement(const Announcement &announcement) {
    string time = system.getAnnouncementTime(announcement);

    Lecturer lecturer = announcement.getLecturer();
    cout << "Lecturer: " << lecturer.getName() << " <" << lecturer.getEmail() << ">" << endl;
    cout << "Time: " << time << endl;
    cout << "Subject: " << announcement.getSubject() << endl;
    cout << "Announcement:\n" << announcement.getAnnouncementText() << endl;
}

void ModuleHomePage::createAnnouncement() {
    Lecturer lecturer = dynamic_cast<LecturerAccount&>(account).getLecturer();

    cout << "Enter the subject for the announcement (max 250 characters): " << endl;
    string subject = ui::getString(Predicate<string>([](const string &s) -> bool { return s.length() < 1 || s.length() > 250; }), "Please enter a subject that is between 1 and 250 characters:");

    cout << "You can now enter the announcement text line by line. Type <!submit> to finish and to create the announcementm or <!cancel> to cancel" << endl;

    string text = "";
    bool run = true;
    bool submit = false;
    bool cancel = false;

    while (run) {
        string temp = ui::getString();

        if (temp != "<!submit>" && temp != "<!cancel>") {
            text += temp + "\n";
        } else if (temp == "<!submit>") {
            submit = true;
            run = false;
        } else if (temp == "<!cancel>") {
            cancel = true;
            run = false;
        }
    }

    if (cancel) {
        cout << "Announcement cancelled" << endl;
    } else {
        Announcement announcement(module, lecturer, subject, text);

        try {
           if (this->system.addAnnouncement(announcement)) {
               cout << "Announcement was created successfully" << endl;
           } else {
               cout << "Announcement was not created successfully, please try again later" << endl;
           }
        } catch (DuplicateException &d) {
            cout << "Announcement already exists" << endl;
        }
    }
}

void ModuleHomePage::show() {
    string code = module.getCode();

    cout << "Welcome to the homepage for Module " << code << "\n" << endl;

    bool run = true;

    while (run) {
        cout << "View: (A)nnouncements, (T)asks, (E)xams, (S)tudent list, (L)ecturer details; Change (M)odule settings, (B)ack, (Q)uit" << endl;

        string choice = ui::getChoice();

        if (choice == "A") {
            bool run = true;
            vector<Announcement> announcements = system.getModuleAnnouncements(module);

            while (run) {
                bool lecturer = false;
                try {
                    LecturerAccount &lecturerAcc = dynamic_cast<LecturerAccount&>(account);
                    lecturer = true;
                } catch (std::bad_cast &b) {
                }

                if (lecturer) cout << "(N)ew announcement, (E)dit announcement, (V)iew announcements, (B)ack, (Q)uit" << endl;
                else cout << "(V)iew announcements, (B)ack, (Q)uit" << endl;

                string choice = ui::getChoice();

                if (choice == "N" && lecturer) {
                    createAnnouncement();
                } else if (choice == "E" && lecturer) {
                    cout << "Not implemented yet" << endl;
                } else if (choice == "V" ) {
                    int length = announcements.size();

                    if (length == 0) {
                        cout << "There are no announcements" << endl;
                    } else {
                        int i = 1;
                        for (const Announcement &announcement : announcements) {
                            cout << i++ << ") " << announcement.getSubject() << endl;
                        }
                        cout << "Choose a number between 1 and " << length << " to choose which announcement to view (1 being the newest): " << endl;
                        int num = ui::getInt(Predicate<int>([length](const int &x) -> bool { return x < 1 || x > length; }), "Choose a number between 1 and " + std::to_string(length) + ": ");

                        num -= 1;

                        viewAnnouncement(announcements[num]);
                    }
                } else if (choice == "B") {
                    run = false;
                } else if (choice == "Q") {
                    ui::quit();
                }

            }
        } else if (choice == "T") {
            cout << "Not implemented yet" << endl;
        } else if (choice == "E") {
            ExamSelectorPage *selectorPage = new ExamSelectorPage(this->account, this->module, this->system);
            ui::pageManager.setNextPage(selectorPage);
            run = false;
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
            ui::pageManager.popCurrentPage();
        } else if (choice == "Q") {
            ui::quit();
        }
    }
}