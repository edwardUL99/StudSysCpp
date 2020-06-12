#include "headers/ExamCreatePage.h"
#include "headers/UI.h"

using std::vector;
using std::string;

ExamCreatePage::ExamCreatePage(StudentSystem &system) : Page(system) {}

void ExamCreatePage::edit() {
    //ui to enter edit mode
}

vector<ExamAnswer> ExamCreatePage::createAnswers() {
    //ui to create and return answers
}

void ExamCreatePage::createQuestions() {
    //ui to create questions and answers and then populate the questions vector
}

Exam ExamCreatePage::getCreatedExam() const {
    //return created exam here
}

void ExamCreatePage::show() {
    cout << "Welcome to the facility to create an exam\n" << endl;

    bool run = true;

    while (run) {
        cout << "(E)dit exam, (S)ave exam, (C)ancel, (Q)uit" << endl;

        string choice = ui::getChoice();

        if (choice == "E") {
            cout << "You are now in edit mode" << endl;

            edit();
        } else if (choice == "S") {
            //submit to database
        } else if (choice == "C") {
            run = false;
        } else if (choice == "Q") {
            ui::quit();
        }
    }
}