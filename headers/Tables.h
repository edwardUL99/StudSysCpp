#ifndef TABLES_H
#define TABLES_H

/**
 * This header file is a simple one to include the 
 *  Tables enum type
 */

//consider having enum class Tables and see if it works but you'll have to use Tables::<ENUM_VALUE>
enum Tables {
        LECTURERS,
        STUDENTS,
        COURSES,
        MODULES,
        EXAMS,
        EXAM_ANSWER_KEYS,
        EXAM_QUESTIONS,
        EXAM_ANSWERS,
        EXAM_GRADES,
        MODULE_GRADES,
        LECTURER_ACCOUNTS,
        STUDENT_ACCOUNTS,
        MAX //used to "iterate" through Tables
};


#endif
