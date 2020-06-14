#include "headers/StudentSystem.h"
#include "headers/DuplicateException.h"
#include "headers/NotFoundException.h"
#include "headers/KeyMismatch.h"

using std::string;

StudentSystem::StudentSystem() {
    restoreSystem();
}

//mightn't need this
void StudentSystem::restoreSystem() {
    this->lecturers = this->database.getAllLecturers();
    this->students = this->database.getAllStudents();
    this->courses = this->database.getAllCourses();
    this->modules = this->database.getAllModules();
    this->moduleGrades = this->database.getAllModuleGrades();
    this->exams = this->database.getAllExams();
    this->examGrades = this->database.getAllExamGrades();
    this->lecturerAccounts = this->database.getAllLecturerAccounts();
    this->studentAccounts = this->database.getAllStudentAccounts();
}

string StudentSystem::recordLogin(const Account &account) {
    string date = "FIRST_LOGIN";

    ResultSet *res = NULL;

    try {
        const StudentAccount &student = dynamic_cast<const StudentAccount&>(account);
        string id = std::to_string(student.getStudent().getID());

        res = this->database.executeQuery("SELECT loginTime FROM student_logins WHERE loginTime >= ALL(SELECT loginTime FROM student_logins) AND id = " + id + ";");

        if (res->next()) {
            date = res->getString("loginTime");
        }

        this->database.execute("INSERT INTO student_logins (id) VALUES (" + id + ");");
    } catch (std::bad_cast &b) {}

    try {
        const LecturerAccount &lecturer = dynamic_cast<const LecturerAccount&>(account);
        string id = std::to_string(lecturer.getLecturer().getID());

        res = this->database.executeQuery("SELECT loginTime FROM lecturer_logins WHERE loginTime >= ALL(SELECT loginTime FROM lecturer_logins) AND id = " + id + ";");

        if (res->next()) {
            date = res->getString("loginTime");
        }

        this->database.execute("INSERT INTO lecturer_logins (id) VALUES (" + id + ");");
    } catch (std::bad_cast &b) {}

    delete res;

    return date;
}

bool StudentSystem::addLecturer(const Lecturer &lecturer) {
    if (this->database.contains(lecturer)) {
        throw DuplicateException(lecturer.getDescription());
    } else {
        if (this->database.add(lecturer)) {
            this->lecturers.push_back(lecturer);
            return true;
        }

        return false;
    }
}

Lecturer StudentSystem::getLecturer(int id) {
    boost::optional<Lecturer> lecturer = this->database.getLecturer(id);

    if (lecturer.has_value()) {
        return lecturer.get();
    } else {
        throw NotFoundException("Lecturer identified by " + std::to_string(id));
    }
}

bool StudentSystem::removeLecturer(const Lecturer &lecturer) {
    if (this->database.remove(lecturer)) {
        for (auto it = this->lecturers.begin(); it != this->lecturers.end(); it++) {
            const Lecturer l = *it;
            if (l.getID() == lecturer.getID() && l.getEmail() == lecturer.getEmail()) {
                this->lecturers.erase(it);
                break;
            }
        }

        return true;
    }

    return false;
}

bool StudentSystem::updateLecturer(int id, const Lecturer &updatedLecturer) {
    try {
        return this->database.update(id, updatedLecturer);
    } catch (KeyMismatch &m) {
        throw m;
    }
}

int StudentSystem::getLecturerID(string email) {
    for (const Lecturer &l : this->database.getAllLecturers()) {
        if (l.getEmail() == email) {
            return l.getID();
        }
    }

    return -1;
}

bool StudentSystem::addStudent(const Student &student) {
    if (this->database.contains(student)) {
        throw DuplicateException(student.getDescription());
    } else {

        if (this->database.add(student)) {
            //check if added to database, then add to system
            this->students.push_back(student);
            return true;
        }

        return false;
    }
}

Student StudentSystem::getStudent(int id) {
    boost::optional<Student> student = this->database.getStudent(id);

    if (student.has_value()) {
        return student.get();
    } else {
        throw NotFoundException("Student identified by " + std::to_string(id));
    }
}

bool StudentSystem::removeStudent(const Student &student) {
    if (this->database.remove(student)) {
        for (auto it = this->students.begin(); it != this->students.end(); it++) {
            const Student s = *it;
            if (s.getID() == student.getID() && s.getEmail() == student.getEmail()) {
                this->students.erase(it);
                break;
            }
        }

        return true;
    }

    return false;
}

bool StudentSystem::updateStudent(int id, const Student &updatedStudent) {
    try {
        return this->database.update(id, updatedStudent);
    } catch (KeyMismatch &m) {
        throw m;
    }
}

int StudentSystem::getStudentID(string email) {
    for (const Student &s : this->database.getAllStudents()) {
        if (s.getEmail() == email) {
            return s.getID();
        }
    }

    return -1;
}

bool StudentSystem::addCourse(const Course &course) {
    if (this->database.contains(course)) {
        throw DuplicateException(course.getDescription());
    } else {
        if (this->database.add(course)) {
            //check if added to database, then add to system
            this->courses.push_back(course);
            return true;
        }

        return false;
    }
}

Course StudentSystem::getCourse(string id) {
    boost::optional<Course> course = this->database.getCourse(id);

    if (course.has_value()) {
        return course.get();
    } else {
        throw NotFoundException("Course identified by " + id);
    }
}

bool StudentSystem::removeCourse(const Course &course) {
    if (this->database.remove(course)) {
        for (auto it = this->courses.begin(); it != this->courses.end(); it++) {
            const Course c = *it;
            if (c.getID() == course.getID()) {
                this->courses.erase(it);
                break;
            }
        }

        return true;
    }

    return false;
}

bool StudentSystem::updateCourse(string id, const Course &updatedCourse) {
    try {
        return this->database.update(id, updatedCourse);
    } catch (KeyMismatch &m) {
        throw m;
    }
}

bool StudentSystem::addModule(const Module &module) {
    if (this->database.contains(module)) {
        throw DuplicateException(module.getDescription());
    } else {
        if (this->database.add(module)) {
            //check if added to database, then add to system
            this->modules.push_back(module);
            return true;
        }

        return false;
    }
}

Module StudentSystem::getModule(string code) {
    boost::optional<Module> module = this->database.getModule(code);

    if (module.has_value()) {
        return module.get();
    } else {
        throw NotFoundException("Module identified by " + code);
    }
}

bool StudentSystem::removeModule(const Module &module) {
    if (this->database.remove(module)) {
        for (auto it = this->modules.begin(); it != this->modules.end(); it++) {
            const Module m = *it;
            if (m.getCode() == module.getCode()) {
                this->modules.erase(it);
                break;
            }
        }

        return true;
    }

    return false;
}

bool StudentSystem::updateModule(string code, const Module &updatedModule) {
    try {
        return this->database.update(code, updatedModule);
    } catch (KeyMismatch &m) {
        throw m;
    }
}

bool StudentSystem::addExam(const Exam &exam) {
    if (this->database.contains(exam)) {
        throw DuplicateException(exam.getDescription());
    } else {
        if (this->database.add(exam)) {
            //check if added to database, then add to system
            this->exams.push_back(exam);
            return true;
        }

        return false;
    }
}

Exam StudentSystem::getExam(int id) {
    boost::optional<Exam> exam = this->database.getExam(id);

    if (exam.has_value()) {
        return exam.get();
    } else {
        throw NotFoundException("Exam identified by " + std::to_string(id));
    }
}

bool StudentSystem::removeExam(const Exam &exam) {
    if (this->database.remove(exam)) {
        for (auto it = this->exams.begin(); it != this->exams.end(); it++) {
            const Exam e = *it;
            if (e.getID() == exam.getID()) {
                this->exams.erase(it);
                break;
            }
        }

        return true;
    }

    return false;
}

bool StudentSystem::updateExam(const Exam &oldExam, const Exam &updatedExam) {
    try {
        return this->database.update(oldExam, updatedExam);
    } catch (KeyMismatch &m) {
        throw m;
    }
}

bool StudentSystem::addExamGrade(const ExamGrade &examGrade) {
    if (this->database.contains(examGrade)) {
        throw DuplicateException(examGrade.getDescription());
    } else {
        if (this->database.add(examGrade)) {
            //check if added to database, then add to system
            this->examGrades.push_back(examGrade);
            
            return true;
        }

        return false;
    }
}

ExamGrade StudentSystem::getExamGrade(const Student &student, const Exam &exam) {
    boost::optional<ExamGrade> examGrade = this->database.getExamGrade(student, exam);

    if (examGrade.has_value()) {
        return examGrade.get();
    } else {
        throw NotFoundException("Exam Grade identified by Student " + std::to_string(student.getID()) + " and Exam " + std::to_string(exam.getID()));
    }
}

bool StudentSystem::removeExamGrade(const ExamGrade &examGrade) {
    if (this->database.remove(examGrade)) {
        for (auto it = this->examGrades.begin(); it != this->examGrades.end(); it++) {
            const ExamGrade eg = *it;
            if (eg.getExam().getID() == examGrade.getExam().getID() && eg.getStudent().getID() == examGrade.getStudent().getID()) {
                this->examGrades.erase(it);
                break;
            }
        }

        return true;
    }

    return false;
}

bool StudentSystem::updateExamGrade(const Student &student, const Exam &exam, const ExamGrade &updatedExamGrade) {
    try {
        return this->database.update(student, exam, updatedExamGrade);
    } catch (KeyMismatch &m) {
        throw m;
    }
}

ModuleGrade StudentSystem::getModuleGrade(const Module &module, const Student &student) {
    boost::optional<ModuleGrade> moduleGrade = this->database.getModuleGrade(module, student);

    if (moduleGrade.has_value()) {
        return moduleGrade.get();
    } else {
        throw NotFoundException("Module Grade identified by module " + module.getCode() + " and student " + std::to_string(student.getID()));
    }
}

bool StudentSystem::addAccount(LecturerAccount lecturerAccount) {
    if (this->database.contains(lecturerAccount)) {
        throw DuplicateException(lecturerAccount.getDescription());
    } else {
        if (this->database.add(lecturerAccount)) {
            //check if added to database, then add to system
            this->lecturerAccounts.push_back(lecturerAccount);

            return true;
        }

        return false;
    }
}

LecturerAccount StudentSystem::getLecturerAccount(int id) {
    boost::optional<LecturerAccount> lecturerAccount = this->database.getLecturerAccount(id);

    if (lecturerAccount.has_value()) {
        return lecturerAccount.get();
    } else {
        throw NotFoundException("Lecturer Account identified by lecturer " + id);
    }
}

bool StudentSystem::removeAccount(const LecturerAccount &lecturerAccount) {
    if (this->database.remove(lecturerAccount)) {
        for (auto it = this->lecturerAccounts.begin(); it != this->lecturerAccounts.end(); it++) {
            const LecturerAccount account = *it;
            if (account.getLecturer().getID() == lecturerAccount.getLecturer().getID()) {
                this->lecturerAccounts.erase(it);
                break;
            }
        }

        return true;
    }

    return false;
}

bool StudentSystem::updateAccount(const Lecturer &lecturer, const LecturerAccount &updatedLecturerAccount) {
    try {
        return this->database.update(lecturer, updatedLecturerAccount);
    } catch (KeyMismatch &m) {
        throw m;
    }
}

bool StudentSystem::addAccount(const StudentAccount &studentAccount) {
    if (this->database.contains(studentAccount)) {
        throw DuplicateException(studentAccount.getDescription());
    } else {
        if (this->database.add(studentAccount)) {
            //check if added to database, then add to system
            this->studentAccounts.push_back(studentAccount);

            return true;
        }

        return false;
    }
}

StudentAccount StudentSystem::getStudentAccount(int id) {
    boost::optional<StudentAccount> studentAccount = this->database.getStudentAccount(id);

    if (studentAccount.has_value()) {
        return studentAccount.get();
    } else {
        throw NotFoundException("Student Account identified by student " + std::to_string(id));
    }
}

bool StudentSystem::removeAccount(const StudentAccount &studentAccount) {
    if (this->database.remove(studentAccount)) {
        for (auto it = this->studentAccounts.begin(); it != this->studentAccounts.end(); it++) {
            const StudentAccount account = *it;
            if (account.getStudent().getID() == studentAccount.getStudent().getID()) {
                this->studentAccounts.erase(it);
                break;
            }
        }

        return true;
    }

    return false;
}

bool StudentSystem::updateAccount(const Student &student, const StudentAccount &updatedStudentAccount) {
    try {
        return this->database.update(student, updatedStudentAccount);
    } catch (KeyMismatch &m) {
        throw m;
    }
}