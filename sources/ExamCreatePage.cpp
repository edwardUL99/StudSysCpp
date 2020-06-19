#include "headers/ExamCreatePage.h"
#include "headers/UIUtils.h"
#include "headers/NotFoundException.h"
#include <time.h>

using std::vector;
using std::string;
using ui::ExamCreatePage;

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
        cout << "Enter: (N)ame, (Y)ear, (S)emester, Number (O)f Questions, (T)otal weight, (Q)uestions, (C)ancel, (D)one" << endl;
        
        string choice = ui::getChoice();
        
        if (choice == "N") {
            cout << "Please enter the name of the exam: " << endl;

            this->name = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);
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

            int semester = ui::getInt(Predicate<int>([](const int &x) -> bool { return x < 1 || x > 2; }), "The semester can only be 1 or 2, try again: ");

            this->semester = semester;
        } else if (choice == "O") {
            if (numQuestChosen) {
                cout << "You have already chosen the number of questions, if you continue, any entered questions will be cleared." << endl;

                bool dontContinue = false;

                while (true) {
                    cout << "Continue? (Y/N)" << endl;
                    
                    string choice = ui::getChoice();

                    if (choice == "Y") {
                        break;
                    } else if (choice == "N") {
                        dontContinue = true;
                        break;
                    }
                }

                if (dontContinue) continue;
            }
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

ExamAnswer ExamCreatePage::createAnswers(int examID, string question, int numberOfAnswers, vector<ExamAnswer> &answers) {
    //ui to create and return answers
    answers.clear();

    for (int i = 0; i < numberOfAnswers; i++) {
        cout << "Enter answer " << i + 1 << ": " << endl;
        string answer = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

        answers.push_back(ExamAnswer(examID, question, answer));
    }

    cout << "Choose which answer is the correct answer (1-" << numberOfAnswers << "): " << endl;

    int key = ui::getInt(Predicate<int>([numberOfAnswers](const int &x) -> bool { return x < 1 || x > numberOfAnswers; }), "You must choose an answer between 1 and " + std::to_string(numberOfAnswers));

    ExamAnswer &akey = answers[key - 1];
    akey.setKey(true);

    return akey;
}

void ExamCreatePage::createQuestions() {
    //ui to create questions and answers and then populate the questions vector
    int examID = Exam::getLastID(); //this will be the ID that will be used for this exam when it is created

    for (int i = 0; i < numQuestions; i++) {
        cout << "Please enter question " << i + 1 << ": " << endl;

        string question = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);

        cout << "Please enter the number of answers for this question: " << endl;

        int numAnswers = ui::getInt(Predicate<int>([](const int &x) -> bool { return x < 2; }), "You must have 2 or more answers, try again: ");
        
        vector<ExamAnswer> answers;

        cout << "Now you can choose the answers: " << endl;

        ExamAnswer key = createAnswers(examID, question, numAnswers, answers);
        
        this->questions.push_back(ExamQuestion(examID, question, key, answers, numAnswers));
    }
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