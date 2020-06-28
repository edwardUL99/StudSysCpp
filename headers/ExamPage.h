#ifndef EXAM_PAGE_H
#define EXAM_PAGE_H

#include "Page.h"
#include "Exam.h"
#include "Student.h"
#include "ExamSubmission.h"
#include <vector>
#include <map>

//will have to make sure to pass in the student from account in AccountPage

namespace ui
{

    class ExamPage : public Page
    {
    private:
        Student student; //the student taking the exam
        Exam exam;
        ExamSubmission answers;
        const bool allowReverse;
        void displayQuestion(const ExamQuestion &examQuestion);

    public:
        //allowReverse if students can go back through answered questions
        ExamPage(Student student, Exam exam, StudentSystem &system, bool allowReverse = true);
        virtual ~ExamPage() = default;
        virtual void show() override;
    };

} // namespace ui

#endif