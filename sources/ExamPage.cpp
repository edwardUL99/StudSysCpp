#include "headers/ExamPage.h"
#include "headers/UIUtils.h"
#include <cstring>

using std::map;
using std::string;
using ui::ExamPage;

ExamPage::ExamPage(Student student, Exam exam, StudentSystem &system, bool allowReverse) : Page(system), student(student), exam(exam), answers(student, exam), allowReverse(allowReverse)
{}

void ExamPage::displayQuestion(const ExamQuestion &examQuestion)
{
    string question = examQuestion.getQuestion();

    cout << question << endl;

    int numberOfAnswers = examQuestion.getNumberOfAnswers();

    cout << "Choose from the following answers (1-" << numberOfAnswers << "): " << endl;

    int i = 0;

    std::vector<ExamAnswer> answers = examQuestion.getPossibleAnswers();

    for (const ExamAnswer &answer : answers)
    {
        cout << ++i << ") " << answer << endl;
    }
    cout << "Choose from 1 to " << numberOfAnswers << ":" << endl;

    int choice = ui::getInt();

    while (choice > numberOfAnswers || choice <= 0)
    {
        cout << "Choice must be between 1 and " << numberOfAnswers << ", please try again: " << endl;
        choice = ui::getInt();
    }

    this->answers.setAnswer(examQuestion, answers[choice - 1]);
}

void ExamPage::show() {
    cout << exam.getName() << endl;

    bool run = true;
    bool answerable = true;

    std::vector<ExamQuestion> questions = exam.getQuestions();

    int index = 0; //used to maintain the position in the exam

    while (run) {
        if (allowReverse && answerable) cout << "(A)nswer current question, (N)ext question, (P)revious question, (S)ubmit for grading, (C)ancel" << endl;
        else if (answerable) cout << "(A)nswer current question, (N)ext question, (S)ubmit for grading, (C)ancel" << endl;
        else cout << "(S)ubmit for grading, (C)ancel" << endl;

        string choice = ui::getChoice();

        if (choice == "A" && answerable) {
            if (index == questions.size()) {
                cout << "You are at the end of the exam, to answer last question again, click P, then answer" << endl;
                continue;
            }

            displayQuestion(questions[index]);
        } else if (choice == "N" && answerable) {
            if (index != questions.size()) {
                index++;
                cout << "Next question--->" << endl;
            } else {
                cout << "You are on the last question" << endl;
            }
        } else if (choice == "P" && answerable && allowReverse) {
            if (index != 0) {
                index--;
                cout << "<---Previous question" << endl;
            } else {
                cout << "You are on the first question" << endl;
            }
        } else if (choice == "S") {
            while (true) {
                cout << "Are you sure you want to submit the exam? You will not be able to go back and change answers. (Y/N)" << endl;

                string choice = ui::getChoice();

                 if (choice == "Y") {
                    ExamGrade grade = this->answers.getExamGrade(); //calculate the exam grade 
                    this->system.addExamGrade(grade);

                    run = false;
                    ui::pageManager.popCurrentPage();
                    break;
                } else if (choice == "N") {
                    break;
                }
            }
        } else if (choice == "C") {
            while (true) {
                cout << "Are you sure you want to cancel your exam and lose your progress? (Y/N)" << endl;

                string choice = ui::getChoice();

                if (choice == "Y") {
                    run = false;
                    ui::pageManager.popCurrentPage();
                    break;
                } else if (choice == "N") {
                    break;
                }
            }
        }
    }
}
