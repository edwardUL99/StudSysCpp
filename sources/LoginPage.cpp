#include "headers/LoginPage.h"
#include "headers/studsys/NotFoundException.h"
#include "headers/studsys/NotLoggedInException.h"
#include "headers/AccountPage.h"
#include "headers/UIUtils.h"

using std::string;
using ui::AccountPage;
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

bool LoginPage::processLoginDetails(bool &exists)
{
    bool submit = false;
    if (this->student)
    {
        try
        {
            string stringId = email.substr(0, email.find('@'));
            int id = std::atoi(stringId.c_str());
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
            LecturerAccount lecturerAccount = this->system.getLecturerAccount(this->email);

            submit = lecturerAccount.getPassword() == password;
        }
        catch (NotFoundException &nf)
        {
            exists = false;
            cout << "E-mail " << email << " does not exist" << endl;
        }
    }

    return submit;
}

AccountPage *LoginPage::getAccountPage()
{
    AccountPage *accountPage = nullptr;

    if (isStudentLogin())
    {
        StudentAccount *studentAccount = new StudentAccount(getStudentAccount()); //this will always be in scope for any pages since WelcomePage is the parent page and if WelcomePage whule loop ends and leaves show, no other page should be open
        accountPage = new AccountPage(system, studentAccount);
        ui::pageManager.addSharedEntity(accountPage, studentAccount);
    }
    else
    {
        LecturerAccount *lecturerAccount = new LecturerAccount(getLecturerAccount()); //this will always be in scope for any pages since WelcomePage is the parent page and if WelcomePage whule loop ends and leaves show, no other page should be open
        accountPage = new AccountPage(system, lecturerAccount);
        ui::pageManager.addSharedEntity(accountPage, lecturerAccount);
    }

    return accountPage;
}

void LoginPage::loginUser()
{
    this->loginSuccessful = true;
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

    ui::pageManager.setNextPage(getAccountPage());
}

string LoginPage::trimEmailAddress(string emailAddress) {
    string str = "";

    for (int i = 0; i < emailAddress.length(); i++) {
        if (!isspace(emailAddress[i])) {
            str += emailAddress[i];
        }
    }

    return str;
}

string LoginPage::autoFillEmailAddress(string emailAddress) {
    if (emailAddress.find('@') == std::string::npos) {
        string extension = this->student ? "@student.mail.ie":"@staff.mail.ie";
        emailAddress = trimEmailAddress(emailAddress);
        return emailAddress += extension;
    } else {
        return emailAddress;
    }
}

void LoginPage::setEmailAddress()
{
    string email = "";
    string format = student ? "id@student.mail.ie" : "first.lastName@staff.mail.ie";

    do
    {
        cout << "Please enter your e-mail address in the format (" << format << ", you can just enter the text before @ if you wish): " << endl;
        email = ui::getString();
    } while (email == "");

    this->email = autoFillEmailAddress(email);
}

void LoginPage::setPassword()
{
    string password = "";

    cout << "Please enter your password:" << endl;

    this->password = ui::getSecureString(ui::passlengthpred, ui::passlengthretrymsg);
}

bool LoginPage::submit()
{
    bool exists = true;
    bool submit = processLoginDetails(exists);

    //here find a way to call next page->show()
    if (submit)
    {
        loginUser();
        return false;
    }
    else if (exists)
    {
        cout << "Password does not match that of " << email << endl;
    }

    return true;
}

void LoginPage::show()
{
    bool run = true;

    string loginType = student ? "Student " : "Lecturer ";

    while (run)
    {
        cout << loginType << "Login Page: (E)nter e-mail address, Enter (P)assword, (S)ubmit, (F)orgot password, (C)ancel, (Q)uit" << endl;

        string choice = ui::getChoice();

        if (choice == "E")
        {
            setEmailAddress();
        }
        else if (choice == "P")
        {
            setPassword();
        }
        else if (choice == "S")
        {
            run = submit();
        }
        else if (choice == "F")
        {
            cout << "Please contact the Information Technology Student Centre to reset your password" << endl;
        }
        else if (choice == "C")
        {
            this->loginSuccessful = false;
            run = false;
            ui::pageManager.popCurrentPage(); //get out of this page
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
        int id = std::atoi(email.substr(0, email.find('@')).c_str());

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
        return this->system.getLecturerAccount(this->email);
    }
}
