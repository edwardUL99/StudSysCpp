#include <cstring>
#include <iostream>

#include <mysql/jdbc.h>
using namespace sql;

#define DB "student_sys"
#define DB_HOST "localhost"
#define DB_USER "eddy"
#define DB_PASS ""

using std::cout;
using std::endl;

int main(int argc, char **argv) {
    Driver *driver = get_driver_instance();
    Connection *connection = driver->connect(DB_HOST, DB_USER, DB_PASS);
    connection->setSchema(DB);

    Statement *stmt = connection->createStatement();

    if (argc == 1) {
        cout << "Arguments expected" << endl;
        exit(-1);   
    } else {
        if (argc >= 2) {
            char* arg = argv[1];
            
            if (strcmp(arg, "-c") == 0) {
                cout << "Clear database? (Y/N): " << endl;

                char ch = toupper(getchar());
                
                if (ch == 'Y') {
                    stmt->execute("SET foreign_key_checks = 0;");

                    stmt->executeUpdate("TRUNCATE TABLE lecturers;");
                    stmt->executeUpdate("TRUNCATE TABLE students;");
                    stmt->executeUpdate("TRUNCATE TABLE courses;");
                    stmt->executeUpdate("TRUNCATE TABLE modules;");
                    stmt->executeUpdate("TRUNCATE TABLE exams;");
                    //stmt->executeUpdate("TRUNCATE TABLE exam_answer_keys;");
                    //stmt->executeUpdate("TRUNCATE TABLE exam_questions;");
                    //stmt->executeUpdate("TRUNCATE TABLE exam_answers;");
                    stmt->executeUpdate("TRUNCATE TABLE exam_grades;");
                    stmt->executeUpdate("TRUNCATE TABLE module_grades;");
                    stmt->executeUpdate("TRUNCATE TABLE lecturer_accounts;");
                    stmt->executeUpdate("TRUNCATE TABLE student_accounts;");

                    stmt->execute("SET foreign_key_checks = 1;");

                    connection->close();

                    delete connection;
                    delete stmt;
                    
                    cout << "Cleared the entire database, exiting..." << endl;
                    exit(0);
                } else if (ch == 'N') {
                    cout << "Aborting..." << endl;
                    exit(0);
                } else {
                    cout << "Invalid input, aborting..." << endl;
                    exit(-1);
                }
            } //other commands here
        }
    }
}