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
        /**
         * Checks conditions of certain flags and decides which prompt to display
         * @param answerable the state of the exam, if it is answerable or not
         */
        void printPrompt();
        /**
         * @brief This method provides the functionality to answer the current question
         * @param index the index marking the position along the exam the user is
         * @param questions the vector of questions in the exam
         */
        void answerCurrentQuestion(int index, const std::vector<ExamQuestion> &questions);
        /**
         * @brief this function handles moving the marker onto the next exam question
         * @param index the reference to the marker marking the position in the exam
         * @param questions the vector of questions in the exam
         */
        void nextQuestion(int &index, const std::vector<ExamQuestion> &questions);
        /**
         * @brief this function handles moving the marker onto the previous exam question
         * @param index the reference to the marker marking the position in the exam
         * @param questions the vector of questions in the exam
         */
        void previousQuestion(int &index, const std::vector<ExamQuestion> &questions);
        /**
         * @brief This function handles submitting of an exam
         * @return whether the user actually wants to submit the exam
         */
        bool submitExam();
        /**
         * @brief This function handles the even of a user wanting to cancel the exam they are doing
         * @return whether the user decides to cancel the exam
         */
        bool cancelExam();
    public:
        //allowReverse if students can go back through answered questions
        ExamPage(Student student, Exam exam, StudentSystem &system, bool allowReverse = true);
        virtual ~ExamPage() = default;
        virtual void show() override;
    };

} // namespace ui

#endif