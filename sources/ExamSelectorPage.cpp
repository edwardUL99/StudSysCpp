#include "headers/ExamSelectorPage.h"
#include "headers/ExamCreatePage.h"
#include "headers/StudentAccount.h"
#include "headers/ExamPage.h"
#include "headers/ExamEditPage.h"
#include "headers/Exam.h"
#include "headers/NotFoundException.h"
#include "headers/UIUtils.h"

using ui::ExamSelectorPage;
using std::vector;
using std::string;

ExamSelectorPage::ExamSelectorPage(Account &account, Module &module, StudentSystem &system) : Page(system), account(account), module(module) {
    initialiseExamList();
}

void ExamSelectorPage::initialiseExamList() {
    for (const Exam &exam : this->system.retrieveExamsByModule(module)) {
        exams.push_back(exam);
    }
}

void ExamSelectorPage::show() {
    cout << "You are viewing the list of Exams for Module " << module.getCode() << ": " << endl;

    int size = exams.size();

    bool noExams = size == 0;

    if (!noExams) {
        int i = 0;

        for (const Exam &exam : exams) {
            cout << i + 1 << ") " << exam.getName() << " - " << exam.getNumberOfQuestions() << " questions - " << exam.getTotalWeight() << " marks" << endl;
            i++;
        }
    } else {
        cout << "There are currently no exams available" << endl;
    }

    bool run = true;

    bool lecturer = false;

    try {
        LecturerAccount &lectAcc = dynamic_cast<LecturerAccount&>(account);
        lecturer = true;
    } catch (std::bad_cast &bc) {}

    while (run) {
        if (noExams) {
           if (lecturer) cout << "(C)reate Exam, (B)ack, (Q)uit" << endl;
           else cout << "(B)ack, (Q)uit" << endl;
        } else { 
            if (lecturer) cout << "(C)reate Exam, (E)dit Exam, (B)ack, (Q)uit" << endl;
            else cout << "(T)ake exam, (B)ack, (Q)uit" << endl;
        }

        string choice = ui::getChoice();

        if (choice == "T" && !noExams && !lecturer) {
            string msg = "Please choose a number between 1 and " + std::to_string(size) + ": ";
            cout << msg << endl;

            int num = ui::getInt(Predicate<int>([size](const int &x) -> bool { return x < 1 || x > size; }), msg);

            StudentAccount &studentAccount = dynamic_cast<StudentAccount&>(account);
            Student student = studentAccount.getStudent();
            Exam exam = exams[num - 1];

            bool tookExam = false;

            try {
                ExamGrade grade = this->system.getExamGrade(student, exam);
                tookExam = true;
            } catch (NotFoundException &nf) {}
            
            if (!tookExam) {
                ExamPage examPage(student, exam, this->system);
                examPage.show();
            } else {
                cout << "You already took this exam, so you cannot take it again. If this is an error, contact your lecturer" << endl;
            }
        } else if (choice == "E" && !noExams && lecturer) {
            string msg = "Please choose a number between 1 and " + std::to_string(size) + ": ";
            cout << msg << endl;

            int num = ui::getInt(Predicate<int>([size](const int &x) -> bool { return x < 1 || x > size; }), msg);
            Exam &exam = exams[num-1];

            if (!this->system.examTaken(exam)) {
                ExamEditPage editPage(exams[num-1], system);
                editPage.show();
            } else {
                cout << "This exam has already been taken by students so you cannot edit it" << endl;
            }
        } else if (choice == "C" && lecturer) {
            ExamCreatePage createPage(this->module, this->system);
            createPage.show();
        } else if (choice == "B") {
            run = false;
        } else if (choice == "Q") {
            ui::quit();
        }
    }
}