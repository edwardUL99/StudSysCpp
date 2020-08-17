#include "headers/ModuleHomePage.h"
#include "headers/ExamSelectorPage.h"
#include "headers/UIUtils.h"
#include "headers/Administration.h"
#include "headers/studsys/NotFoundException.h"
#include "headers/studsys/DuplicateException.h"
#include "headers/AnnouncementPage.h"

using std::string;
using std::vector;
using ui::ModuleHomePage;

ModuleHomePage::ModuleHomePage(Account *account, Module *module, StudentSystem &system) : Page(system), account(account), module(module) {}

void ModuleHomePage::deRegisterStudent(int id)
{
    try
    {
        Student student = this->system.getStudent(id);

        if (this->system.unregisterStudentModule(student, *module))
        {
            cout << "Student " << id << " de-registered from module " << module->getCode() << " successfully" << endl;
        }
        else
        {
            cout << "Student " << id << " de-registered from module " << module->getCode() << " unsuccessfully, please try again later" << endl;
        }
    }
    catch (NotFoundException &nf)
    {
        cout << "Student " << id << " does not exist" << endl;
    }
}

void ModuleHomePage::registerStudents()
{
    while (true)
    {
        cout << "Would you like to register a student on the module? (R)egister, (D)e-register, (N)o" << endl;

        string choice = ui::getChoice();

        if (choice == "R")
        {
            Administration admin(this->system);
            admin.registerStudent(module->getCode());
            break;
        }
        else if (choice == "D")
        {
            cout << "Enter the Student ID for the student to de-register: " << endl;

            int id = ui::getInt(ui::intltezeropred, ui::ltezeroretrymsg);

            deRegisterStudent(id);

            break;
        }
        else if (choice == "N")
        {
            break;
        }
    }
}

void ModuleHomePage::displayRegisteredStudents()
{
    cout << "Students registered on Module " << module->getCode() << ":" << endl;

    int i = 1;

    for (const Student &student : this->system.getStudentsRegisteredOnModule(*module))
    {
        cout << "\t" << i++ << ") " << student.getName() << " - " << student.getID() << " - " << student.getEmail() << endl;
    }

    bool lecturer = dynamic_cast<LecturerAccount*>(account);

    if (lecturer)
        registerStudents();
}

void ModuleHomePage::displayLecturerDetails()
{
    Lecturer lecturer = module->getLecturer();

    cout << "The lecturer for this module is: " << endl;
    cout << "\tName: " << lecturer.getName() << endl;
    cout << "\tDepartment: " << lecturer.getDepartment() << endl;
    cout << "\tE-mail Address: " << lecturer.getEmail() << endl;
}

void ModuleHomePage::show()
{
    string code = module->getCode();

    cout << "Welcome to the homepage for Module " << code << "\n"
         << endl;

    bool run = true;

    while (run)
    {
        cout << "View: (A)nnouncements, (T)asks, (E)xams, (S)tudent list, (L)ecturer details; Change (M)odule settings, (B)ack, (Q)uit" << endl;

        string choice = ui::getChoice();

        if (choice == "A")
        {
            AnnouncementPage *announcementPage = new AnnouncementPage(this->account, module, system);
            ui::pageManager.addSharedEntity(announcementPage, account);
            ui::pageManager.addSharedEntity(announcementPage, module);
            ui::pageManager.setNextPage(announcementPage);
            run = false;
        }
        else if (choice == "T")
        {
            cout << "Not implemented yet" << endl;
        }
        else if (choice == "E")
        {
            ExamSelectorPage *selectorPage = new ExamSelectorPage(this->account, this->module, this->system);
            ui::pageManager.addSharedEntity(selectorPage, account);
            ui::pageManager.addSharedEntity(selectorPage, module);
            ui::pageManager.setNextPage(selectorPage);
            run = false;
        }
        else if (choice == "S")
        {
            displayRegisteredStudents();
        }
        else if (choice == "L")
        {
            displayLecturerDetails();
        }
        else if (choice == "M")
        {
            cout << "Not implemented yet" << endl;
        }
        else if (choice == "B")
        {
            cout << "Going back to module selector page\n"
                 << endl;
            run = false;
            ui::pageManager.popCurrentPage();
        }
        else if (choice == "Q")
        {
            ui::quit();
        }
    }
}