#ifndef EXAM_CREATE_PAGE
#define EXAM_CREATE_PAGE

#include "Exam.h"
#include "Page.h"
#include "Module.h"
#include <string>
#include <vector>

namespace ui
{

    class ExamCreatePage : public Page
    {
    private:
        Module module;
        std::string name;
        int year;
        int semester;
        int numQuestions;
        float totalWeight;
        std::vector<ExamQuestion> questions;
        //edit methods here like change/add questions etc

        /**
          * @brief this subroutine is used by the edit method to edit the year for the exam
          */
        void editYear();
        /**
         * @brief this subroutine is used by the edit method to handle the entering of the number of questions in the exam
         * @param numberOfQuestionsChosen a reference to a flag used to check if the number of questions have been chosen
         */
        void enterNumberOfQuestions(bool &numberOfQuestionsChosen);
        /**
         * @brief this subroutine is used by the edit method to handle entering of the semester
         */
        void enterSemester();
        /**
         * @brief this subroutine is used by the edit method to handle entering the total weight for this exam
         */
        void enterTotalWeight();
        void edit();
        ExamAnswer createAnswers(int examID, int question, int numberOfAnswers, std::vector<ExamAnswer> &answerVector); //returns the key
        void createQuestions();
        void submit();
        bool checkPopulated() const;

    public:
        ExamCreatePage(Module module, StudentSystem &system);
        virtual ~ExamCreatePage() = default;
        virtual void show() override;
    };

} // namespace ui

#endif
