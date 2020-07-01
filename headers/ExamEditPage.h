#ifndef EXAM_EDIT_PAGE_H
#define EXAM_EDIT_PAGE_H

#include "Page.h"
#include "Exam.h"
#include <string>

namespace ui
{
    class ExamEditPage : public Page {
        private:
            const Exam &exam;
            Exam newExam; //this is this exam you can edit
            void updateQuestions(); //don't allow to change number of questions 
        
        public:
            ExamEditPage(const Exam &exam, StudentSystem &system);
            virtual ~ExamEditPage() = default;
            virtual void show() override;
    };
}

#endif