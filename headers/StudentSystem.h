#ifndef STUDENT_SYSTEM_H
#define STUDENT_SYSTEM_H

#include "Lecturer.h"
#include "Student.h"
#include "Course.h"
#include "Module.h"
#include "ModuleGrade.h"
#include "Exam.h"
#include "ExamGrade.h"
#include "LecturerAccount.h"
#include "StudentAccount.h"
#include "DatabaseManager.h"

#include <vector>

//todo see db manager h file top about remove methods and foreign key checks
//will need a table to save what modules students are taking etc

class StudentSystem {
    private:
        std::vector<Lecturer> lecturers;
        std::vector<Student> students;
        std::vector<Course> courses;
        std::vector<Module> modules;
        std::vector<Exam> exams; //this may need to change
        std::vector<ExamGrade> examGrades;
        std::vector<ModuleGrade> moduleGrades;
        std::vector<LecturerAccount> lecturerAccounts;
        std::vector<StudentAccount> studentAccounts;
        DatabaseManager database;
        void restoreSystem(); //restores system from database

    public:
        StudentSystem();
        bool addLecturer(const Lecturer &lecturer);
        Lecturer getLecturer(int id);
        bool removeLecturer(const Lecturer &lecturer);
        bool updateLecturer(int id, const Lecturer &updatedLecturer);
        int getLecturerID(std::string email);
        bool addStudent(const Student &student);
        Student getStudent(int id);
        bool removeStudent(const Student &student);
        bool updateStudent(int id, const Student &updatedStudent);
        int getStudentID(std::string email);
        bool addCourse(const Course &course);
        Course getCourse(std::string id);
        bool removeCourse(const Course &course);
        bool updateCourse(std::string id, const Course &updatedCourse);
        bool addModule(const Module &module);
        Module getModule(std::string code);
        bool removeModule(const Module &module);
        bool updateModule(std::string code, const Module &updaredModule);
        bool addExam(const Exam &exam); //exam may need to change
        Exam getExam(int id);
        bool removeExam(const Exam &exam);
        bool updateExam(int id, const Exam &updatedExam);
        bool addExamGrade(const ExamGrade &examGrade);
        ExamGrade getExamGrade(const Student &student, const Exam &exam);
        bool removeExamGrade(const ExamGrade &examGrade);
        bool updateExamGrade(const Student &student, const Exam &exam, const ExamGrade &updatedExamGrade);
        ModuleGrade getModuleGrade(const Module &module, const Student &student);
        bool addAccount(LecturerAccount lecturerAccount);
        LecturerAccount getLecturerAccount(int id);
        bool removeAccount(const LecturerAccount &lecturerAccount);
        bool updateAccount(const Lecturer &lecturer, const LecturerAccount &updatedLecturerAccount);
        bool addAccount(const StudentAccount &studentAccount);
        StudentAccount getStudentAccount(int id);
        bool removeAccount(const StudentAccount &studentAccount);
        bool updateAccount(const Student &student, const StudentAccount &studentAccount);
        //have get methods too for above and ModuleGrades 
        //have update methods too for the accounts and others;
};

#endif