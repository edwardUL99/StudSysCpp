#include "headers/AccountSettingsPage.h"
#include "headers/LecturerAccount.h"
#include "headers/StudentAccount.h"
#include "headers/Lecturer.h"
#include "headers/Student.h"
#include "headers/UI.h"

using std::string;

AccountSettingsPage::AccountSettingsPage(StudentSystem &system, Account &account) : Page(system), account(account) {}

boost::optional<LecturerAccount> AccountSettingsPage::accToLectAcc()
{
    try
    {
        LecturerAccount &lectAcc = dynamic_cast<LecturerAccount &>(account);
        return LecturerAccount(lectAcc); //copies the lecturerAccount so you don't return a reference to a local variable
    }
    catch (std::bad_cast &b)
    {
        return boost::none;
    }
}

boost::optional<StudentAccount> AccountSettingsPage::accToStudAcc()
{
    try
    {
        StudentAccount &studAcc = dynamic_cast<StudentAccount &>(account);
        return StudentAccount(studAcc);
    }
    catch (std::bad_cast &b)
    {
        return boost::none;
    }
}

void AccountSettingsPage::changeName()
{
    cout << "Enter the new name: " << endl;

    string name = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    while (true)
    {
        cout << "Are you sure you want to change your name to: " << name << "? (Y)es\\(N)o: " << endl;

        string choice = ui::getChoice();

        if (choice == "Y")
        {
            account.setName(name);
            boost::optional<LecturerAccount> lectAcc = accToLectAcc();
            boost::optional<StudentAccount> studAcc = accToStudAcc();

            if (lectAcc.has_value())
            {
                Lecturer lecturer = lectAcc.get().getLecturer();
                lecturer.setName(name);
                system.updateLecturer(lecturer.getID(), lecturer);
            }
            else if (studAcc.has_value())
            {
                Student student = studAcc.get().getStudent();
                student.setName(name);
                system.updateStudent(student.getID(), student);
            }

            cout << "Your name has been updated successfully" << endl;
            break;
        }
        else if (choice == "N")
        {
            cout << "Aborting changes..." << endl;
            break;
        }
    }
}

void AccountSettingsPage::changePassword()
{
    cout << "Please enter your current password to continue: " << endl;

    string password = ui::getSecureString();

    while (password.length() < 8 || password.length() > 16)
    {
        cout << "Password must be 8 or more characters and no longer than 16 characters, try again: " << endl;
        password = ui::getSecureString();
    }

    if (password != account.getPassword())
    {
        cout << "The password you entered does not match your current password, aborting..." << endl;
    }
    else
    {
        cout << "Enter the new password: " << endl;

        string newPass = ui::getSecureString();

        while (newPass.length() < 8 || password.length() > 16)
        {
            cout << "Password must be 8 or more characters and no longer than 16 characters, try again: " << endl;
            newPass = ui::getSecureString();
        }

        while (newPass == password)
        {
            cout << "The new password cannot be the same as the previous one, please try again: " << endl;
            newPass = ui::getSecureString();
        }

        while (true)
        {
            cout << "Are you sure you want to change your password? (Y)es\\(N)o" << endl;

            string choice = ui::getChoice();

            if (choice == "Y")
            {
                account.setPassword(newPass);
                boost::optional<LecturerAccount> lectAcc = accToLectAcc();
                boost::optional<StudentAccount> studAcc = accToStudAcc();

                if (lectAcc.has_value())
                {
                    LecturerAccount lecturerAccount = lectAcc.get();
                    lecturerAccount.setPassword(newPass);
                    system.updateAccount(lecturerAccount.getLecturer(), lecturerAccount);
                }
                else if (studAcc.has_value())
                {
                    StudentAccount studentAccount = studAcc.get();
                    studentAccount.setPassword(newPass);
                    system.updateAccount(studentAccount.getStudent(), studentAccount);
                }

                cout << "Your password has been successfully updated" << endl;

                break;
            }
            else if (choice == "N")
            {
                cout << "Aborting changes..." << endl;
                break;
            }
        }
    }
}

//add more settings as system develops
void AccountSettingsPage::show() {
    cout << "\nAccount Settings for: " << account.getName() << "\n" << endl;

    bool run = true;

    while (run) {
        cout << "Do you want to Change (N)ame or (P)assword, (C)ancel, (Q)uit?" << endl;

        string choice = ui::getChoice();

        if (choice == "N") {
            changeName();
        } else if (choice == "P") {
            changePassword();
        } else if (choice == "C") {
            run = false;
        } else if (choice == "Q") {
            ui::quit();
        }
    }
}