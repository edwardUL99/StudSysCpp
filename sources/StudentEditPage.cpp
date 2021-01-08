#include "headers/StudentEditPage.h"
#include "headers/studsys/Student.h"
#include "headers/studsys/Predicate.h"
#include "headers/studsys/NotFoundException.h"

using std::string;
using ui::StudentEditPage;

StudentEditPage::StudentEditPage(const Student &student, StudentSystem &system) : Page(system), oldStudent(student), newStudent(student) {}

void StudentEditPage::updateId()
{
    cout << "Please enter the new ID: " << endl;

    int id = ui::getInt(ui::intltezeropred, ui::ltezeroretrymsg);

    try
    {
        system.getStudent(id);
        // if we got to here without throwing, the student already exists
        cout << "Student with ID " << id << " already exists" << endl;

        return;
    }
    catch (NotFoundException &nf)
    {
        // ignore, we can continue since the student doesn't exist
    }

    newStudent.setID(id);
}

void StudentEditPage::updateName()
{
    cout << "Please enter the new name: " << endl;
    newStudent.setName(ui::getString(ui::emptystrpred, ui::emptystrretrymsg));
}

void StudentEditPage::updateAge()
{
    cout << "Please enter the new age: " << endl;
    newStudent.setAge(ui::getInt(ui::intltezeropred, ui::ltezeroretrymsg));
}

void StudentEditPage::updateCourse()
{
    cout << "Enter the code of the new course: " << endl;

    string code = ui::getCourseCode();
    try
    {
        Course course = system.getCourse(code);
        newStudent.setCourse(course);
    }
    catch (NotFoundException &nf)
    {
        cout << "Course with code " << code << " not found on the system" << endl;
    }
}

bool StudentEditPage::submitChanges()
{
    while (true)
    {
        cout << "Confirm Changes:\n\tID: " << newStudent.getID() << "\n\tName: " << newStudent.getName() << "\n\tAge: " << newStudent.getAge() << "\n\tCourse: " << newStudent.getCourse().getID() << "\n(Y/N): " << endl;

        string choice = ui::getChoice();

        if (choice == "Y")
        {
            bool success = system.updateStudent(oldStudent.getID(), newStudent);

            if (success)
            {
                cout << "Student updated successfully\n"
                     << endl;
            }
            else
            {
                cout << "Student not updated successfully, please try again later" << endl;
                cout << "This may be because there are no changes to be made or an error occurred\n"
                     << endl;
            }

            return true;
        }
        else
        {
            cout << "Not saving changes" << endl;
            return false;
        }
    }
}

void StudentEditPage::show()
{
    cout << "Welcome to the edit page for Student " << oldStudent.getID() << "\n"
         << endl;

    bool run = true;

    while (run)
    {
        cout << "Edit: (I)D, (N)ame, (A)ge, C(o)urse, (S)ubmit changes; (C)ancel, (Q)uit" << endl;

        string choice = ui::getChoice();

        if (choice == "I")
        {
            updateId();
        }
        else if (choice == "N")
        {
            updateName();
        }
        else if (choice == "A")
        {
            updateAge();
        }
        else if (choice == "O")
        {
            updateCourse();
        }
        else if (choice == "S")
        {
            run = !submitChanges();

            if (!run)
            {
                ui::pageManager.popCurrentPage();
            }
        }
        else if (choice == "C")
        {
            run = false;
            ui::pageManager.popCurrentPage();
        }
        else if (choice == "Q")
        {
            ui::quit();
        }
    }
}