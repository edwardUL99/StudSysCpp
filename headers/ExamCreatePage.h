#ifndef EXAM_CREATE_PAGE
#define EXAM_CREATE_PAGE

#include "Exam.h"
#include "Page.h"
#include "Module.h"
#include <string>
#include <vector>

class ExamCreatePage : public Page {
    private:
        Module module;
        std::string name;
        int year;
        int semester;
        int numQuestions;
        float totalWeight;
        std::vector<ExamQuestion> questions;
        //edit methods here like change/add questions etc

        void edit();
        ExamAnswer createAnswers(int examID, std::string question, int numberOfAnswers, std::vector<ExamAnswer> &answerVector); //returns the key
        void createQuestions();
        void submit();

    public:
        ExamCreatePage(Module module, StudentSystem &system);
        virtual ~ExamCreatePage() = default;
        bool checkPopulated() const;
        virtual void show() override;
};

#endif