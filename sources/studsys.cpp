#include "headers/studsys/DatabaseManager.h"
#include "headers/studsys/Student.h"
#include "headers/studsys/Course.h"
#include "headers/studsys/Lecturer.h"
#include "headers/studsys/Module.h"
#include "headers/studsys/Exam.h"
#include "headers/studsys/ExamGrade.h"
#include "headers/studsys/Warning.h"
#include "headers/WelcomePage.h"
#include "headers/studsys/LecturerAccount.h"
#include "headers/studsys/StudentAccount.h"
#include "headers/studsys/ConfigFileProcessor.h"
#include "headers/ExamEditPage.h"
#include "headers/ModuleHomePage.h"
#include "headers/UIUtils.h"

#include <vector>
#include <cstring>
#include <string>

using std::cout;
using std::boolalpha;
using std::cin;
using std::endl;
using std::string;
using ui::WelcomePage;

ui::PageManager ui::pageManager;

int main(int argc, char **argv)
{
    string dbname;
    string username;
    string password;
    string host;

    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0) {
            cout << "Usage 1: ./studsys -d database_name -u username -p password -h host" << endl;
            cout << "Usage 2: ./studsys login_file" << endl; 

            exit(0);
        } else if (argc == 2) {

            ConfigFileProcessor processor(argv[1], '=');

            if (processor.available()) {
                dbname = processor.getValue("database");
                username = processor.getValue("user");
                password = processor.getValue("pass");
                host = processor.getValue("host");
            } else {
                cout << "Error: " << argv[1] << " does not exist" << endl;
                exit(-1);
            }
        } else if (argc == 9) {
            if (strcmp(argv[1], "-d") == 0 && strcmp(argv[3], "-u") == 0 && strcmp(argv[5], "-p") == 0 && strcmp(argv[7], "-h") == 0) {
                dbname = argv[2];
                username = argv[4];
                password = argv[6];
                host = argv[8];
            } else {
	            cout << "Invalid arguments, run ./studsys -h for help" << endl;
                exit(-1);
            }
        } else {
	        cout << "Invalid arguments, run ./studsys -h for help" << endl;
            exit(-1);
        }
    } else {
        cout << "Invalid arguments, run ./studsys -h for help" << endl;
        exit(-1);
    }

    try {
        ui::pageManager.initializeSystem(dbname, username, password, host);
        ui::pageManager.start();
    } catch (SQLException &sq) {
        exit(1);
    } catch (ui::ExitSignal &ex) {}

    return 0;
}
