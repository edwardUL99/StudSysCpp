#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H

#include "Page.h"
#include "studsys/LecturerAccount.h"
#include "studsys/StudentAccount.h"

#include <string>

namespace ui
{
    class AccountPage;

    class LoginPage : public Page
    {
    private:
        std::string email;
        std::string password;
        bool student; //true if student, false if lecturer
        bool loginSuccessful;
        /**
         * @brief this method processes the entered login details to check if they are correct
         * @param exists this method sets this boolean with the statis of the existance of the account specified by the login details
         * @return the correctness of the login details
         */
        bool processLoginDetails(bool &exists);
        /**
         * @brief Gets the account page that will be displayed after login
         * @return appropriate account page
         */
        AccountPage* getAccountPage();
        /**
         * @brief This method processes logging in of the user
         */
        void loginUser();
        /**
         * @brief Takes the e-mail address and checks if it is shortened i.e. just <username> of <username>@staff.mail.ie for example and depending on the login page appends the correct ending on
         * If the e-mail is fully qualified, it just returns the provided email address
         * @param emailAddress the e-mail to attempt to autofill
         */
        std::string autoFillEmailAddress(std::string emailAddress);
        /**
         * @brief Gets the email address from the user and sets the pages email field
         */
        void setEmailAddress();
        /**
         * @brief Gets the password from the user and sets the pages email field
         */
        void setPassword();
        /**
         * @brief provides the submit functionality to login
         * @return true if you want the calling while loop to continue, false otherwise
         */
        bool submit();
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