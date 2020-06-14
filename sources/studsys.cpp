#include "headers/DatabaseManager.h"
#include "headers/Student.h"
#include "headers/Course.h"
#include "headers/Lecturer.h"
#include "headers/Module.h"
#include "headers/Exam.h"
#include "headers/ExamGrade.h"
#include "headers/Warning.h"
#include "headers/WelcomePage.h"
#include "headers/LecturerAccount.h"
#include "headers/StudentAccount.h"

#include <vector>
#include <cstring>
#include <string>

using std::cout;
using std::boolalpha;
using std::cin;
using std::endl;
using std::string;

int main(int argc, char **argv)
{
    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0) {
            cout << "Login as a Lecturer to access lecturer resources, student to access student resources" << endl;
            cout << "If you are administration you have access to a hidden command, which is:\n(A)dministration\nContact your administration supervisor for login details" << endl;
            cout << "Otherwise, instructions are very clear" << endl;

            exit(0);
        }
    }

    StudentSystem system;
    //have a UI class/library which holds globals/data members and have ui.start() to abstract out that you have a welcome screen like her
    WelcomePage welcome(system);
    welcome.show();

    return 0;
}