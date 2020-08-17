#ifndef EXAM_SELECTOR_PAGE_H
#define EXAM_SELECTOR_PAGE_H

#include "studsys/Account.h"
#include "studsys/Module.h"
#include "Page.h"
#include <vector>

class Exam;

namespace ui
{

    class ExamSelectorPage : public Page
    {
        private:
            Account *account;
            Module *module;
            std::vector<Exam> exams;
            void initialiseExamList();
            /**
             * @brief displays the exams on this selector page
             */
            void displayExams();
            /**
             * @brief allows the user to select an exam from the list displayed by displayExams and returns it
             * @return the chosen exam
             */
            Exam getExam();
            /**
             * @brief Checks if the user using the selector page is a lecturer
             * @return true if the account is that of a lecturer
             */
            bool isLecturer();
            /**
             * @brief Checks the status of several boolean flags and prints the prompt accordingly
             * @param lecturer true if a lecturer is viewing this page
             */
            void printPrompt(bool lecturer);
            /**
             * @brief This provides functionality for taking an exam
             * @return true if you want to exit the loop, false otherwise
             */
            bool takeExam();
            /**
             * @brief Provides functionality for starting an edit on an exam
             * @return true if you want to exit the loop, false otherwise
             */
            bool editExam();
        public:
            ExamSelectorPage(Account *account, Module *module, StudentSystem &system);
            virtual ~ExamSelectorPage() = default;
            virtual void show() override;
    };

} // namespace ui

#endif