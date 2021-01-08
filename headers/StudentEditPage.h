#ifndef STUDENT_EDIT_PAGE_H
#define STUDENT_EDIT_PAGE_H

#include "Page.h"

class Student;

namespace ui {
    class StudentEditPage : public Page {
        private:
            const Student oldStudent;
            Student newStudent;
            void updateId();
            void updateName();
            void updateAge();
            void updateCourse();
            bool submitChanges();

        public:
            StudentEditPage(const Student &student, StudentSystem &system);
            virtual ~StudentEditPage() = default;
            virtual void show() override;
    };
}

#endif