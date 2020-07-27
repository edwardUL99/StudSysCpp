#include "headers/ExamPage.h"
#include "headers/UIUtils.h"
#include <cstring>

using std::map;
using std::string;
using ui::ExamPage;

ExamPage::ExamPage(Student student, Exam exam, StudentSystem &system, bool allowReverse) : Page(system), student(student), exam(exam), answers(student, exam), allowReverse(allowReverse)
{
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

    int choice = ui::getInt(Predicate<int>([numberOfAnswers](const int &x) -> bool { return x <= 0 || x > numberOfAnswers }), "Choice must be between 1 and " << numberOfAnswers << ", please try again: ");

    this->answers.setAnswer(examQuestion, answers[choice - 1]);
}

void ExamPage::printPrompt()
{
    if (allowReverse)
        cout << "(A)nswer current question, (N)ext question, (P)revious question, (S)ubmit for grading, (C)ancel" << endl;
    else
        cout << "(A)nswer current question, (N)ext question, (S)ubmit for grading, (C)ancel" << endl;
}

void ExamPage::answerCurrentQuestion(int index, const std::vector<ExamQuestion> &questions)
{
    if (index == questions.size())
    {
        if (allowReverse)
            cout << "You are at the end of the exam, to answer last question again, click P, then answer" << endl;
        else 
            cout << "You are at the end of the exam" << endl;
        return;
    }

    displayQuestion(questions[index]);
}

void ExamPage::nextQuestion(int &index, const std::vector<ExamQuestion> &questions, bool &answerable)
{
    if (index != questions.size())
    {
        index++;
        cout << "Next question--->" << endl;
        answerable = true;
    }
    else
    {
        cout << "You are on the last question" << endl;
        answerable = false;
    }
}

void ExamPage::previousQuestion(int &index, const std::vector<ExamQuestion> &questions)
{
    if (index != 0)
    {
        index--;
        cout << "<---Previous question" << endl;
    }
    else
    {
        cout << "You are on the first question" << endl;
    }
}

bool ExamPage::submitExam()
{
    while (true)
    {
        cout << "Are you sure you want to submit the exam? You will not be able to go back and change answers. (Y/N)" << endl;

        string choice = ui::getChoice();

        if (choice == "Y")
        {
            ExamGrade grade = this->answers.getExamGrade(); //calculate the exam grade
            this->system.addExamGrade(grade);

            ui::pageManager.popCurrentPage();
            return true;
        }
        else if (choice == "N")
        {
            return false;
        }
    }
}

bool ExamPage::cancelExam()
{
    while (true)
    {
        cout << "Are you sure you want to cancel your exam and lose your progress? (Y/N)" << endl;

        string choice = ui::getChoice();

        if (choice == "Y")
        {
            ui::pageManager.popCurrentPage();
            return true;
        }
        else if (choice == "N")
        {
            return false;
        }
    }
}

void ExamPage::show()
{
    cout << exam.getName() << endl;
    bool run = true;
    std::vector<ExamQuestion> questions = exam.getQuestions();
    int index = 0; //used to maintain the position in the exam

    while (run)
    {
        printPrompt();
        string choice = ui::getChoice();

        if (choice == "A")
        {
            answerCurrentQuestion(index, questions);
        }
        else if (choice == "N")
        {
            nextQuestion(index, questions);
        }
        else if (choice == "P" && allowReverse)
        {
            previousQuestion(index, questions);
        }
        else if (choice == "S")
        {
            run = !submitExam();
        }
        else if (choice == "C")
        {
            run = !cancelExam();
        }
    }
}
