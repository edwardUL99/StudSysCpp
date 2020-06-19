#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H

#include "Page.h"
#include "LecturerAccount.h"
#include "StudentAccount.h"

#include <string>

namespace ui
{

    class LoginPage : public Page
    {
    private:
        std::string email;
        std::string password;
        bool student; //true if student, false if lecturer
        bool loginSuccessful;

    public:
        //In UI going to LoginPage from previous page, have a button for lecturer login and have student false, have button for Student login and have student true
        LoginPage(StudentSystem &studentSystem, bool student);
        virtual ~LoginPage() = default;
        std::string getEmailText() const;
        void setEmailText(std::string email);
        std::string getPasswordText() const;
        void setPasswordText(std::string password);
        bool login() const; //if login is true on returning from this page, the parent page(UI) should then proceed to the next page
        //implement following 3 methods
        /**
         * Returns true if this login page is logging in a student, false if logging in a lecturer
         */
        bool isStudentLogin() const;
        /*following methods throw NotLoggedInException in the folllowing situations:
            They are called when login() returns false
            If getLecturerAccount() is called and isStudentLogin() is true
            If getStudentAccount() is called and isStudentLogin() is false
        */
        StudentAccount getStudentAccount() const; //MAY BE GOOD IDEA TO HAVE POLYMORPHIC CLASSES AND HAVE ACCOUNT AS BASE CLASS SO CAN JUST HAVE getAccount RETURNING ACCOUNT AND IN THE SCREEN AFTER WELCOME, JUST ONE CONSTRUCTOR TAKING ACCOUNT PARAM RATHER THAN 2 DIFFERENT PAGES
        LecturerAccount getLecturerAccount() const;
        virtual void show() override;
    };

} // namespace ui
#endif