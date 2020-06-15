#include "headers/Administration.h"
#include "headers/Module.h"
#include "headers/Student.h"
#include "headers/StudentAccount.h"
#include "headers/Lecturer.h"
#include "headers/LecturerAccount.h"
#include "headers/NotFoundException.h"
#include "headers/DuplicateException.h"
#include "headers/UI.h"

#include <cstring>
#include <string>
using std::string;

Administration::Administration(StudentSystem &studentSystem) : Page(studentSystem) {}

bool Administration::login()
{
    cout << "Please enter the admin username: " << endl;

    string username = ui::getString();

    if (username == "adminITT20")
    {
        cout << "Please enter the password: " << endl;

        string password = ui::getSecureString();

        if (password == "ittAdminPass20")
        {
            cout << "You are now logged in as admin" << endl;

            return true;
        }
        else
        {
            cout << "The password does not match" << endl;

            return false;
        }
    }
    else
    {
        cout << "The username is not correct" << endl;

        return false;
    }
}

void Administration::createCourse() {
    cout << "Please enter the course code: " << endl;

    string code = ui::getString(ui::mcodepred, ui::mcoderetrymsg);

    cout << "Please enter the course type: " << endl;

    string type = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    cout << "Please enter the name of the module: " << endl;

    string name = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    cout << "Please enter the duration of the course in years: " << endl;

    int duration = ui::getInt(ui::intltezeropred, ui::ltezeroretrymsg);

    cout << "Please enter the course leader e-mail address: " << endl;

    string email = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    try
    {
        Lecturer lecturer = system.getLecturer(system.getLecturerID(email));
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

void Administration::removeCourse() {
    cout << "Please enter the course code to remove: " << endl;

    string code = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    try
    {
        Course course = system.getCourse(code);

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

    string code = ui::getString(ui::mcodepred, ui::mcoderetrymsg);

    cout << "Please enter the module name: " << endl;

    string name = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    cout << "Please enter the number of credits: " << endl;

    int credits = ui::getInt(ui::intltezeropred, ui::ltezeroretrymsg);

    cout << "Please enter the lecturer's e-mail address who's in charge of the module: " << endl;

    string email = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    try
    {
        Lecturer lecturer = system.getLecturer(system.getLecturerID(email));
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

    string code = ui::getString(ui::mcodepred, ui::mcoderetrymsg);

    try
    {
        Module module = system.getModule(code);

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

    int age = ui::getInt([](const int &x) -> bool { return x < 17; }, "The age of the student must be 17 or over, please try again: ");

    string email = std::to_string(id) + "@student.mail.ie";

    cout << "Please enter the course code for which this Student will be enrolled in: " << endl;

    string code = ui::getString(ui::ccodepred, ui::ccoderetrymsg);

    try
    {
        Course course = system.getCourse(code);
        Student student(id, name, age, email, course);

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
        }
    }
    catch (NotFoundException &nf)
    {
        cout << "The Student " << id << " does not exist, exiting Student password reset" << endl;
    }
}

void Administration::createLecturer()
{
    cout << "Please enter the lecturer ID to be used for the new lecturer: " << endl;

    int id = ui::getInt(ui::intltezeropred, ui::ltezeroretrymsg);

    cout << "Please enter the Lecturer's first name: " << endl;

    string fname = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    cout << "Please enter the Lecturer's second name: " << endl;

    string sname = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

    cout << "Please enter the Lecturer's age: " << endl;

    int age = ui::getInt([](const int &x) -> bool { return x < 24; }, "The age of the lecturer must be 24 or over, please try again: ");
    
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
        Lecturer lecturer(id, name, age, email, department);

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
    cout << "Please enter the Lecturer ID you wish to remove: " << endl;

    int id = ui::getInt(ui::intltezeropred, ui::ltezeroretrymsg);

    try
    {
        Lecturer lecturer = system.getLecturer(id);
        LecturerAccount account = system.getLecturerAccount(id);
        string name = lecturer.getName();

        system.removeAccount(account);

        if (system.removeLecturer(lecturer))
        {
            cout << "Lecturer " << name << " removed successfully from the system" << endl;
        }
        else
        {
            cout << "Lecturer " << name << " was not removed successfully from the system, please try again later" << endl;
        }
    }
    catch (NotFoundException &nf)
    {
        cout << "The Lecturer " << id << " does not exist, exiting Lecturer deletion" << endl;
    }
}

void Administration::resetLecturerPassword()
{
    cout << "Please enter the Lecturer ID for whom to reset the password: " << endl;

    int id = ui::getInt(ui::intltezeropred, ui::ltezeroretrymsg);

    try
    {
        Lecturer lecturer = system.getLecturer(id);

        LecturerAccount account = system.getLecturerAccount(id);

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
        }
    }
    catch (NotFoundException &nf)
    {
        cout << "The Lecturer " << id << " does not exist, exiting Lecturer password reset" << endl;
    }
}

void Administration::show() {
    bool run = true;

    if (!login()) {
        cout << "You were not logged in successfully, so will be returned to the previous page" << endl;
        return;
    }

    cout << "\nWelcome to the Administration Panel\n" << endl;

    while (run) {
        cout << "(C)reate course/module/student/lecturer, (R)emove course/module/student/lecturer, Reset student/lecturer (P)assword, (L)ogout, (Q)uit" << endl;

        string choice = ui::getChoice();

        if (choice == "C") {
            while (true) {
                cout << "Create (C)ourse, (M)odule, (S)tudent, (L)ecturer" << endl;

                string choice1 = ui::getChoice();

                if (choice1 == "C") {
                    createCourse();
                    break;
                } else if (choice1 == "M") {
                    createModule();
                    break;
                } else if (choice1 == "S") {
                    createStudent();
                    break;
                } else if (choice1 == "L") {
                    createLecturer();
                    break;
                }
            }
        } else if (choice == "R") {
            while (true) {
                cout << "Remove (C)ourse, (M)odule, (S)tudent, (L)ecturer" << endl;

                string choice1 = ui::getChoice();

                if (choice1 == "C") {
                    removeCourse();
                    break;
                } else if (choice1 == "M") {
                    removeModule();
                    break;
                } else if (choice1 == "S") {
                    removeStudent();
                    break;
                } else if (choice1 == "L") {
                    removeLecturer();
                    break;
                }
            }
        } else if (choice == "P" || choice == "p") {
            while (true) {
                cout << "Reset (L)ecturer/(S)tudent password" << endl;

                string choice1 = ui::getChoice();

                if (choice1 == "L") {
                    resetLecturerPassword();
                    break;
                } else if (choice1 == "S") {
                    resetStudentPassword();
                    break;
                }
            }
        } else if (choice == "L") {
            run = false;
        } else if (choice == "Q") {
            ui::quit();
        }
    }
}