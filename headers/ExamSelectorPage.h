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
            ExamSelectorPage(Account &account, Module &module, StudentSystem &system);
            virtual ~ExamSelectorPage() = default;
            virtual void show() override;
    };

} // namespace ui

#endif