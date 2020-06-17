#include "headers/StudentSystem.h"
#include "headers/DuplicateException.h"
#include "headers/NotFoundException.h"
#include "headers/KeyMismatch.h"
#include "headers/StudentRegistration.h"

using std::string;

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
        return this->database.add(lecturer);
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
    return this->database.remove(lecturer);
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
        return this->database.add(student);
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
    return this->database.remove(student);
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
        return this->database.add(course);
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
    return this->database.remove(course);
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
        return this->database.add(module);
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
    return this->database.remove(module);
}

bool StudentSystem::updateModule(string code, const Module &updatedModule) {
    try {
        return this->database.update(code, updatedModule);
    } catch (KeyMismatch &m) {
        throw m;
    }
}

bool StudentSystem::registerStudentModule(const Student &student, const Module &module) {
    StudentRegistration registration(student, module);

    if (this->database.contains(registration)) {
        throw DuplicateException(registration.getDescription());
    } else {
        return this->database.add(registration);
    }
}

bool StudentSystem::unregisterStudentModule(const Student &student, const Module &module) {
    StudentRegistration registration(student, module);

    return this->database.remove(registration);
}

std::vector<Module> StudentSystem::getStudentRegisteredModules(const Student &student) {
    std::vector<Module> modules;

    for (const StudentRegistration &registration : this->database.getAllStudentRegistrations()) {
        if (registration.getStudent().getID() == student.getID()) {
            modules.push_back(registration.getModule());
        }
    }

    return modules;
}

bool StudentSystem::addExam(const Exam &exam) {
    if (this->database.contains(exam)) {
        throw DuplicateException(exam.getDescription());
    } else {
        return this->database.add(exam);
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
    return this->database.remove(exam);
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
        return this->database.add(examGrade);
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
    return this->database.remove(examGrade);
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
        return this->database.add(lecturerAccount);
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
    return this->database.remove(lecturerAccount);
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
        return this->database.add(studentAccount);
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
    return this->database.remove(studentAccount);
}

bool StudentSystem::updateAccount(const Student &student, const StudentAccount &updatedStudentAccount) {
    try {
        return this->database.update(student, updatedStudentAccount);
    } catch (KeyMismatch &m) {
        throw m;
    }
}