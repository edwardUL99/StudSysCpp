#ifndef EXAM_CREATE_PAGE
#define EXAM_CREATE_PAGE

#include "Exam.h"
#include "Page.h"
#include <string>
#include <vector>

class ExamCreatePage : public Page {
    private:
        std::string moduleCode;
        std::string name;
        int year;
        int semester;
        int numQuestions;
        float weightPerQ;
        float totalWeight;
        std::vector<ExamQuestion> questions;
        //edit methods here like change/add questions etc

        void edit();
        std::vector<ExamAnswer> createAnswers();
        void createQuestions();

    public:
        ExamCreatePage(StudentSystem &system);
        virtual ~ExamCreatePage() = default;
        Exam getCreatedExam() const;
        virtual void show() override;
};

#endif