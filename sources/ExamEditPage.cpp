#include "headers/ExamEditPage.h"
#include "headers/UIUtils.h"
#include "headers/studsys/NotFoundException.h"
#include "headers/studsys/KeyMismatch.h"

using std::string;
using std::vector;
using ui::ExamEditPage;

ExamEditPage::ExamEditPage(const Exam &exam, StudentSystem &system) : Page(system), exam(exam), newExam(exam) /*before any edits are made the newExam is the same as the old one*/ {}

ExamQuestion &ExamEditPage::chooseExamQuestion(vector<ExamQuestion> &questions)
{
    cout << "Please choose a question to edit: " << endl;
    int size = questions.size();

    for (int i = 0; i < size; i++)
    {
        cout << i + 1 << ") " + questions[i].getQuestion() << endl;
    }

    int choice = ui::getInt(Predicate<int>([size](const int &x) -> bool { return x < 1 || x > size; }), "Please enter a number between 1 and " + std::to_string(size) + ": ");

    return questions[choice - 1];
}

void ExamEditPage::editAnswerKey(ExamQuestion &question, vector<ExamAnswer> &answers)
{
    cout << "Choose the new answer key: " << endl;
    int oldKey = 0;
    int numAnswers = answers.size();

    for (int i = 0; i < numAnswers; i++)
    {
        ExamAnswer &answer = answers[i];
        if (answer.isKey())
            oldKey = i;
        cout << i + 1 << ")" << answer.getAnswer() << endl;
    }

    int key = ui::getInt(Predicate<int>([numAnswers](const int &x) -> bool { return x < 1 || x > numAnswers; }), "Please enter a number between 1 and " + std::to_string(numAnswers) + ": ");

    ExamAnswer &answerKey = answers[key - 1];
    answerKey.setKey(true);
    question.setKey(answerKey);
    if (key - 1 != oldKey)
        answers[oldKey].setKey(false);
}

void ExamEditPage::editAnswers(ExamQuestion &question, vector<ExamAnswer> &answers)
{
    int numAnswers = answers.size();

    cout << "Change the answer, or leave blank to stay the same: " << endl;

    for (int i = 0; i < numAnswers; i++)
    {
        ExamAnswer &answer = answers[i];
        cout << i + 1 << ")" << answer.getAnswer() << endl;
        string newAns = ui::getString();
        if (newAns != "")
        {
            answer = ExamAnswer(newExam.getID(), question.getNumber(), newAns, answer.isKey());
        }
    }
}

void ExamEditPage::editAnswers(ExamQuestion &question)
{
    while (true)
    {
        vector<ExamAnswer> answers = question.getPossibleAnswers();
        editAnswers(question, answers);
        editAnswerKey(question, answers);
        string ch = "";

        do
        {
            cout << "Do you want to submit these changes? (Y/N)?" << endl;

            ch = ui::getChoice();
        } while (ch != "N" && ch != "Y");

        if (ch == "Y")
        {
            question.setPossibleAnswers(answers);
            break;
        }
        else
        {
            break;
        }
    }
}

bool ExamEditPage::submitUpdates(vector<ExamQuestion> &questions)
{
    string consent = "";

    do
    {
        cout << "Do you want to keep editing the questions? (Y/N)" << endl;

        consent = ui::getChoice();
    } while (consent != "Y" && consent != "N");

    if (consent == "Y")
    {
        return false;
    }
    else
    {
        newExam.setQuestions(questions);
        return true;
    }
}

void ExamEditPage::updateQuestions()
{
    bool run = true;
    vector<ExamQuestion> questions = newExam.getQuestions();

    while (run)
    {
        ExamQuestion &question = chooseExamQuestion(questions);

        while (true)
        {
            cout << "Would you like to edit the (Q)uestion or (A)nswers; (C)ancel" << endl;

            string ch = ui::getChoice();

            if (ch == "Q")
            {
                cout << "Please enter the new question: " << endl;

                question.setQuestion(ui::getString(ui::emptystrpred, ui::emptystrretrymsg));
            }
            else if (ch == "A")
            {
                editAnswers(question);
            }
            else if (ch == "C")
            {
                break;
            }
        }

        run = !submitUpdates(questions);
    }
}

void ExamEditPage::editModule()
{
    cout << "Please enter the new module code: " << endl;

    string code = ui::getModuleCode();

    try
    {
        Module module = this->system.getModule(code);

        newExam.setModule(module);
    }
    catch (NotFoundException &nf)
    {
        cout << "The module " << code << " does not exist" << endl;
    }
}

void ExamEditPage::editName()
{
    cout << "Please enter the new name: " << endl;

    newExam.setName(ui::getString(ui::emptystrpred, ui::emptystrretrymsg));
}

void ExamEditPage::editYear()
{
    cout << "Please enter the new year: " << endl;

    newExam.setYear(ui::getInt(ui::intltezeropred, ltezeroretrymsg));
}

void ExamEditPage::editSemester()
{
    cout << "Please enter the new semester: " << endl;

    newExam.setSemester(ui::getInt(Predicate<int>([](const int &x) -> bool { return x < 1 || x > 2; }), "Please enter a number between 1 and 2: "));
}

void ExamEditPage::editTotalWeight()
{
    cout << "Please enter the new semester: " << endl;

    newExam.setSemester(ui::getInt(Predicate<int>([](const int &x) -> bool { return x < 1 || x > 2; }), "Please enter a number between 1 and 2: "));
}

void ExamEditPage::submitChanges()
{
    cout << "Submitting changes" << endl;

    try
    {
        if (this->system.updateExam(exam, newExam))
        {
            cout << "The changes have been submitted successfully" << endl;
        }
        else
        {
            cout << "The changes have not been successfully submitted, try again later" << endl;
            cout << "This may be because there's no changes to update or an error occurred" << endl;
        }
    }
    catch (KeyMismatch &km)
    {
        cout << "An error occurred, try again later" << endl;
    }

    ui::pageManager.popCurrentPage();
}

void ExamEditPage::show()
{
    cout << "Welcome to the Edit page for Exam " << exam.getName() << endl;

    bool run = true;

    while (run)
    {
        cout << "Edit: (M)odule, (N)ame, (Y)ear, (S)emester, (Q)uestions, (T)otal Weight; Submit (E)dits, (C)ancel" << endl;

        string choice = ui::getChoice();

        if (choice == "M")
        {
            editModule();
        }
        else if (choice == "N")
        {
            editName();
        }
        else if (choice == "Y")
        {
            editYear();
        }
        else if (choice == "S")
        {
            editSemester();
        }
        else if (choice == "Q")
        {
            updateQuestions();
        }
        else if (choice == "T")
        {
            editTotalWeight();
        }
        else if (choice == "E")
        {
            submitChanges();
            run = false;
        }
        else if (choice == "C")
        {
            run = false;
            ui::pageManager.popCurrentPage(); //get out of this paege
        }
    }
}