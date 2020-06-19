#include "headers/ModuleSelectorPage.h"
#include "headers/Module.h"
#include "headers/StudentAccount.h"
#include "headers/UIUtils.h"

using std::string;
using ui::ModuleSelectorPage;

ModuleSelectorPage::ModuleSelectorPage(Account &account, StudentSystem &system) : Page(system), account(account) {}

bool ModuleSelectorPage::isStudentAccount() const
{
    bool student = true;

    try
    {
        StudentAccount &sAccount = dynamic_cast<StudentAccount &>(this->account); //if this is a student account the catch wouldn't run so student is true, else it will throw bad cast and make student false meaning it's a lecturer account
    }
    catch (std::bad_cast &b)
    {
        student = false;
    }

    return student;
}

std::vector<Module> ModuleSelectorPage::getModuleList() const
{
    std::vector<Module> modules;

    if (isStudentAccount())
    {
        Student student = dynamic_cast<StudentAccount &>(this->account).getStudent();

        modules = this->system.getStudentRegisteredModules(student);
    }
    else
    {
        Lecturer lecturer = dynamic_cast<LecturerAccount &>(this->account).getLecturer();

        for (const Module &module : this->system.getModules())
        {
            if (module.getLecturer().getID() == lecturer.getID())
            {
                modules.push_back(module);
            }
        }
    }

    return modules;
}

void ModuleSelectorPage::show()
{
    if (isStudentAccount())
    {
        cout << "List of modules you are registered for: " << endl;
    }
    else
    {
        cout << "List of modules you are registered as the lecturer for: " << endl;
    }

    bool run = true;

    std::vector<Module> modules = getModuleList();

    for (int i = 0; i < modules.size(); i++)
    {
        cout << i + 1 << ") " << modules[i].getCode() << " - " << modules[i].getName() << endl;
    }

    while (run)
    {
        cout << "Please Choose a (M)odule to view, (C)ancel, (Q)uit: " << endl;

        string choice = ui::getChoice();

        if (choice == "M")
        {
            int numModules = modules.size();

            if (numModules > 0)
            {
                cout << "Please choose a number between 1 and " << numModules << ": " << endl;

                int num = ui::getInt(Predicate<int>([numModules](const int &x) -> bool { return x < 1 || x > numModules; }), "Please re-enter a number between 1 and " + std::to_string(numModules) + ": ");

                /*
            * ModuleHomePage modulePage(modules[num-1], account, system);
            * modulePage.show();
            * run = false;
            */
            }
            else
            {
                string msg = isStudentAccount() ? "your Lecturer or ITT":"ITT";
                cout << "You are registered for no modules, if this is a mistake contact " << msg << endl;
            }
        }
        else if (choice == "C")
        {
            run = false;
        }
        else if (choice == "Q")
        {
            ui::quit();
        }
    }
}