#ifndef EXAM_EDIT_PAGE_H
#define EXAM_EDIT_PAGE_H

#include "Page.h"
#include "studsys/Exam.h"
#include <string>
#include <vector>

namespace ui
{
    class ExamEditPage : public Page {
        private:
            const Exam exam;
            Exam newExam; //this is this exam you can edit
            /**
             * @brief Asks the user to choose an exam question from a list of them and returns it
             * @param questions the list of questions
             * @return the chosen question
             */
            ExamQuestion& chooseExamQuestion(std::vector<ExamQuestion> &questions);
            /**
             * @brief Edits the answer key for the specified question
             * @param question the question which is being edited
             * @param answers the vector of answers belonging to this question, i.e. the answers being currently edited by editAnswers
             */
            void editAnswerKey(ExamQuestion &question, std::vector<ExamAnswer> &answers);
            /**
             * @brief this method is used by editAnswers(ExamQuestion) to go through each answer of that question and give the option to change it
             * @param question the question to which answers belongs
             * @param answers the list of answers to edit
             */
            void editAnswers(ExamQuestion &question, std::vector<ExamAnswer> &answers);
            /**
             * @brief This method provides the functionality to edit answers of the specified question
             * @param question A reference to the question being edited
             */
            void editAnswers(ExamQuestion &question);
            /**
             * @brief submit the changes and returns whether editing should finish
             * @param questions the list of questions being edited
             * @return true if you are to finish editing
             */
            bool submitUpdates(std::vector<ExamQuestion> &questions);
            void updateQuestions(); //don't allow to change number of questions 
            /**
             * @brief this method provides functionality for editing the module
             */
            void editModule();
            /**
             * @brief this method provides functionality to change exam name
             */
            void editName();
            /**
             * @brief this method provides functionality to change exam year
             */
            void editYear();
            /**
             * @brief this method provides functionality to change semester
             */
            void editSemester();
            /**
             * @brief this method provides functionality to change total weight
             */
            void editTotalWeight();
            /**
             * @brief This method submits the changes to the database
             */
            void submitChanges();
        public:
            ExamEditPage(const Exam &exam, StudentSystem &system);
            virtual ~ExamEditPage() = default;
            virtual void show() override;
    };
}

#endif