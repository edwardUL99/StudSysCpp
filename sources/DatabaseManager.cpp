#include "headers/DatabaseManager.h"
#include "headers/Warning.h"
#include "headers/Student.h"
#include "headers/Lecturer.h"
#include "headers/Course.h"
#include "headers/Module.h"
#include "headers/StudentRegistration.h"
#include "headers/Exam.h"
#include "headers/ExamGrade.h"
#include "headers/ModuleGrade.h"
#include "headers/LecturerAccount.h"
#include "headers/StudentAccount.h"
#include "headers/KeyMismatch.h"
#include "headers/ExamAnswer.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::map;
using std::ofstream;
using std::string;
using std::stringstream;
using std::vector;

const map<Tables, string> DatabaseManager::tableNames = {
    {LECTURERS, "lecturers"},
    {STUDENTS, "students"},
    {COURSES, "courses"},
    {MODULES, "modules"},
    {EXAMS, "exams"},
    {EXAM_GRADES, "exam_grades"},
    {MODULE_GRADES, "module_grades"},
    {LECTURER_ACCOUNTS, "lecturer_accounts"},
    {STUDENT_ACCOUNTS, "student_accounts"}};

DatabaseManager::DatabaseManager(string database, string url, string user, string pass)
{
    this->driver = get_driver_instance();
    this->connection = this->driver->connect(url, user, pass);
    this->connection->setSchema(database);
    //now connected to the database
    this->stmt = this->connection->createStatement();
    setLastExamID();
}

DatabaseManager::~DatabaseManager()
{
    this->connection->close();
    this->writeWarningsToLog();
    this->unlockTables();
    delete this->connection;
    delete this->stmt;
}

void DatabaseManager::setLastExamID()
{
    ResultSet *res = executeQuery("SELECT id FROM exams WHERE id >= ALL(SELECT id FROM exams);");

    int id = 0;
    if (res->next())
    {
        id = res->getInt("id") + 1;
        if (id == 0)
            id = 1;
    } else {
        id = 1;
    }

    Exam::setLastID(id); //set the last id to use with new exams
}

bool DatabaseManager::add(const Lecturer &lecturer)
{   
    this->lockTable(LECTURERS);

    string query = "INSERT INTO lecturers (id, name, age, email, department) VALUES (" + std::to_string(lecturer.getID()) + ", '" + lecturer.getName() + "', " + std::to_string(lecturer.getAge()) + ", '" + lecturer.getEmail() + "', '" + lecturer.getDepartment() + "');";

    try
    {
        this->stmt->execute(query);

        this->unlockTables();

        return true; //if you reached this line execute ran without any exceptions, so lecturer was added successfully. This is the same with other add methods
    }
    catch (SQLException &e)
    {
        Warning w(e.what(), query);
        this->warnings.push_back(w);
        
        this->unlockTables();
        
        return false;
    }
}

boost::optional<Lecturer> DatabaseManager::getLecturer(int id)
{
    ResultSet *res = executeQuery("SELECT * FROM lecturers WHERE id = " + std::to_string(id) + ";");

    if (res->next())
    {
        string name = res->getString("name");
        int age = res->getInt("age");
        string email = res->getString("email");
        string department = res->getString("department");

        delete res;

        return Lecturer(id, name, age, email, department);
    }

    delete res;

    return boost::none;
}

/*bool DatabaseManager::contains(const Lecturer &lecturer) {
    return getLecturer(lecturer.getID()).has_value();
}*/

bool DatabaseManager::contains(const DatabaseItem &item)
{
    Tables table = item.getTable();

    switch (table)
    {
    case LECTURERS:
    {
        const Lecturer &lecturer = dynamic_cast<const Lecturer &>(item);
        return getLecturer(lecturer.getID()).has_value();
    }
    case STUDENTS:
    {
        const Student &student = dynamic_cast<const Student &>(item);
        return getStudent(student.getID()).has_value();
    }
    case COURSES:
    {
        const Course &course = dynamic_cast<const Course &>(item);
        return getCourse(course.getID()).has_value();
    }
    case MODULES:
    {
        const Module &module = dynamic_cast<const Module &>(item);
        return getModule(module.getCode()).has_value();
    }
    case EXAMS:
    {
        const Exam &exam = dynamic_cast<const Exam &>(item);
        return getExam(exam.getID()).has_value();
    }
    case EXAM_GRADES:
    {
        const ExamGrade &examGrade = dynamic_cast<const ExamGrade &>(item);
        return getExamGrade(examGrade.getStudent(), examGrade.getExam()).has_value();
    }
    case MODULE_GRADES:
    {
        const ModuleGrade &moduleGrade = dynamic_cast<const ModuleGrade &>(item);
        return getModuleGrade(moduleGrade.getModule(), moduleGrade.getStudent()).has_value();
    }
    case LECTURER_ACCOUNTS:
    {
        const LecturerAccount &lecturerAccount = dynamic_cast<const LecturerAccount &>(item);
        return getLecturerAccount(lecturerAccount.getLecturer().getID()).has_value();
    }
    case STUDENT_ACCOUNTS:
    {
        const StudentAccount &studentAccount = dynamic_cast<const StudentAccount &>(item);
        return getStudentAccount(studentAccount.getStudent().getID()).has_value();
    }
    default:
        return false;
    }
}

vector<Lecturer> DatabaseManager::getAllLecturers()
{
    vector<Lecturer> lecturers;

    ResultSet *res = executeQuery("SELECT id FROM lecturers;");

    while (res->next())
    {
        lecturers.push_back(getLecturer(res->getInt("id")).get());
    }

    delete res;

    return lecturers;
}

bool DatabaseManager::remove(const Lecturer &lecturer)
{
    this->lockTable(LECTURERS);

    string email = "'" + lecturer.getEmail() + "'";
    bool removed = this->stmt->executeUpdate("DELETE FROM lecturers WHERE email = " + email + ";") != 0;

    this->unlockTables();

    return removed;
}

int DatabaseManager::getLecturerID(Lecturer &lecturer)
{
    string email = "'" + lecturer.getEmail() + "'";
    ResultSet *res = executeQuery("SELECT id FROM lecturers WHERE email = " + email + ";");

    int id = -1;

    while (res->next())
    {
        id = res->getInt("id");
        lecturer.setID(id);
    }

    delete res;

    return id;
}

bool DatabaseManager::update(int id, const Lecturer &updatedLecturer)
{
    int id1 = updatedLecturer.getID();

    if (id != id1)
    {
        throw KeyMismatch(std::to_string(id), std::to_string(id1));
    }
    else
    {
        this->lockTable(LECTURERS);

        string query = "UPDATE lecturers SET name = '" + updatedLecturer.getName() + "', age = " + std::to_string(updatedLecturer.getAge()) + ", email = '" + updatedLecturer.getEmail() + "', department = '" + updatedLecturer.getDepartment() + "' WHERE id = " + std::to_string(id) + ";";

        bool updated = executeUpdate(query) != 0;

        this->unlockTables();

        return updated;
    }
}

bool DatabaseManager::add(const Course &course)
{
    this->lockTable(COURSES);

    string query = "INSERT INTO courses (id, type, name, duration, course_leader) VALUES ('" + course.getID() + "', '" + course.getType() + "', '" + course.getName() + "', " + std::to_string(course.getDuration()) + ", " + std::to_string(course.getCourseLeader().getID()) + ");";

    try
    {
        this->stmt->execute(query);

        this->unlockTables();

        return true;
    }
    catch (SQLException &e)
    {
        Warning w(e.what(), query);
        this->warnings.push_back(w);

        this->unlockTables();
        return false;
    }
}

boost::optional<Course> DatabaseManager::getCourse(string id)
{
    ResultSet *res = executeQuery("SELECT * FROM courses WHERE id = '" + id + "';");

    if (res->next())
    {
        string type = res->getString("type");
        string name = res->getString("name");
        int duration = res->getInt("duration");
        boost::optional<Lecturer> lecturer = getLecturer(res->getInt("course_leader"));
        delete res;

        return Course(id, type, name, duration, lecturer.get());
    }

    delete res;

    return boost::none;
}

/*bool DatabaseManager::contains(const Course &course) {
    return getCourse(course.getID()).has_value();
}*/

vector<Course> DatabaseManager::getAllCourses()
{
    vector<Course> courses;

    ResultSet *res = executeQuery("SELECT id FROM courses;");

    while (res->next())
    {
        courses.push_back(getCourse(res->getString("id")).get());
    }

    delete res;

    return courses;
}

bool DatabaseManager::remove(const Course &course)
{
    this->lockTable(COURSES);

    string c = "'" + course.getID() + "'";
    bool removed = this->stmt->executeUpdate("DELETE FROM courses WHERE id = " + c + ";") != 0;

    this->unlockTables();

    return removed;
}

bool DatabaseManager::update(string id, const Course &updatedCourse)
{
    string id1 = updatedCourse.getID();

    if (id != id1)
    {
        throw KeyMismatch(id, id1);
    }
    else
    {
        this->lockTable(COURSES);

        string query = "UPDATE courses SET type = '" + updatedCourse.getType() + "', name = '" + updatedCourse.getName() + "', duration = " + std::to_string(updatedCourse.getDuration()) + ", course_leader = " + std::to_string(updatedCourse.getCourseLeader().getID()) + " WHERE id = '" + id1 + "';";

        bool updated = executeUpdate(query) != 0;

        this->unlockTables();

        return updated;
    }
}

bool DatabaseManager::add(const Student &student)
{
    this->lockTable(STUDENTS);

    string query = "INSERT INTO students (id, name, age, email, qca, course) VALUES (" + std::to_string(student.getID()) + ", '" + student.getName() + "', " + std::to_string(student.getAge()) + ", '" + student.getEmail() + "', " + std::to_string(student.getQCA()) + ", '" + student.getCourse().getID() + "');";

    try
    {
        this->stmt->execute(query);
        
        this->unlockTables();
        
        return true;
    }
    catch (SQLException &e)
    {
        Warning w(e.what(), query);
        this->warnings.push_back(w);

        this->unlockTables();

        return false;
    }
}

boost::optional<Student> DatabaseManager::getStudent(int id)
{
    ResultSet *res = executeQuery("SELECT * FROM students WHERE id = " + std::to_string(id) + ";");

    if (res->next())
    {
        string name = res->getString("name");
        int age = res->getInt("age");
        string email = res->getString("email");
        float qca = res->getDouble("qca");
        boost::optional<Course> course = getCourse(res->getString("course"));

        delete res;

        return Student(id, name, age, email, course.get(), qca);
    }

    delete res;

    return boost::none;
}

/*bool DatabaseManager::contains(const Student &student) {
    return getStudent(student.getID()).has_value();
}*/

vector<Student> DatabaseManager::getAllStudents()
{
    vector<Student> students;

    ResultSet *res = executeQuery("SELECT id FROM students;");

    while (res->next())
    {
        students.push_back(getStudent(res->getInt("id")).get());
    }

    delete res;

    return students;
}

bool DatabaseManager::remove(const Student &student)
{
    this->lockTable(STUDENTS);

    string email = "'" + student.getEmail() + "'";
    bool removed = this->stmt->executeUpdate("DELETE FROM students WHERE email = " + email + ";") != 0;

    this->unlockTables();

    return removed;
}

bool DatabaseManager::update(int id, const Student &updatedStudent)
{
    int id1 = updatedStudent.getID();

    if (id != id1)
    {
        throw KeyMismatch(std::to_string(id), std::to_string(id1));
    }
    else
    {
        this->lockTable(STUDENTS);

        string query = "UPDATE students SET name = '" + updatedStudent.getName() + "', age = " + std::to_string(updatedStudent.getAge()) + ", email = '" + updatedStudent.getEmail() + "', qca = " + std::to_string(updatedStudent.getQCA()) + ", course = '" + updatedStudent.getCourse().getID() + "' WHERE id = " + std::to_string(id1) + ";";

        bool updated = executeUpdate(query) != 0;

        this->unlockTables();
        return updated;
    }
}

int DatabaseManager::getStudentID(Student &student)
{
    string email = "'" + student.getEmail() + "'";
    ResultSet *res = executeQuery("SELECT id FROM lecturers WHERE email = " + email + ";");

    int id = -1;

    if (res->next())
    {
        id = res->getInt("id");
        student.setID(id);
    }

    delete res;

    return id;
}

bool DatabaseManager::add(const Module &module)
{
    this->lockTable(MODULES);

    string query = "INSERT INTO modules (code, name, credits, lecturer) VALUES ('" + module.getCode() + "', '" + module.getName() + "', " + std::to_string(module.getCredits()) + ", " + std::to_string(module.getLecturer().getID()) + ");";

    try
    {
        this->stmt->execute(query);

        this->unlockTables();

        return true;
    }
    catch (SQLException &e)
    {
        Warning w(e.what(), query);
        this->warnings.push_back(w);
        
        this->unlockTables();

        return false;
    }
}

boost::optional<Module> DatabaseManager::getModule(string code)
{
    ResultSet *res = executeQuery("SELECT * FROM modules WHERE code = '" + code + "';");

    if (res->next())
    {
        string name = res->getString("name");
        int credits = res->getInt("credits");
        boost::optional<Lecturer> lecturer = getLecturer(res->getInt("lecturer"));

        delete res;

        return Module(code, name, credits, lecturer.get());
    }

    delete res;

    return boost::none;
}

/*bool DatabaseManager::contains(const Module &module) {
    return getModule(module.getCode()).has_value();
}*/

vector<Module> DatabaseManager::getAllModules()
{
    vector<Module> modules;

    ResultSet *res = executeQuery("SELECT code FROM modules;");

    while (res->next())
    {
        modules.push_back(getModule(res->getString("code")).get());
    }

    delete res;

    return modules;
}

bool DatabaseManager::remove(const Module &module)
{
    this->lockTable(MODULES);

    string query = "DELETE FROM modules WHERE code = '" + module.getCode() + "';";
    
    bool removed = this->stmt->executeUpdate(query) != 0;

    this->unlockTables();

    return removed;
}

bool DatabaseManager::update(string code, const Module &updatedModule)
{
    string code1 = updatedModule.getCode();

    if (code != code1)
    {
        throw KeyMismatch(code, code1);
    }
    else
    {
        this->lockTable(MODULES);

        string query = "UPDATE modules SET name = '" + updatedModule.getName() + "', credits = " + std::to_string(updatedModule.getCredits()) + ", lecturer = " + std::to_string(updatedModule.getLecturer().getID()) + " WHERE code = '" + code1 + "';";

        bool updated = executeUpdate(query) != 0;

        this->unlockTables();

        return updated;
    }
}

bool DatabaseManager::add(const StudentRegistration &registration) {
    this->lockTable(STUDENT_REGISTRATIONS);

    string query = "INSERT INTO student_registrations (student, module) VALUES (" + std::to_string(registration.getStudent().getID()) + ", '" + registration.getModule().getCode() + "');"; 
    
    try {
        this->stmt->execute(query);

        this->unlockTables();

        return true;
    } catch (SQLException &e) {
        Warning w(e.what(), query);
        this->warnings.push_back(w);

        this->unlockTables();

        return false;
    }
}

bool DatabaseManager::remove(const StudentRegistration &registration) {
    this->lockTable(STUDENT_REGISTRATIONS);
    
    string query = "DELETE FROM student_registrations WHERE student = " + std::to_string(registration.getStudent().getID()) + " AND module = '" + registration.getModule().getCode() + "';";

    bool removed = this->executeUpdate(query) != 0;

    this->unlockTables();

    return removed;
}

boost::optional<StudentRegistration> DatabaseManager::getStudentRegistration(const Student &student, const Module &module) {
    string query = "SELECT * FROM student_registrations WHERE student = " + std::to_string(student.getID()) + " AND module = '" + module.getCode() + "';";

    ResultSet *res = executeQuery(query);

    if (res->next()) {
        Student s = getStudent(res->getInt("student")).get();
        Module m = getModule(res->getString("module")).get();

        return StudentRegistration(s, m);
    }

    delete res;

    return boost::none;
}

std::vector<StudentRegistration> DatabaseManager::getAllStudentRegistrations() {
    vector<StudentRegistration> registrations;

    string query = "SELECT * FROM student_registrations;";

    ResultSet *res = executeQuery(query);

    while (res->next()) {
        registrations.push_back(StudentRegistration(getStudent(res->getInt("student")).get(), getModule(res->getString("module")).get()));
    }

    delete res;

    return registrations;
}

bool DatabaseManager::add(const ExamAnswer &answer) {
    try {
        string query = "INSERT INTO exam_answers (exam, question, answer) VALUES (" + std::to_string(answer.getExamID()) + ", '" + answer.getQuestion() + "', '" + answer.getAnswer() + "');";
        this->stmt->execute(query);
        
        return true;
    } catch (SQLException &e) {
        throw e;
    }
}

bool DatabaseManager::add(const ExamQuestion &question) {
    try {
        string query = "INSERT INTO exam_questions (exam, question, answer_key, numberOfAnswers) VALUES (" + std::to_string(question.getExamID()) + ", '" + question.getQuestion() + "', '" + question.getKey().getAnswer() + "', " + std::to_string(question.getNumberOfAnswers()) + ");";
        this->stmt->execute(query);
        bool added = true; //if you reach this line, line above ran ssuccessfully

        for (const ExamAnswer &examAnswer : question.getPossibleAnswers())
        {   
            bool answerAdded = add(examAnswer);
            added = added && answerAdded;
        }

        return added;
    } catch (SQLException &e) {
        throw e;
    }
}

//CHECK SCHEMA TO SEE IF THE TABLES WORK I.E CLEAR EVERY TABLE AND REIMPORT TO CHECK FOR SYNTAX ERRORS
//this may not work, you'll have to edit getExam too but remove may be fine because you have cascades
bool DatabaseManager::add(const Exam &exam)
{
    this->lockTable(EXAMS);
    this->lockTable(EXAM_QUESTIONS);
    this->lockTable(EXAM_ANSWERS);

    string query = "INSERT INTO exams (id, module, name, year, semester, numberOfQuestions, weightPerQuestion, totalWeight) VALUES (" + std::to_string(exam.getID()) + ", '" + exam.getModule().getCode() + "', '" + exam.getName() + "', " + std::to_string(exam.getYear()) + ", " + std::to_string(exam.getSemester()) + ", " + std::to_string(exam.getNumberOfQuestions()) + ", " + std::to_string(exam.getWeightPerQuestion()) + ", " + std::to_string(exam.getTotalWeight()) + ");";

    try
    {
        this->stmt->execute(query);
        bool added = true;

        for (const ExamQuestion &examQuestion : exam.getQuestions())
        {
            bool questionAdded = add(examQuestion);
            added = added && questionAdded;
        }

        this->unlockTables();

        return added;
    }
    catch (SQLException &e)
    {
        Warning w(e.what(), query);
        this->warnings.push_back(w);

        this->unlockTables();

        return false;
    }
}

vector<ExamQuestion> DatabaseManager::getAllExamQuestions(int examID)
{
    vector<ExamQuestion> questions;

    ResultSet *res = executeQuery("SELECT * FROM exam_questions WHERE exam = " + std::to_string(examID) + ";");

    while (res->next())
    {
        string question = res->getString("question");
        string answer_key = res->getString("answer_key");
        int numberOfAnswers = res->getInt("numberOfAnswers");

        vector<ExamAnswer> answers;

        ResultSet *res1 = executeQuery("SELECT * FROM exam_answers WHERE exam = " + std::to_string(examID) + " AND question = '" + question + "';");

        while (res1->next())
        {
            string answer = res1->getString("answer");

            answers.push_back(ExamAnswer(examID, question, answer));
        }

        ExamQuestion examQuestion(examID, question, ExamAnswer(examID, question, answer_key, true), answers, numberOfAnswers);
        questions.push_back(examQuestion);

        delete res1;
    }

    delete res;

    return questions;
}

boost::optional<Exam> DatabaseManager::getExam(int id)
{
    ResultSet *res = executeQuery("SELECT * FROM exams WHERE id = " + std::to_string(id) + ";");

    if (res->next())
    {
        boost::optional<Module> module = getModule(res->getString("module"));
        string name = res->getString("name");
        int year = res->getInt("year");
        int semester = res->getInt("semester");
        int numQuestions = res->getInt("numberOfQuestions");
        float totalW = res->getDouble("totalWeight");

        vector<ExamQuestion> questions = this->getAllExamQuestions(id);

        delete res;

        return Exam(id, module.get(), name, year, semester, numQuestions, totalW, questions);
    }

    delete res;

    return boost::none;
}

/*bool DatabaseManager::contains(const Exam &exam) {
    return getExam(exam.getID()).has_value();
}*/

vector<Exam> DatabaseManager::getAllExams()
{
    vector<Exam> exams;

    ResultSet *res = executeQuery("SELECT id FROM exams;");

    while (res->next())
    {
        exams.push_back(getExam(res->getInt("id")).get());
    }

    delete res;

    return exams;
}

bool DatabaseManager::remove(const Exam &exam)
{
    this->lockTable(EXAMS);
    this->lockTable(EXAM_QUESTIONS);
    this->lockTable(EXAM_ANSWERS);

    string module = "'" + exam.getModule().getCode() + "'";
    string name = "'" + exam.getName() + "'";
    string numQuestions = std::to_string(exam.getNumberOfQuestions());
    string weight = std::to_string(exam.getWeightPerQuestion());
    string totalW = std::to_string(exam.getTotalWeight()); //most likely to be only one result if all attributes match

    bool deleted = this->stmt->executeUpdate("DELETE FROM exams WHERE module = " + module + " AND " + "name = " + name + " AND " + "numberOfQuestions = " + numQuestions + " AND " + "weightPerQuestion = " + weight + " AND " + "totalWeight = " + totalW + ";") != 0;

    this->unlockTables();

    return deleted;
}

//this may not be correct
bool DatabaseManager::update(const ExamAnswer &oldAnswer, const ExamAnswer &newAnswer) {
    string oid = std::to_string(oldAnswer.getExamID());
    string nid = std::to_string(newAnswer.getExamID());

    string query = "UPDATE exam_answers SET answer = '" + newAnswer.getAnswer() 
                  + "', WHERE exam = " + oid
                  + " AND question = '" + oldAnswer.getQuestion()
                  + "' AND answer = '" + oldAnswer.getAnswer()
                  + "';";

    return executeUpdate(query) != 0;
}

bool DatabaseManager::update(const ExamQuestion &oldQuestion, const ExamQuestion &newQuestion) {
    string oid = std::to_string(oldQuestion.getExamID());
    string nid = std::to_string(newQuestion.getExamID());
    bool updated = true;

    string query = "UPDATE exam_questions SET exam = " + nid
                  + ", question = '" + newQuestion.getQuestion()
                  + "', answer_key = '" + newQuestion.getKey().getAnswer()
                  + "', numberOfAnswers = " + std::to_string(newQuestion.getNumberOfAnswers())
                  + " WHERE exam = " + oid + " AND question = '" + oldQuestion.getQuestion() + "';";

    updated = updated && executeUpdate(query) != 0;

    vector<ExamAnswer> oldAnswers = oldQuestion.getPossibleAnswers();
    vector<ExamAnswer> newAnswers = newQuestion.getPossibleAnswers();

    int osize = oldAnswers.size(), nsize = newAnswers.size();

    if (osize <= nsize) {
        int index = 0;

        for (int i = 0; i < osize; i++, index++) {
            bool answersUpdated = update(oldAnswers[i], newAnswers[i]);
            updated = updated && answersUpdated;
        }

        for (int i = index; i < nsize; i++) {
            bool newAnswerAdded = add(newAnswers[i]);
            updated = updated && newAnswerAdded;
        }
    } else if (nsize < osize) {
        this->unlockTables();

        throw "Answer count sizes not equal, thrown from private ExamQuestion update";
    }

    return updated;
}

bool DatabaseManager::update(const Exam &oldExam, const Exam &updatedExam)
{
    int id = oldExam.getID();
    int id1 = updatedExam.getID();

    if (id != id1)
    {
        throw KeyMismatch(std::to_string(id), std::to_string(id1));
    }
    else
    {   
        this->lockTable(EXAMS);
        this->lockTable(EXAM_QUESTIONS);
        this->lockTable(EXAM_ANSWERS);

        string query = "UPDATE exams SET module = '" + updatedExam.getModule().getCode() + "', name = '" + updatedExam.getName() + "', year = " + std::to_string(updatedExam.getYear()) + ", semester = " + std::to_string(updatedExam.getSemester()) + ", numberOfQuestions = " + std::to_string(updatedExam.getNumberOfQuestions()) + ", weightPerQuestion = " + std::to_string(updatedExam.getWeightPerQuestion()) + ", totalWeight = " + std::to_string(updatedExam.getTotalWeight()) + " WHERE id = " + std::to_string(id) + ";";

        bool updated = executeUpdate(query) != 0;

        for (const ExamQuestion &question : updatedExam.getQuestions()) {
            query = "UPDATE questions SET question = '" + question.getQuestion() + "', numberOfAnswers = " + std::to_string(question.getNumberOfAnswers()) + " WHERE exam = " + std::to_string(id) + " AND question = '" + question.getQuestion() + "';";

            updated = updated && executeUpdate(query) != 0; 

            vector<ExamQuestion> oldQuestions = oldExam.getQuestions();
            vector<ExamQuestion> newQuestions = updatedExam.getQuestions();

            int osize = oldQuestions.size(), nsize = newQuestions.size();

            if (osize <= nsize) {
                int index = 0;
                for (int i = 0; i < osize; i++, index++) {
                    update(oldQuestions[i], newQuestions[i]);
                }

                for (int i = index; i < nsize; i++) {
                    add(newQuestions[i]);
                }
            } else {
                this->unlockTables();

                throw "Size differences between number of exam questions between old and updated exam";
            }

        }

        this->unlockTables();

        return updated;
    }
}

int DatabaseManager::getExamID(Exam &exam)
{
    string module = "'" + exam.getModule().getCode() + "'";
    string name = "'" + exam.getName() + "'";
    string numQuestions = std::to_string(exam.getNumberOfQuestions());
    string weight = std::to_string(exam.getWeightPerQuestion());
    string totalW = std::to_string(exam.getTotalWeight()); //most likely to be only one result if all attributes match

    string query = "SELECT id FROM exams WHERE module = " + module + " AND " + "name = " + name + " AND " + "numberOfQuestions = " + numQuestions + " AND " + "weightPerQuestion = " + weight + " AND " + "totalWeight = " + totalW + ";";

    int id = -1;

    ResultSet *res = executeQuery(query);

    if (res->next())
    {
        id = res->getInt("id");
        exam.setID(id);
    }

    return id;
}

bool DatabaseManager::add(const ExamGrade &examGrade)
{
    this->lockTable(EXAM_GRADES);

    Exam exam = examGrade.getExam();
    int id = exam.getID();
    string query = "INSERT INTO exam_grades (student, exam, grade) VALUES (" + std::to_string(examGrade.getStudent().getID()) + ", " + std::to_string(id) + ", " + std::to_string(examGrade.getGrade()) + ");";

    try
    {
        this->stmt->execute(query);
        
        this->unlockTables();

        return true;
    }
    catch (SQLException &e)
    {
        Warning w(e.what(), query);
        this->warnings.push_back(w);

        this->unlockTables();

        return false;
    }
}

boost::optional<ExamGrade> DatabaseManager::getExamGrade(const Student &student, const Exam &exam)
{
    ResultSet *res = executeQuery("SELECT * FROM exam_grades WHERE student = " + std::to_string(student.getID()) + " AND exam = " + std::to_string(exam.getID()) + ";");

    if (res->next())
    {
        float grade = res->getDouble("grade");

        delete res;

        return ExamGrade(student, exam, grade);
    }

    delete res;

    return boost::none;
}

/*bool DatabaseManager::contains(const ExamGrade &examGrade) {
    return getExamGrade(examGrade.getStudent(), examGrade.getExam()).has_value();
}*/

vector<ExamGrade> DatabaseManager::getAllExamGrades()
{
    vector<ExamGrade> examGrades;

    ResultSet *res = executeQuery("SELECT student, exam FROM exam_grades;");

    while (res->next())
    {
        Student student = getStudent(res->getInt("student")).get();
        Exam exam = getExam(res->getInt("exam")).get();
        examGrades.push_back(getExamGrade(student, exam).get());
    }

    delete res;

    return examGrades;
}

bool DatabaseManager::remove(const ExamGrade &examGrade)
{
    this->lockTable(EXAM_GRADES);

    string query = "DELETE FROM exam_grades WHERE student = " + std::to_string(examGrade.getStudent().getID()) + " AND exam = " + std::to_string(examGrade.getExam().getID()) + ";";
    bool removed = this->stmt->executeUpdate(query) != 0;

    this->unlockTables();

    return removed;
}

bool DatabaseManager::update(const Student &student, const Exam &exam, const ExamGrade &updatedExamGrade)
{
    int sId = student.getID();
    int sId1 = updatedExamGrade.getStudent().getID();
    int eId = exam.getID();
    int eId1 = updatedExamGrade.getStudent().getID();

    if (sId != sId1 && eId != eId1)
    {
        string key = std::to_string(sId) + "-" + std::to_string(eId);
        string key1 = std::to_string(sId1) + "-" + std::to_string(eId1);
        throw KeyMismatch(key, key1);
    }
    else
    {
        this->lockTable(EXAM_GRADES);

        string query = "UPDATE exam_grades SET grade = " + std::to_string(updatedExamGrade.getGrade()) + " WHERE student = " + std::to_string(sId1) + " AND exam = " + std::to_string(eId1) + ";";

        bool updated = executeUpdate(query) != 0;

        this->unlockTables();

        return updated;
    }
}

void DatabaseManager::calculateModuleGrades(std::string module, const Student &student)
{
    this->lockTable(MODULE_GRADES);

    string query = "CALL calculate_grades('" + module + "', " + std::to_string(student.getID()) + ");";

    try
    {
        this->stmt->execute(query);
        this->unlockTables();
    }
    catch (SQLException &e)
    {
        Warning w(e.what(), query);
        warnings.push_back(w);
        this->unlockTables();
    }
}

boost::optional<ModuleGrade> DatabaseManager::getModuleGrade(const Module &module, const Student &student)
{
    string moduleCode = "'" + module.getCode() + "'";
    int id = student.getID();

    ResultSet *res = this->stmt->executeQuery("SELECT * FROM module_grades WHERE module = " + moduleCode + " AND student = " + std::to_string(id) + ";");

    if (res->next())
    {
        boost::optional<Module> module = getModule(res->getString("module"));
        boost::optional<Student> student = getStudent(res->getInt("student"));
        float mark = res->getDouble("mark");

        delete res;

        return ModuleGrade(module.get(), student.get(), mark);
    }

    delete res;

    return boost::none;
}

vector<ModuleGrade> DatabaseManager::getAllModuleGrades()
{
    vector<ModuleGrade> moduleGrades;

    ResultSet *res = executeQuery("SELECT module, student FROM module_grades;");

    while (res->next())
    {
        Module module = getModule(res->getString("module")).get();
        Student student = getStudent(res->getInt("student")).get();
        moduleGrades.push_back(getModuleGrade(module, student).get());
    }

    delete res;

    return moduleGrades;
}

bool DatabaseManager::add(const LecturerAccount &lecturerAccount)
{   
    this->lockTable(LECTURER_ACCOUNTS);
    string query = "INSERT INTO lecturer_accounts (id, pass) VALUES (" + std::to_string(lecturerAccount.getLecturer().getID()) + ", '" + lecturerAccount.getPassword() + "');";

    try
    {
        this->stmt->execute(query);

        this->unlockTables();

        return true;
    }
    catch (SQLException &e)
    {
        Warning w(e.what(), query);
        this->warnings.push_back(w);

        this->unlockTables();

        return false;
    }
}

boost::optional<LecturerAccount> DatabaseManager::getLecturerAccount(int id)
{
    string query = "SELECT * FROM lecturer_accounts WHERE id = " + std::to_string(id) + ";";

    ResultSet *res = executeQuery(query);

    if (res->next())
    {
        boost::optional<Lecturer> lecturer = getLecturer(id);
        string pass = res->getString("pass");

        delete res;

        return LecturerAccount(lecturer.get(), pass);
    }

    delete res;

    return boost::none;
}

vector<LecturerAccount> DatabaseManager::getAllLecturerAccounts()
{
    vector<LecturerAccount> accounts;

    ResultSet *res = executeQuery("SELECT id FROM lecturer_accounts;");

    while (res->next())
    {
        accounts.push_back(getLecturerAccount(res->getInt("id")).get());
    }

    delete res;

    return accounts;
}

bool DatabaseManager::remove(const LecturerAccount &lecturerAccount)
{   
    this->lockTable(LECTURER_ACCOUNTS);
    
    bool removed = executeUpdate("DELETE FROM lecturer_accounts WHERE id = " + std::to_string(lecturerAccount.getLecturer().getID()) + ";") != 0;

    this->unlockTables();

    return removed;
}

bool DatabaseManager::update(const Lecturer &lecturer, const LecturerAccount &updatedLecturerAccount)
{
    int id = lecturer.getID();
    int id1 = updatedLecturerAccount.getLecturer().getID();

    if (id != id1)
    {
        throw KeyMismatch(std::to_string(id), std::to_string(id1));
    }
    else
    {   
        this->lockTable(LECTURER_ACCOUNTS);
        string query = "UPDATE lecturer_accounts SET pass = '" + updatedLecturerAccount.getPassword() + "' WHERE id = " + std::to_string(id1) + ";";

        bool updated = executeUpdate(query) != 0;

        this->unlockTables();

        return updated;
    }
}

bool DatabaseManager::add(const StudentAccount &studentAccount)
{
    this->lockTable(STUDENT_ACCOUNTS);

    string query = "INSERT INTO student_accounts (id, pass) VALUES (" + std::to_string(studentAccount.getStudent().getID()) + ", '" + studentAccount.getPassword() + "');";

    try
    {
        this->stmt->execute(query);
       
        this->unlockTables();
       
        return true;
    }
    catch (SQLException &e)
    {
        Warning w(e.what(), query);
        this->warnings.push_back(w);

        this->unlockTables();

        return false;
    }
}

boost::optional<StudentAccount> DatabaseManager::getStudentAccount(int id)
{
    string query = "SELECT * FROM student_accounts WHERE id = " + std::to_string(id) + ";";

    ResultSet *res = executeQuery(query);

    if (res->next())
    {
        boost::optional<Student> student = getStudent(id);
        string pass = res->getString("pass");

        delete res;

        return StudentAccount(student.get(), pass);
    }

    delete res;

    return boost::none;
}

vector<StudentAccount> DatabaseManager::getAllStudentAccounts()
{
    vector<StudentAccount> accounts;

    ResultSet *res = executeQuery("SELECT id FROM student_accounts;");

    while (res->next())
    {
        accounts.push_back(getStudentAccount(res->getInt("id")).get());
    }

    delete res;

    return accounts;
}

bool DatabaseManager::remove(const StudentAccount &studentAccount)
{   
    this->lockTable(STUDENT_ACCOUNTS);

    bool removed = executeUpdate("DELETE FROM student_accounts WHERE id = " + std::to_string(studentAccount.getStudent().getID()) + ";") != 0;

    this->unlockTables();

    return removed;
}

bool DatabaseManager::update(const Student &student, const StudentAccount &updatedStudentAccount)
{
    int id = student.getID();
    int id1 = updatedStudentAccount.getStudent().getID();

    if (id != id1)
    {
        throw KeyMismatch(std::to_string(id), std::to_string(id1));
    }
    else
    {
        this->lockTable(STUDENT_ACCOUNTS);

        string query = "UPDATE student_accounts SET pass = '" + updatedStudentAccount.getPassword() + "' WHERE id = " + std::to_string(id1) + ";";

        bool updated = executeUpdate(query) != 0;

        this->unlockTables();

        return updated;
    }
}

void DatabaseManager::execute(string query)
{
    try
    {
        this->stmt->execute(query);
    }
    catch (SQLException &e)
    {
        Warning w(e.what(), query);
        this->warnings.push_back(w);
    }
}

ResultSet *DatabaseManager::executeQuery(string query)
{
    ResultSet *res = this->stmt->executeQuery(query);

    return res;
}

int DatabaseManager::executeUpdate(string query)
{
    return this->stmt->executeUpdate(query);
}

void DatabaseManager::clearTable(Tables table)
{
    this->stmt->executeUpdate("TRUNCATE TABLE IF EXISTS " + tableNames.at(table) + ";");
}

void DatabaseManager::clearDatabase()
{
    this->stmt->execute("SET foreign_key_checks = 0;");
    for (int i = 0; i < MAX; i++)
    {
        clearTable(static_cast<Tables>(i));
    }
    this->stmt->execute("SET foreign_key_checks = 1;");

    setLastExamID();
}

vector<Warning> DatabaseManager::getWarnings() const
{
    return this->warnings;
}

void DatabaseManager::clearWarnings()
{
    this->warnings.clear();
}

void DatabaseManager::writeWarningsToLog()
{
    if (warnings.size() == 0)
        return;

    time_t now = time(0);

    tm *ltm = localtime(&now);

    stringstream s;
    s << "../Logs/" << ltm->tm_mday << "_" << ltm->tm_mon + 1 << "_" << 1900 + ltm->tm_year << "_" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "_Database_Warnings.log";
    string filename;
    s >> filename;
    ofstream writer;
    writer.open(filename);

    if (writer.is_open())
    {
        for (const Warning &w : warnings)
        {
            writer << w.toString() << "\n";
        }
        writer.flush();
    }
}

void DatabaseManager::lockTable(Tables table) {
    this->stmt->execute("LOCK TABLE " + DatabaseManager::tableNames.at(table) + " WRITE;");
}

void DatabaseManager::unlockTables() {
    this->stmt->execute("UNLOCK TABLES;");
}

bool DatabaseManager::isTableLocked(Tables table) {
    string tname = "'" + DatabaseManager::tableNames.at(table) + "'";
    string dbname = DB;
    ResultSet *res = this->stmt->executeQuery("SHOW OPEN TABLES IN " + dbname + "WHERE Table = " + tname + " AND In_use > 1;");

    bool locked = res->next();

    delete res;

    return locked;
}