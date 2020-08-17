#include "headers/AccountSettingsPage.h"
#include "headers/studsys/LecturerAccount.h"
#include "headers/studsys/StudentAccount.h"
#include "headers/studsys/Lecturer.h"
#include "headers/studsys/Student.h"
#include "headers/UIUtils.h"

using std::string;
using ui::AccountSettingsPage;

AccountSettingsPage::AccountSettingsPage(StudentSystem &system, Account *account) : Page(system), account(account) {}

boost::optional<LecturerAccount> AccountSettingsPage::accToLectAcc()
{
    LecturerAccount *lectAcc = dynamic_cast<LecturerAccount *>(account);

    if (lectAcc)
        return LecturerAccount(*lectAcc);
    else
        return boost::none;
}

boost::optional<StudentAccount> AccountSettingsPage::accToStudAcc()
{
    StudentAccount *studAcc = dynamic_cast<StudentAccount*>(account);

    if (studAcc)
        return StudentAccount(*studAcc);
    else 
        return boost::none;
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
            account->setName(name);
            boost::optional<LecturerAccount> lectAcc = accToLectAcc();
            boost::optional<StudentAccount> studAcc = accToStudAcc();

            if (lectAcc)
            {
                Lecturer lecturer = lectAcc.get().getLecturer();
                lecturer.setName(name);
                system.updateLecturer(lecturer.getEmail(), lecturer);
            }
            else if (studAcc)
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

    string password = ui::getSecureString(ui::passlengthpred, ui::passlengthretrymsg);

    if (password != account->getPassword())
    {
        cout << "The password you entered does not match your current password, aborting..." << endl;
    }
    else
    {
        cout << "Enter the new password: " << endl;

        string newPass = ui::getSecureString(ui::passlengthpred, ui::passlengthretrymsg);

        while (newPass == password)
        {
            cout << "The new password cannot be the same as the previous one, please try again: " << endl;
            newPass = ui::getSecureString(ui::passlengthpred, ui::passlengthretrymsg);
        }

        while (true)
        {
            cout << "Are you sure you want to change your password? (Y)es\\(N)o" << endl;

            string choice = ui::getChoice();

            if (choice == "Y")
            {
                account->setPassword(newPass);
                boost::optional<LecturerAccount> lectAcc = accToLectAcc();
                boost::optional<StudentAccount> studAcc = accToStudAcc();

                if (lectAcc)
                {
                    LecturerAccount lecturerAccount = lectAcc.get();
                    lecturerAccount.setPassword(newPass);
                    system.updateAccount(lecturerAccount.getLecturer(), lecturerAccount);
                }
                else if (studAcc)
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
void AccountSettingsPage::show()
{
    cout << "\nAccount Settings for: " << account->getName() << "\n"
         << endl;

    bool run = true;

    while (run)
    {
        cout << "Do you want to Change (N)ame or (P)assword, (C)ancel, (Q)uit?" << endl;

        string choice = ui::getChoice();

        if (choice == "N")
        {
            changeName();
        }
        else if (choice == "P")
        {
            changePassword();
        }
        else if (choice == "C")
        {
            run = false;
            ui::pageManager.popCurrentPage();
        }
        else if (choice == "Q")
        {
            ui::quit();
        }
    }
}