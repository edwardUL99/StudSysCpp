#include "headers/LoginPage.h"
#include "headers/NotFoundException.h"
#include "headers/NotLoggedInException.h"

using std::string;
using ui::LoginPage;

LoginPage::LoginPage(StudentSystem &system, bool student) : Page(system)
{
    this->email = "";
    this->password = "";
    this->student = student;
    this->loginSuccessful = false;
}

string LoginPage::getEmailText() const
{
    return email;
}

void LoginPage::setEmailText(string emailText)
{
    this->email = emailText;
}

string LoginPage::getPasswordText() const
{
    return password;
}

void LoginPage::setPasswordText(string passwordText)
{
    this->password = passwordText;
}

void LoginPage::show()
{
    bool run = true;

    string loginType = student ? "Student " : "Lecturer ";
    string format = student ? "id@student.mail.ie" : "first.lastName@staff.mail.ie";

    while (run)
    {
        cout << loginType << "Login Page: (E)nter e-mail address, Enter (P)assword, (S)ubmit, (F)orgot password, (C)ancel, (Q)uit" << endl;

        string choice = ui::getChoice();

        if (choice == "E")
        {
            string email = "";

            do
            {
                cout << "Please enter your e-mail address in the format (" << format << "): " << endl;
                email = ui::getString();
            } while (email == "");

            this->email = email;
        }
        else if (choice == "P")
        {
            string password = "";

            cout << "Please enter your password:" << endl;

            this->password = ui::getSecureString(ui::passlengthpred, ui::passlengthretrymsg);
        }
        else if (choice == "S")
        {
            int id = this->student ? this->system.getStudentID(email) : this->system.getLecturerID(email);

            bool submit = false;
            bool exists = true;

            if (this->student)
            {
                try
                {
                    StudentAccount studentAccount = this->system.getStudentAccount(id);

                    submit = studentAccount.getPassword() == password;
                }
                catch (NotFoundException &nf)
                {
                    exists = false;
                    cout << "E-mail " << email << " does not exist" << endl;
                }
            }
            else
            {
                try
                {
                    LecturerAccount lecturerAccount = this->system.getLecturerAccount(id);

                    submit = lecturerAccount.getPassword() == password;
                }
                catch (NotFoundException &nf)
                {
                    exists = false;
                    cout << "E-mail " << email << "does not exist" << endl;
                }
            }

            //here find a way to call next page->show()
            if (submit)
            {
                this->loginSuccessful = true;
                run = false; //bring up next page here, if email matches password, the program returns from login page and the caller of login->show should check if (login.login()) should proceed to page after
                cout << "You have been logged in successfully" << endl;

                string date = this->student ? this->system.recordLogin(this->getStudentAccount()) : this->system.recordLogin(this->getLecturerAccount());

                if (date != "FIRST_LOGIN")
                {
                    cout << "Last login for " << email << ": " << date << ". If the time is suspicious contact the ITT Support Centre immediately.\n"
                         << endl;
                }
                else
                {
                    cout << "This is " + email + "'s first login\n"
                         << endl;
                }
            }
            else if (exists)
                cout << "The password does not match the account identified by the e-mail " << email << endl;
        }
        else if (choice == "F")
        {
            cout << "Please contact the Information Technology Student Centre to reset your password" << endl;
        }
        else if (choice == "C")
        {
            this->loginSuccessful = false;
            run = false;
        }
        else if (choice == "Q")
        {
            ui::quit();
        }
    }
}

bool LoginPage::login() const
{
    return loginSuccessful;
}

bool LoginPage::isStudentLogin() const
{
    return student;
}

StudentAccount LoginPage::getStudentAccount() const
{
    if (!this->student)
    {
        throw NotLoggedInException("You cannot be logged in as a Student with a Lecturer login page");
    }
    else if (!loginSuccessful)
    {
        throw NotLoggedInException("Student is not logged in, so no account to return");
    }
    else
    {
        int id = this->system.getStudentID(this->email);

        return this->system.getStudentAccount(id);
    }
}

LecturerAccount LoginPage::getLecturerAccount() const
{
    if (this->student)
    {
        throw NotLoggedInException("You cannot be logged in as a Lecturer with a Student login page");
    }
    else if (!loginSuccessful)
    {
        throw NotLoggedInException("Lecturer is not logged in, so no account to return");
    }
    else
    {
        int id = this->system.getLecturerID(this->email);

        return this->system.getLecturerAccount(id);
    }
}
