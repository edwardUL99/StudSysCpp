#include "headers/ModuleSelectorPage.h"
#include "headers/ModuleHomePage.h"
#include "headers/studsys/Module.h"
#include "headers/studsys/StudentAccount.h"
#include "headers/UIUtils.h"

using std::string;
using ui::ModuleSelectorPage;

ModuleSelectorPage::ModuleSelectorPage(Account *account, StudentSystem &system) : Page(system), account(account) {}

bool ModuleSelectorPage::isStudentAccount() const
{
    bool student = dynamic_cast<StudentAccount*>(this->account); 

    return student;
}

std::vector<Module> ModuleSelectorPage::getModuleList() const
{
    std::vector<Module> modules;

    if (isStudentAccount())
    {
        Student student = dynamic_cast<StudentAccount*>(this->account)->getStudent();

        modules = this->system.getStudentRegisteredModules(student);
    }
    else
    {
        Lecturer lecturer = dynamic_cast<LecturerAccount*>(this->account)->getLecturer();

        for (const Module &module : this->system.getModules())
        {
            if (module.getLecturer().getEmail() == lecturer.getEmail())
            {
                modules.push_back(module);
            }
        }
    }

    return modules;
}

void ModuleSelectorPage::displayRegisteredModules()
{
    if (isStudentAccount())
    {
        cout << "List of modules you are registered for: " << endl;
    }
    else
    {
        cout << "List of modules you are registered as the lecturer for: " << endl;
    }

    std::vector<Module> modules = getModuleList();

    for (int i = 0; i < modules.size(); i++)
    {
        cout << i + 1 << ") " << modules[i].getCode() << " - " << modules[i].getName() << endl;
    }
}

bool ModuleSelectorPage::viewModule()
{
    std::vector<Module> modules = getModuleList();

    int numModules = modules.size();

    if (numModules > 0)
    {
        Module *module = NULL;
        if (numModules == 1) {
            cout << "Only 1 module available, choosing it" << endl;
            module = new Module(modules[0]);
        } else {
            cout << "Please choose a number between 1 and " << numModules << ": " << endl;

            int num = ui::getInt(Predicate<int>([numModules](const int &x) -> bool { return x < 1 || x > numModules; }), "Please re-enter a number between 1 and " + std::to_string(numModules) + ": ");
        }
        
        ModuleHomePage *modulePage = new ModuleHomePage(account, module, system);
        ui::pageManager.addSharedEntity(modulePage, module);
        ui::pageManager.setNextPage(modulePage);
        return true;
    }
    else
    {
        string msg = isStudentAccount() ? "your Lecturer or ITT" : "ITT";
        cout << "You are registered for no modules, if this is a mistake contact " << msg << endl;
        return false;
    }
}

void ModuleSelectorPage::show()
{
    displayRegisteredModules();
    bool run = true;

    while (run)
    {
        cout << "Please Choose a (M)odule to view, (C)ancel, (Q)uit: " << endl;

        string choice = ui::getChoice();

        if (choice == "M")
        {
            run = !viewModule();
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