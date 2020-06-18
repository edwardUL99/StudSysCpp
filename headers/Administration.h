#ifndef ADMINISTRATION_H
#define ADMINISTRATION_H

#include "Page.h"

class Administration : public Page {
    private:
        bool login();
        void createCourse();
        void removeCourse();
        void createModule();
        void removeModule();
        void createStudent();
        void removeStudent();
        void registerStudent();
        void resetStudentPassword();
        void createLecturer();
        void removeLecturer();
        void resetLecturerPassword();
    
    public:
        Administration(StudentSystem &system);
        virtual ~Administration() = default;
        virtual void show() override;
};

#endif