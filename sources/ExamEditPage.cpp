#include "headers/ExamEditPage.h"
#include "headers/UIUtils.h"
#include "headers/NotFoundException.h"
#include "headers/KeyMismatch.h"
#include <vector>

using ui::ExamEditPage;
using std::string;
using std::vector;

ExamEditPage::ExamEditPage(const Exam &exam, StudentSystem &system) : Page(system), exam(exam), newExam(exam) /*before any edits are made the newExam is the same as the old one*/ {}

void ExamEditPage::updateQuestions() {
    bool run = true;

    vector<ExamQuestion> questions = newExam.getQuestions();
    int size = questions.size();

    while (run) {
        cout << "Please choose a question to edit: " << endl;

        for (int i = 0; i < size; i++) {
            cout << i + 1 << ") " + questions[i].getQuestion() << endl;
        }

        int choice = ui::getInt(Predicate<int>([size](const int &x) -> bool { return x < 1 || x > size; }), "Please enter a number between 1 and " + std::to_string(size) + ": ");

        ExamQuestion &question = questions[choice-1];

        while (true) {
            cout << "Would you like to edit the (Q)uestion or (A)nswers; (C)ancel" << endl;

            string ch = ui::getChoice();

            if (ch== "Q") {
                cout << "Please enter the new question: " << endl;

                question.setQuestion(ui::getString(ui::emptystrpred, ui::emptystrretrymsg));
            } else if (ch == "A") {
                while (true) {
                    vector<ExamAnswer> answers = question.getPossibleAnswers();
                    int numAnswers = answers.size();

                    cout << "Change the answer, or leave blank to stay the same: " << endl;
                   
                    for (int i = 0; i < numAnswers; i++) {
                        ExamAnswer &answer = answers[i];
                        cout << i + 1 << ")" << answer.getAnswer() << endl;
                        string newAns = ui::getString();
                        if (newAns != "") {
                            answer = ExamAnswer(newExam.getID(), question.getQuestion(), newAns, answer.isKey());    
                        }    
                    }

                    cout << "Choose the new answer key: " << endl;
                    
                    int oldKey = 0;
                    for (int i = 0; i < numAnswers; i++) {
                        ExamAnswer &answer = answers[i];
                        if (answer.isKey()) oldKey = i;
                        cout << i + 1 << ")" << answer.getAnswer() << endl;
                    }

                    int key = ui::getInt(Predicate<int>([numAnswers](const int &x) -> bool { return x < 1 || x > numAnswers; }), "Please enter a number between 1 and " + std::to_string(numAnswers) + ": ");

                    answers[key-1].setKey(true);
                    answers[oldKey].setKey(false);

                    ch = "";

                    do {
                        cout << "Do you want to submit these changes? (Y/N)?" << endl;

                        ch = ui::getChoice();
                    } while (ch != "N" && ch != "Y");

                    if (ch == "Y") {
                        question.setPossibleAnswers(answers);
                        break;
                    } else {
                        break;
                    }
                }
            } else if (ch == "C") {
                break;
            }
        }

        string consent = "";

        do {
            cout << "Do you want to keep editing the questions? (Y/N)" << endl;

            consent = ui::getChoice();
        } while (consent != "Y" && consent != "N");

        if (consent == "Y") {
        } else if (consent == "N") {
            run = false;
            newExam.setQuestions(questions);
        }
    }
}

void ExamEditPage::show() {
    cout << "Welcome to the Edit page for Exam " << exam.getName() << endl;

    bool run = true;

    while (run) {
        cout << "Edit: (M)odule, (N)ame, (Y)ear, (S)emester, (Q)uestions, (T)otal Weight; Submit (E)dits, (C)ancel" << endl;

        string choice = ui::getChoice();

        if (choice == "M") {
            cout << "Please enter the new module code: " << endl;

            string code = ui::getString(ui::emptystrpred, ui::emptystrretrymsg);
        
            try {
                Module module = this->system.getModule(code);

                newExam.setModule(module);
            } catch (NotFoundException &nf) {
                cout << "The module " << code << " does not exist" << endl;
            }
        } else if (choice == "N") {
            cout << "Please enter the new name: " << endl;

            newExam.setName(ui::getString(ui::emptystrpred, ui::emptystrretrymsg));
        } else if (choice == "Y") {
            cout << "Please enter the new year: " << endl;

            newExam.setYear(ui::getInt(ui::intltezeropred, ltezeroretrymsg));
        } else if (choice == "S") {
            cout << "Please enter the new semester: " << endl;

            newExam.setSemester(ui::getInt(Predicate<int>([](const int &x) -> bool { return x < 1 || x > 2; }), "Please enter a number between 1 and 2: "));
        } else if (choice == "Q") {
            updateQuestions();
        } else if (choice == "T") {
            cout << "Please enter the new total weight: " << endl;

            newExam.setTotalWeight(ui::getFloat(ui::floatltezeropred, ui::ltezeroretrymsg));
        } else if (choice == "E") {
            cout << "Submitting changes" << endl;

            try {
                if (this->system.updateExam(exam, newExam)) {
                    cout << "The changes have been submitted successfully" << endl;
                } else {
                    cout << "The changes have not been successfully submitted, try again later" << endl;
                }
            } catch (KeyMismatch &km) {
                cout << "An error occurred, try again later" << endl;
            }
        } else if (choice == "C") {
            run = false;
        }
    }
}