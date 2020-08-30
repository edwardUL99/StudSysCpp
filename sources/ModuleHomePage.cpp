#include "headers/ModuleHomePage.h"
#include "headers/ExamSelectorPage.h"
#include "headers/UIUtils.h"
#include "headers/studsys/NotFoundException.h"
#include "headers/studsys/DuplicateException.h"
#include "headers/AnnouncementPage.h"
#include "headers/ModuleSettingsPage.h"

using std::string;
using std::vector;
using ui::ModuleHomePage;

ModuleHomePage::ModuleHomePage(Account *account, Module *module, StudentSystem &system) : Page(system), account(account), module(module) {}

void ModuleHomePage::displayRegisteredStudents()
{
    cout << "Students registered on Module " << module->getCode() << ":" << endl;

    int i = 1;

    for (const Student &student : this->system.getStudentsRegisteredOnModule(*module))
    {
        cout << "\t" << i++ << ") " << student.getName() << " - " << student.getID() << " - " << student.getEmail() << endl;
    }

    bool lecturer = dynamic_cast<LecturerAccount *>(account);

    if (lecturer)
        cout << "\nTo add/remove students from the module, go to the Settings page" << endl;
}

void ModuleHomePage::displayLecturerDetails()
{
    Lecturer lecturer = module->getLecturer();

    cout << "The lecturer for this module is: " << endl;
    cout << "\tName: " << lecturer.getName() << endl;
    cout << "\tDepartment: " << lecturer.getDepartment() << endl;
    cout << "\tE-mail Address: " << lecturer.getEmail() << endl;
}

void ModuleHomePage::printPrompt() const
{
    if (isLecturerAccount())
    {
        cout << "View: (A)nnouncements, (E)xams, (S)tudent list, (L)ecturer details, (G)rades, Change (M)odule settings, (B)ack, (Q)uit" << endl;
    }
    else
    {
        cout << "View: (A)nnouncements, (E)xams, (S)tudent list, (L)ecturer details, (G)rades, (B)ack, (Q)uit" << endl;
    }
}

bool ModuleHomePage::isLecturerAccount() const
{
    return dynamic_cast<LecturerAccount *>(account);
}

vector<ExamGrade> ModuleHomePage::getExamGradesByStudent(const vector<Exam> &exams, const Student &student)
{
    vector<ExamGrade> filtered;

    for (const Exam &exam : exams)
    {
        try
        {
            ExamGrade examGrade = system.getExamGrade(student, exam);
            filtered.push_back(examGrade); //if you got here without a NotFoundException, an exam grade for this exam exists for this student
        }
        catch (NotFoundException &nf)
        {
        }
    }

    return filtered;
}

vector<ExamGrade> ModuleHomePage::getExamGradesByExam(const Exam &exam, const std::vector<Student> &students)
{
    vector<ExamGrade> filtered;

    for (const Student &student : students)
    {
        try
        {
            ExamGrade examGrade = system.getExamGrade(student, exam);
        }
        catch (NotFoundException &nf)
        {
        }
    }

    return filtered;
}

void ModuleHomePage::viewExamGrades()
{
    bool lecturer = isLecturerAccount();

    vector<Exam> exams = system.retrieveExamsByModule(*module);

    if (lecturer)
    {
        vector<Student> students = system.getStudentsRegisteredOnModule(*module);

        for (const Exam &exam : exams)
        {
            cout << exam.getDescription() << ", Total Weight: " << exam.getTotalWeight() << endl;

            vector<ExamGrade> examGrades = getExamGradesByExam(exam, students);

            cout << "Student Results: " << endl;
            if (examGrades.size() > 0)
            {
                for (const ExamGrade &examGrade : examGrades)
                {
                    try
                    {
                        float grade = examGrade.getGrade();
                        float totalWeight = examGrade.getExam().getTotalWeight();
                        cout << "\tStudent ID: " << examGrade.getStudent().getID() << ", Mark: " << grade << "/" << totalWeight << ", Percentage: " << (grade / totalWeight) * 100 << endl;
                    }
                    catch (NotFoundException &nf)
                    {
                    }
                }
            } else {
                cout << "\tNo results available for this exam" << endl;
            }
        }
    }
    else
    {
        StudentAccount *studentAccount = dynamic_cast<StudentAccount *>(account);
        Student student = studentAccount->getStudent();

        vector<ExamGrade> examGrades = getExamGradesByStudent(exams, student);

        if (examGrades.size() > 0)
        {
            cout << student.getDescription() << endl;

            for (const ExamGrade &examGrade : examGrades)
            {
                try
                {
                    float grade = examGrade.getGrade();
                    float totalWeight = examGrade.getExam().getTotalWeight();
                    cout << "\t" << examGrade.getDescription() << ", Mark: " << grade << "/" << totalWeight << ", Percentage: " << (grade / totalWeight) * 100 << endl;
                }
                catch (NotFoundException &nf)
                {
                }
            }
        }
        else
        {
            cout << "There are no exam grades to view" << endl;
        }
    }
}

void ModuleHomePage::displayGradesInfo()
{
    string prompt = "View: (E)xam grades, (C)ancel, (Q)uit";

    while (true)
    {
        cout << prompt << endl;

        string choice = ui::getChoice();

        if (choice == "E")
        {
            viewExamGrades();
        }
        else if (choice == "C")
        {
            break;
        }
        else if (choice == "Q")
        {
            ui::quit();
        }
    }
}

void ModuleHomePage::show()
{
    string code = module->getCode();

    cout << "Welcome to the homepage for Module " << code << "\n"
         << endl;

    bool run = true;

    while (run)
    {
        printPrompt();

        string choice = ui::getChoice();

        if (choice == "A")
        {
            AnnouncementPage *announcementPage = new AnnouncementPage(this->account, module, system);
            ui::pageManager.addSharedEntity(announcementPage, account);
            ui::pageManager.addSharedEntity(announcementPage, module);
            ui::pageManager.setNextPage(announcementPage);
            run = false;
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
        else if (choice == "G")
        {
            displayGradesInfo();
        }
        else if (choice == "M" && isLecturerAccount())
        {
            ModuleSettingsPage *settingsPage = new ModuleSettingsPage(module, dynamic_cast<LecturerAccount *>(account), system);
            ui::pageManager.addSharedEntity(settingsPage, module);
            ui::pageManager.addSharedEntity(settingsPage, account);
            ui::pageManager.setNextPage(settingsPage);
            run = false;
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