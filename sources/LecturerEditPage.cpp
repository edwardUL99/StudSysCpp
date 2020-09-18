#include "headers/LecturerEditPage.h"
#include "headers/studsys/Lecturer.h"
#include "headers/studsys/Predicate.h"
#include <cstring>

using ui::LecturerEditPage;
using std::string;

LecturerEditPage::LecturerEditPage(const Lecturer &lecturer, StudentSystem &system) : Page(system), oldLecturer(lecturer), newLecturer(lecturer) {}

void LecturerEditPage::setNameCaps(string &str, bool firstLetterUpper) {
    for (int i = 0; i < str.size(); i++) {
        if (i == 0)
            str[i] = firstLetterUpper ? toupper(str[i]):tolower(str[i]);
        else
            str[i] = tolower(str[i]);
        
    }
}

string LecturerEditPage::nameToEmailAddress(string name) {
    if (name.find("@") != std::string::npos)
        return emailAddressToName(name);

    std::size_t spacePos = name.find(" ");
    string fname = name.substr(0, spacePos);
    string lname = name.substr(spacePos + 1, name.size());
    setNameCaps(fname, false);
    setNameCaps(lname, false);
    return fname + "." + lname + "@staff.mail.ie";
}

string LecturerEditPage::emailAddressToName(string email) {
    std::size_t atSymPos = email.find("@");
    if (atSymPos == std::string::npos)
        return nameToEmailAddress(email);
    
    string nameSection = email.substr(0, atSymPos);
    std::size_t dotPos = nameSection.find(".");
    if (dotPos == std::string::npos)
        return "";
    
    string fname = nameSection.substr(0, dotPos);
    string lname = nameSection.substr(dotPos + 1, nameSection.size());
    setNameCaps(fname, true);
    setNameCaps(lname, true);

    return fname + " " + lname;
}

void LecturerEditPage::updateName() {
    cout << "Please enter the new name: " << endl;

    string newName = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    updateEmail(nameToEmailAddress(newName));
    newLecturer.setName(newName);
}

void LecturerEditPage::updateName(string name) {
    newLecturer.setName(name);
}

void LecturerEditPage::updateAge() {
    cout << "Please enter the new age: " << endl;

    int newAge = ui::getInt(Predicate<int>([](const int &x) -> bool { return x < 24; }), "The age of the lecturer must be 24 or over, please try again: ");

    newLecturer.setAge(newAge);
}

void LecturerEditPage::updateEmail() {
    cout << "Please enter the new e-mail (first.lastname@staff.mail.ie):" << endl;

    string newEmail = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    updateName(emailAddressToName(newEmail));
    newLecturer.setEmail(newEmail);
}

void LecturerEditPage::updateEmail(string email) {
    newLecturer.setEmail(email);
}

void LecturerEditPage::updateDepartment() {
    cout << "Enter the new department: " << endl;

    string newDepartment = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    newLecturer.setDepartment(newDepartment);
}

bool LecturerEditPage::submitChanges() {
    while (true) {
        cout << "Confirm Changes:\n\tName: " << newLecturer.getName() << "\n\tAge: " << newLecturer.getAge() << "\n\tE-mail: " << newLecturer.getEmail() << "\n\tDepartment: " << newLecturer.getDepartment() << "\n(Y/N): " << endl;

        string choice = ui::getChoice();

        if (choice == "Y") {
            bool success = system.updateLecturer(oldLecturer.getEmail(), newLecturer);

            if (success) {
                cout << "Lecturer updated successfully\n" << endl;
            } else {
                cout << "Lecturer not updated successfully, please try again later" << endl;
                cout << "This may be because there are no changes to be made or an error occurred\n" << endl;
            }

            return true;
        } else if (choice == "N") {
            cout << "Not saving changes" << endl;
            return false;
        }
    }
}

void LecturerEditPage::show() {
    cout << "Welcome to the edit page for Lecturer " << oldLecturer.getEmail() << "\n" << endl;

    bool run = true;

    while (run) {
        cout << "Edit: (N)ame, (E)-mail Address, (A)ge, (D)epartment, (S)ubmit changes; (C)ancel, (Q)uit" << endl;

        string choice = ui::getChoice();

        if (choice == "N") {
            updateName();
        } else if (choice == "E") {
            updateEmail();
        } else if (choice == "A") {
            updateAge();
        } else if (choice == "D") {
            updateDepartment();
        } else if (choice == "S") {
            run = !submitChanges();

            if (!run) {
                ui::pageManager.popCurrentPage();
            }
        } else if (choice == "C") {
            run = false;
            ui::pageManager.popCurrentPage();
        } else if (choice == "Q") {
            ui::quit();
        }
    }
}