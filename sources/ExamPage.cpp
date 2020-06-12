#include "headers/ExamPage.h"
#include "headers/UI.h"
#include <cstring>

using std::map;
using std::string;

ExamPage::ExamPage(Student student, Exam exam, StudentSystem &system, bool allowReverse) : Page(system), student(student), exam(exam), answers(student, exam), allowReverse(allowReverse)
{
    qIterator = exam.getQuestions().begin();
}

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

    const std::vector<ExamQuestion> questions = exam.getQuestions();

    while (run) {
        if (allowReverse && answerable) cout << "(A)nswer current question, (N)ext question, (P)revious question, (S)ubmit for grading, (C)ancel" << endl;
        else if (answerable) cout << "(A)nswer current question, (N)ext question, (S)ubmit for grading, (C)ancel" << endl;
        else cout << "(S)ubmit for grading, (C)ancel" << endl;

        string choice = ui::getChoice();

        if (choice == "A" && answerable) {
            displayQuestion(*qIterator);
            if (qIterator != questions.end()) {
                qIterator++;
            } else {
                if (!allowReverse) {
                    answerable = false;
                }
            }
        } else if (choice == "N" && answerable) {
            if (qIterator != questions.end()) {
                qIterator++;
                cout << "Next question--->" << endl;
            } else {
                cout << "You are on the last question" << endl;
            }
        } else if (choice == "P" && answerable && allowReverse) {
            if (qIterator != questions.begin()) {
                qIterator--;
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
                    
                    run = false;
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
                    break;
                } else if (choice == "N") {
                    break;
                }
            }
        }
    }
}
