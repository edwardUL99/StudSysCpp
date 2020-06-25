#ifndef EXAM_SELECTOR_PAGE_H
#define EXAM_SELECTOR_PAGE_H

#include "Account.h"
#include "Module.h"
#include "Page.h"
#include <vector>

class Exam;

namespace ui
{

    class ExamSelectorPage : public Page
    {
        private:
            Account &account;
            Module &module;
            std::vector<Exam> exams;
            void initialiseExamList();
        
        public:
            /**
             * Word of warning with this constructor.
             * It is assumed the only person taking an exam is a student so the Account reference is expected to be referring to a StudentAccount object
             * Otherwise show() method will throw an exception of bad_cast if T is chosen
             */
            ExamSelectorPage(Account &account, Module &module, StudentSystem &system);
            virtual ~ExamSelectorPage() = default;
            virtual void show() override;
    };

} // namespace ui

#endif