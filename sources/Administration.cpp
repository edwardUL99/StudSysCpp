#include "headers/Administration.h"
#include "headers/studsys/Module.h"
#include "headers/studsys/Student.h"
#include "headers/studsys/StudentAccount.h"
#include "headers/studsys/Lecturer.h"
#include "headers/studsys/LecturerAccount.h"
#include "headers/studsys/NotFoundException.h"
#include "headers/studsys/DuplicateException.h"
#include "headers/UIUtils.h"
#include "headers/ModuleEditPage.h"
#include "headers/LecturerEditPage.h"
#include "headers/studsys/StudentRegistration.h"

#include <cstring>
#include <string>
using std::string;
using ui::Administration;
using ui::ModuleEditPage;

Administration::Administration(StudentSystem &studentSystem, string adminUserName, string adminPass) : Page(studentSystem), loggedIn(false)
{
    this->adminUserName = adminUserName;
    this->adminPass = adminPass;
}

bool Administration::login()
{
    if (!loggedIn)
    {
        cout << "Please enter the admin username: " << endl;

        string username = ui::getString();

        if (username == adminUserName)
        {
            cout << "Please enter the password: " << endl;

            string password = ui::getSecureString();

            if (password == adminPass)
            {
                cout << "You are now logged in as admin" << endl;

                loggedIn = true;
            }
            else
            {
                cout << "The password does not match\n"
                     << endl;
            }
        }
        else
        {
            cout << "The username is not correct\n"
                 << endl;
        }
    }

    return loggedIn;
}

void Administration::createCourse()
{
    cout << "Please enter the course code: " << endl;

    string code = ui::getCourseCode();

    cout << "Please enter the course type: " << endl;

    string type = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    cout << "Please enter the name of the course: " << endl;

    string name = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    cout << "Please enter the duration of the course in years: " << endl;

    int duration = ui::getInt(ui::intltezeropred, ui::ltezeroretrymsg);

    cout << "Please enter the course leader e-mail address: " << endl;

    string email = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    try
    {
        Lecturer lecturer = system.getLecturer(email);
        Course course(code, type, name, duration, lecturer);

        if (system.addCourse(course))
        {
            cout << "Course " << code << " created successfully" << endl;
        }
        else
        {
            cout << "Course " << code << " was not created successfully, please try again later" << endl;
        }
    }
    catch (NotFoundException &nf)
    {
        cout << "Lecturer does not exist, exiting course setup" << endl;
        return;
    }
    catch (DuplicateException &d)
    {
        cout << "Course already exists, exiting course setup" << endl;
    }
}

void Administration::removeCourse()
{
    cout << "Please enter the course code to remove: " << endl;

    string code = ui::getCourseCode();

    try
    {
        Course course = system.getCourse(code);

        string removalMessage = "Removing a course has the following effect:\n\t-All students on this course will no longer be registered on a course\n";
        bool remove = ui::confirmRemoval(removalMessage, 1);

        if (!remove)
        {
            cout << "ABorting removal of course..." << endl;
            return;
        }

        if (system.removeCourse(course))
        {
            cout << "Course " << code << " was removed successfully" << endl;
        }
        else
        {
            cout << "Course " << code << " was not removed successfully, please try again later" << endl;
        }
    }
    catch (NotFoundException &nf)
    {
        cout << "The course you specified does not exist, exiting course deletion" << endl;
    }
}

void Administration::createModule()
{
    cout << "Please enter the module code: " << endl;

    string code = ui::getModuleCode();

    cout << "Please enter the module name: " << endl;

    string name = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    cout << "Please enter the number of credits: " << endl;

    int credits = ui::getInt(ui::intltezeropred, ui::ltezeroretrymsg);

    cout << "Please enter the lecturer's e-mail address who's in charge of the module: " << endl;

    string email = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    try
    {
        Lecturer lecturer = system.getLecturer(email);
        Module module(code, name, credits, lecturer);

        if (system.addModule(module))
        {
            cout << "Module " << code << " created successfully" << endl;
        }
        else
        {
            cout << "Module " << code << " was not created successfully, please try again later" << endl;
        }
    }
    catch (NotFoundException &nf)
    {
        cout << "Lecturer does not exist, exiting module setup" << endl;
        return;
    }
    catch (DuplicateException &d)
    {
        cout << "Module already exists, exiting module setup" << endl;
    }
}

void Administration::removeModule()
{
    cout << "Please enter the Module code to remove: " << endl;

    string code = ui::getModuleCode();

    try
    {
        Module module = system.getModule(code);

        string removalMessage = "Removing a module also removes the following for this module:\n\t-All announcements\n\t-All student registrations\n\t-Exams\n\t-Exam Grades\n\t-Module Grades\n";
        bool remove = ui::confirmRemoval(removalMessage, 2);

        if (!remove)
        {
            cout << "Aborting removal of lecturer..." << endl;
            return;
        }

        if (system.removeModule(module))
        {
            cout << "Module " << code << " was removed successfully" << endl;
        }
        else
        {
            cout << "Module " << code << " was not removed successfully, please try again later" << endl;
        }
    }
    catch (NotFoundException &nf)
    {
        cout << "The module you specified does not exist, exiting module deletion" << endl;
    }
}

void Administration::createStudent()
{
    cout << "Please enter the student ID to be used for the new student: " << endl;

    int id = ui::getInt(ui::intltezeropred, ui::ltezeroretrymsg);

    cout << "Please enter the Student's name: " << endl;

    string name = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    cout << "Please enter the Student's age: " << endl;

    int age = ui::getInt(Predicate<int>([](const int &x) -> bool { return x < 17; }), "The age of the student must be 17 or over, please try again: ");

    string email = std::to_string(id) + "@student.mail.ie";

    cout << "Please enter the course code for which this Student will be enrolled in: " << endl;

    string code = ui::getCourseCode();

    try
    {
        Course course = system.getCourse(code);
        Student student(id, name, age, course);

        cout << "Choose a password for this student: " << endl;

        string password = ui::getSecureString(ui::passlengthpred, ui::passlengthretrymsg);

        StudentAccount account(student, password);

        if (system.addStudent(student))
        {
            cout << "Student " << id << " successfully added to system" << endl;
            system.addAccount(account);
        }
        else
        {
            cout << "Student " << id << " not successfully added to the system, please try again later" << endl;
        }
    }
    catch (NotFoundException &nf)
    {
        cout << "The course does not exist, exiting student setup" << endl;
    }
    catch (DuplicateException &d)
    {
        cout << "The student already exists, if you are trying to reset password, use Reset Password option, exiting student setup" << endl;
    }
}

void Administration::removeStudent()
{
    cout << "Please enter the student ID you wish to remove: " << endl;

    int id = ui::getInt(ui::intltezeropred, ui::ltezeroretrymsg);

    try
    {
        Student student = system.getStudent(id);
        StudentAccount account = system.getStudentAccount(id);

        string removalMessage = "Removing a student also removes the following information about this student:\n\t-Exam Grades\n\t-Module Grades\n\t-Student account\n";
        bool remove = ui::confirmRemoval(removalMessage, 2);

        if (!remove)
        {
            cout << "Aborting removal of student..." << endl;
            return;
        }

        system.removeAccount(account);

        if (system.removeStudent(student))
        {
            cout << "Student " << id << " removed successfully from the system" << endl;
        }
        else
        {
            cout << "The Student " << id << " was not successfully removed from the system, please try again later" << endl;
        }
    }
    catch (NotFoundException &nf)
    {
        cout << "The Student " << id << " does not exist, exiting Student deletion" << endl;
    }
}

void Administration::resetStudentPassword()
{
    cout << "Please enter the student ID for whom to reset the password: " << endl;

    int id = ui::getInt(ui::intltezeropred, ui::ltezeroretrymsg);

    try
    {
        Student student = system.getStudent(id);

        StudentAccount account = system.getStudentAccount(id);

        cout << "Please enter the new password: " << endl;

        string password = ui::getSecureString(ui::passlengthpred, ui::passlengthretrymsg);

        account.setPassword(password);

        if (system.updateAccount(student, account))
        {
            cout << "The password was updated successfully" << endl;
        }
        else
        {
            cout << "The password was not updated successfully, please try again later" << endl;
            cout << "This may be because there's no changes to update or an error occurred" << endl;
        }
    }
    catch (NotFoundException &nf)
    {
        cout << "The Student " << id << " does not exist, exiting Student password reset" << endl;
    }
}

void Administration::registerStudent()
{
    cout << "Enter the code of the module to register the student on: " << endl;
    string code = ui::getModuleCode();
    Administration::registerStudentOnModule(code, system);
}

void Administration::registerStudentOnModule(std::string module, StudentSystem &system)
{
    bool containsStudent = false;
    bool containsModule = false;

    cout << "Enter the student ID of the student to register: " << endl;

    int id = ui::getInt(ui::intltezeropred, ui::ltezeroretrymsg);

    try
    {
        Student student = system.getStudent(id);
        containsStudent = true; //if above executed without throwing, the student was found in the system

        Module registeredModule = system.getModule(module);

        if (registeredModule.getLecturer() == Lecturer::NOT_FOUND)
        {
            cout << "This module does not have a lecturer associated with it, you cannot register students to it, aborting..." << endl;
            return;
        }

        containsModule = true; //if above executed without throwing, the module was found in the system

        if (system.registerStudentModule(student, registeredModule))
        {
            cout << "Student " << id << " has been registered on Module " << module << "." << endl;
        }
        else
        {
            cout << "Registration was not successful, please try again later" << endl;
        }
    }
    catch (NotFoundException &nf)
    {
        if (!containsStudent)
            cout << "Student with ID " << id << " not found in the system, exiting registration..." << endl;
        if (!containsModule)
            cout << "Module with code " << module << " not found in the system, exiting registration..." << endl;
        return;
    }
    catch (DuplicateException &dup)
    {
        cout << "Student " << id << " has already been registered on Module " << module << "." << endl;
    }
}

void Administration::calculateStudentQCA()
{
    cout << "Enter the ID of the student to calculate the QCA for: " << endl;

    int id = ui::getInt(ui::intltezeropred, ui::ltezeroretrymsg);

    try
    {
        Student student = system.getStudent(id);

        if (system.calculateStudentQCA(student))
        {
            student = system.getStudent(id); // make a call to the database again to get the QCA
            cout << "The QCA for student with ID " << id << " has been calculated successfully as " << student.getQCA() << endl;
        }
        else
        {
            cout << "The QCA for student with ID " << id << " could not be calculated at the moment, please try again later" << endl;
        }
    }
    catch (NotFoundException &nf)
    {
        cout << "The student with ID " << id << " was not found in the system, aborting...";
    }
}

void Administration::createLecturer()
{
    cout << "Please enter the Lecturer's first name: " << endl;

    string fname = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    cout << "Please enter the Lecturer's second name: " << endl;

    string sname = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    cout << "Please enter the Lecturer's age: " << endl;

    int age = ui::getInt(Predicate<int>([](const int &x) -> bool { return x < 24; }), "The age of the lecturer must be 24 or over, please try again: ");

    string fnameLower = fname;
    fnameLower[0] = tolower(fname[0]);
    string snameLower = sname;
    snameLower[0] = tolower(sname[0]);

    string email = fnameLower + "." + snameLower + "@staff.mail.ie";
    cout << "Please enter the department for which this Lecturer will teach in: " << endl;

    string department = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    try
    {
        string name = fname + " " + sname;
        Lecturer lecturer(name, age, email, department);

        cout << "Choose a password for this Lecturer: " << endl;

        string password = ui::getSecureString(ui::passlengthpred, ui::passlengthretrymsg);

        LecturerAccount account(lecturer, password);

        if (system.addLecturer(lecturer))
        {
            cout << "Lecturer " << name << " successfully added to system" << endl;
            system.addAccount(account);
        }
        else
        {
            cout << "Lecturer " << name << " not successfully added to the system, please try again later" << endl;
        }
    }
    catch (DuplicateException &d)
    {
        cout << "The Lecturer already exists, if you are trying to reset password, use Reset Password option, exiting Lecturer setup" << endl;
    }
}

void Administration::removeLecturer()
{
    cout << "Please enter the Lecturer email you wish to remove: " << endl;

    string email = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    try
    {
        Lecturer lecturer = system.getLecturer(email);

        if (lecturer == Lecturer::NOT_FOUND)
        {
            cout << "This Lecturer cannot be removed from the system, aborting..." << endl;
            return;
        }

        string removalMessage = "Removing a lecturer has the following effects:\n\t-All courses being lead by them will no longer have a course leader\n\t-ALl modules being taught by this lecturer will no longer have a lecturer\n\t-Any student registrations on those modules will be removed\n\t-All announcements made by the lecturer will be removed\n\t-The lecturer's account will be removed\n";
        bool remove = ui::confirmRemoval(removalMessage, 2);

        if (!remove)
        {
            cout << "Aborting removal of lecturer..." << endl;
            return;
        }

        LecturerAccount account = system.getLecturerAccount(email);
        system.removeAccount(account);

        if (system.removeLecturer(lecturer))
        {
            cout << "Lecturer " << email << " removed successfully from the system" << endl;
        }
        else
        {
            cout << "Lecturer " << email << " was not removed successfully from the system, please try again later" << endl;
        }
    }
    catch (NotFoundException &nf)
    {
        cout << "The Lecturer " << email << " does not exist, exiting Lecturer deletion" << endl;
    }
}

void Administration::resetLecturerPassword()
{
    cout << "Please enter the Lecturer email for whom to reset the password: " << endl;

    string email = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    try
    {
        Lecturer lecturer = system.getLecturer(email);

        LecturerAccount account = system.getLecturerAccount(email);

        cout << "Please enter the new password: " << endl;

        string password = ui::getSecureString(ui::passlengthpred, ui::passlengthretrymsg);

        account.setPassword(password);

        if (system.updateAccount(lecturer, account))
        {
            cout << "The password was updated successfully" << endl;
        }
        else
        {
            cout << "The password was not updated successfully, please try again later" << endl;
            cout << "This may be because there's no changes to update or an error occurred" << endl;
        }
    }
    catch (NotFoundException &nf)
    {
        cout << "The Lecturer " << email << " does not exist, exiting Lecturer password reset" << endl;
    }
}

void Administration::create()
{
    while (true)
    {
        cout << "Create (C)ourse, (M)odule, (S)tudent, (L)ecturer" << endl;

        string choice1 = ui::getChoice();

        if (choice1 == "C")
        {
            createCourse();
            break;
        }
        else if (choice1 == "M")
        {
            createModule();
            break;
        }
        else if (choice1 == "S")
        {
            createStudent();
            break;
        }
        else if (choice1 == "L")
        {
            createLecturer();
            break;
        }
    }
}

void Administration::remove()
{
    while (true)
    {
        cout << "Remove (C)ourse, (M)odule, (S)tudent, (L)ecturer" << endl;

        string choice1 = ui::getChoice();

        if (choice1 == "C")
        {
            removeCourse();
            break;
        }
        else if (choice1 == "M")
        {
            removeModule();
            break;
        }
        else if (choice1 == "S")
        {
            removeStudent();
            break;
        }
        else if (choice1 == "L")
        {
            removeLecturer();
            break;
        }
    }
}

void Administration::resetPassword()
{
    while (true)
    {
        cout << "Reset (L)ecturer/(S)tudent password" << endl;

        string choice1 = ui::getChoice();

        if (choice1 == "L")
        {
            resetLecturerPassword();
            break;
        }
        else if (choice1 == "S")
        {
            resetStudentPassword();
            break;
        }
    }
}

bool Administration::editModule()
{
    cout << "Enter the code of the module to edit: " << endl;

    string code = ui::getModuleCode();

    try
    {
        Module module = system.getModule(code);
        ModuleEditPage *editPage = new ModuleEditPage(module, system);
        ui::pageManager.setNextPage(editPage);
        return true;
    }
    catch (NotFoundException &nf)
    {
        cout << "Module with code " << code << " not found in system, aborting..." << endl;
        return false;
    }
}

bool Administration::editLecturer()
{
    cout << "Enter the e-mail address of the lecturer to update: " << endl;

    string email = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    try
    {
        Lecturer lecturer = system.getLecturer(email);
        LecturerEditPage *editPage = new LecturerEditPage(lecturer, system);
        ui::pageManager.setNextPage(editPage);
        return true;
    }
    catch (NotFoundException &nf)
    {
        cout << "Lecturer with email " << email << " not found in system, aborting..." << endl;
        return false;
    }
}

void Administration::show()
{
    bool run = true;

    if (!login())
    {
        cout << "You were not logged in successfully, so will be returned to the previous page\n"
             << endl;
        ui::pageManager.popCurrentPage();
        return;
    }

    cout << "\nWelcome to the Administration Panel\n"
         << endl;

    while (run)
    {
        cout << "(C)reate course/module/student/lecturer, (R)emove course/module/student/lecturer, Reset Student/lecturer (P)assword, Register (S)tudent on Module, Calculate Student QC(A), Edit (D)etails, (L)ogout, (Q)uit" << endl;

        string choice = ui::getChoice();

        if (choice == "C")
        {
            create();
        }
        else if (choice == "R")
        {
            remove();
        }
        else if (choice == "P")
        {
            resetPassword();
        }
        else if (choice == "S")
        {
            registerStudent();
        }
        else if (choice == "A")
        {
            calculateStudentQCA();
        }
        else if (choice == "D")
        {
            cout << "Edit (M)odule, (L)ecturer" << endl;

            string choice1 = ui::getChoice();

            if (choice1 == "M")
            {
                run = !editModule();
            }
            else if (choice1 == "L")
            {
                run = !editLecturer();
            }
        }
        else if (choice == "L")
        {
            run = false;
            loggedIn = false;
            ui::pageManager.popCurrentPage(); //get out of the administration page
        }
        else if (choice == "Q")
        {
            ui::quit();
        }
    }
}
