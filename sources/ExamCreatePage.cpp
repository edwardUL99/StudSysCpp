#include "headers/ExamCreatePage.h"
#include "headers/UI.h"
#include "headers/NotFoundException.h"
#include <time.h>

using std::vector;
using std::string;

ExamCreatePage::ExamCreatePage(Module module, StudentSystem &system) : Page(system), module(module) {
    this->name = "";
    this->year = this->semester = this->numQuestions = 0;
    this->totalWeight = 0;
}

bool ExamCreatePage::checkPopulated() const {
    return name != "" && year != 0 && semester != 0 && numQuestions != 0 && totalWeight != 0 && questions.size() != 0;
}

void ExamCreatePage::edit() {
    //ui to enter edit mode
    bool run = true;
    bool numQuestChosen = false;

    while (run) {
        cout << "Enter: (N)ame, (Y)ear, (S)emester, (N)umber Of Questions, (T)otal weight, (Q)uestions, (C)ancel, (D)one" << endl;
        
        string choice = ui::getChoice();
        
        if (choice == "N") {
            cout << "Please enter the name of the exam: " << endl;

            string name = ui::getString();

            while (name == "") {
                cout << "Name cannot be blank, try again" << endl;
                name = ui::getString();
            }

            this->name = name;
        } else if (choice == "Y") {
            cout << "Please enter the year of the exam: " << endl;

            int year = ui::getInt();

            time_t cTime = time(NULL);
            tm *aTime = localtime(&cTime);

            int cYear = aTime->tm_year + 1900;

            while (year < cYear) {
                cout << "The year must be greater than or equal to this year, try again" << endl;
                year = ui::getInt();
            }

            this->year = year;
        } else if (choice == "S") {
            cout << "Please enter the semester(1-2): " << endl;

            int semester = ui::getInt();

            while (semester < 1 || semester > 2) {
                cout << "The semester can only be 1 or 2, try again: " << endl;
                semester = ui::getInt();
            }

            this->semester = semester;
        } else if (choice == "N") {
            cout << "Please enter the number of questions: " << endl;

            int numQuestions = ui::getInt();

            while (numQuestions <= 0) {
                cout << "Number of questions must be greater than 0, try again: " << endl;
                numQuestions = ui::getInt();
            }

            numQuestChosen = true;
            this->numQuestions = numQuestions;
        } else if (choice == "T") {
            cout << "Please enter the total weighting for this exam: " << endl;

            float totalWeight = ui::getFloat();

            if (totalWeight <= 0.00f) {
                cout << "The total weighting must be greater than 0.00, try again: " << endl;
                totalWeight = ui::getFloat();
            }

            this->totalWeight = totalWeight;
        } else if (choice == "Q") {
            if (!numQuestChosen) {
                cout << "You must enter the number of questions first" << endl;
            } else {
                createQuestions();
            }
        } else if (choice == "C" || choice == "D") {
            run = false;
        }
    }
}

vector<ExamAnswer> ExamCreatePage::createAnswers(ExamQuestion &question) {
    //ui to create and return answers
}

void ExamCreatePage::createQuestions() {
    //ui to create questions and answers and then populate the questions vector
} 

void ExamCreatePage::submit() {
    //return created exam here

    if (checkPopulated()) {
        Exam exam(this->module, this->name, this->year, this->semester, this->numQuestions, this->totalWeight, this->questions);

        int id = exam.getID();

        if (this->system.addExam(exam)) {
            cout << "Exam with ID " << id << " created successfully" << endl;
        } else {
            cout << "Exam with ID " << id << " not created successfully, please try again later" << endl;
        }
    } else {
        throw "Not enough information inserted";
    }
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
            submit();
        } else if (choice == "C") {
            run = false;
        } else if (choice == "Q") {
            ui::quit();
        }
    }
}