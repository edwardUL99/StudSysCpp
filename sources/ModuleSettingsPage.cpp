#include "headers/ModuleSettingsPage.h"
#include "headers/Administration.h"
#include "headers/studsys/NotFoundException.h"
#include "headers/ModuleEditPage.h"

using std::string;
using ui::Administration;
using ui::ModuleSettingsPage;

ModuleSettingsPage::ModuleSettingsPage(Module *module, LecturerAccount *account, StudentSystem &system) : Page(system), module(module), account(account) {}

void ModuleSettingsPage::registerStudents()
{
    while (true)
    {
        cout << "Would you like to register a student on the module? (R)egister, (D)e-register, (N)o" << endl;

        string choice = ui::getChoice();

        if (choice == "R")
        {
            Administration::registerStudentOnModule(module->getCode(), system);
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

void ModuleSettingsPage::deRegisterStudent(int id)
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

void ModuleSettingsPage::editModule() {
    ModuleEditPage *editPage = new ModuleEditPage(*module, system);
    ui::pageManager.setNextPage(editPage);

}

void ModuleSettingsPage::show()
{
    bool run = true;

    cout << "Settings Page for module " << module->getCode() << "\n"
         << endl;

    while (run)
    {
        cout << "Choose: (S)tudent Management, (E)dit module details, (C)ancel, (Q)uit" << endl;

        string choice = ui::getChoice();

        if (choice == "S")
        {
            registerStudents();
        }
        else if (choice == "E")
        {
            editModule();
            run = false;
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