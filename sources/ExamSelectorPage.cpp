#include "headers/ExamSelectorPage.h"
#include "headers/ExamCreatePage.h"
#include "headers/studsys/StudentAccount.h"
#include "headers/ExamPage.h"
#include "headers/ExamEditPage.h"
#include "headers/studsys/Exam.h"
#include "headers/studsys/NotFoundException.h"
#include "headers/UIUtils.h"

using std::string;
using std::vector;
using ui::ExamSelectorPage;

ExamSelectorPage::ExamSelectorPage(Account *account, Module *module, StudentSystem &system) : Page(system), account(account), module(module)
{
    initialiseExamList();
}

void ExamSelectorPage::initialiseExamList()
{
    exams.clear();
    for (const Exam &exam : this->system.retrieveExamsByModule(*module))
    {
        exams.push_back(exam);
    }
}

void ExamSelectorPage::displayExams()
{
    if (exams.size() != 0)
    {
        int i = 0;

        for (const Exam &exam : exams)
        {
            cout << ++i << ") " << exam.getName() << " - " << exam.getNumberOfQuestions() << " questions - " << exam.getTotalWeight() << " marks" << endl;
        }
    }
    else
    {
        cout << "There are currently no exams available" << endl;
    }
}

Exam ExamSelectorPage::getExam()
{
    int size = exams.size();
    if (size > 1)
    {
        string msg = "Please choose a number between 1 and " + std::to_string(size) + ": ";
        cout << msg << endl;

        int num = ui::getInt(Predicate<int>([size](const int &x) -> bool { return x < 1 || x > size; }), msg);

        return exams[num - 1];
    }
    else
    {
        cout << "Only 1 exam in module, choosing it";
        return exams[0];
    }
}

bool ExamSelectorPage::isLecturer()
{
    return dynamic_cast<LecturerAccount *>(account);
}

void ExamSelectorPage::printPrompt(bool lecturer)
{
    if (exams.size() == 0)
    {
        if (lecturer)
            cout << "(C)reate Exam, (B)ack, (Q)uit" << endl;
        else
            cout << "(B)ack, (Q)uit" << endl;
    }
    else
    {
        if (lecturer)
            cout << "(C)reate Exam, (D)elete Exam, (E)dit Exam, (B)ack, (Q)uit" << endl;
        else
            cout << "(T)ake exam, (B)ack, (Q)uit" << endl;
    }
}

bool ExamSelectorPage::takeExam()
{
    if (exams.size() != 0)
    {
        Exam exam = getExam();
        StudentAccount *studentAccount = dynamic_cast<StudentAccount *>(account);
        Student student = studentAccount->getStudent();

        try
        {
            ExamGrade grade = this->system.getExamGrade(student, exam);
            cout << "You already took this exam, so you cannot take it again. If this is an error, contact your lecturer" << endl;
            return false;
        }
        catch (NotFoundException &nf)
        {
            ExamPage *examPage = new ExamPage(student, exam, this->system);
            ui::pageManager.setNextPage(examPage);
            return true;
        }
    }

    return false;
}

bool ExamSelectorPage::editExam()
{
    if (exams.size() != 0)
    {
        Exam exam = getExam();

        if (!this->system.examTaken(exam))
        {
            ExamEditPage *editPage = new ExamEditPage(exam, system);
            ui::pageManager.setNextPage(editPage);
            return true;
        }
        else
        {
            cout << "This exam has already been taken by students so you cannot edit it" << endl;
            return false;
        }
    }

    return false;
}

bool ExamSelectorPage::deleteExam()
{
    if (exams.size() != 0)
    {
        Exam exam = getExam();
        string examDesc = exam.getName() + " " + std::to_string(exam.getYear());

        while (true)
        {
            cout << "Are you sure you want to delete the exam: " << examDesc << "? (Y/N)" << endl;

            string choice = ui::getChoice();

            if (choice == "Y")
            {
                if (system.removeExam(exam))
                {
                    cout << "Exam " << examDesc << " deleted successfully\n"
                         << endl;
                    return true;
                }
                else
                {
                    cout << "Exam " << examDesc << " not deleted successfully, please try again later\n"
                         << endl;
                    return false;
                }
            }
            else if (choice == "N")
            {
                return false;
            }
        }
    }

    return false;
}

void ExamSelectorPage::show()
{
    cout << "You are viewing the list of Exams for Module " << module->getCode() << ": " << endl;
    bool run = true;

    initialiseExamList();

    displayExams();

    bool lecturer = isLecturer();

    while (run)
    {
        printPrompt(lecturer);
        string choice = ui::getChoice();

        if (choice == "T" && !lecturer)
        {
            run = !takeExam();
        }
        else if (choice == "E" && lecturer)
        {
            run = !editExam();
        }
        else if (choice == "C" && lecturer)
        {
            ExamCreatePage *createPage = new ExamCreatePage(*this->module, this->system);
            ui::pageManager.addSharedEntity(createPage, module);
            ui::pageManager.setNextPage(createPage);
            break;
        }
        else if (choice == "D" && lecturer)
        {
            run = !deleteExam();
        }
        else if (choice == "B")
        {
            run = false;
            ui::pageManager.popCurrentPage(); //want to get out of this page to the previous one
        }
        else if (choice == "Q")
        {
            ui::quit();
        }
    }
}