#ifndef EXAM_QUESTION_H
#define EXAM_QUESTION_H

#include <vector>
#include <string>
#include "ExamAnswer.h"
#include "DatabaseItem.h"

/**
 * A class representing an Exam Question and it's answers
 */
class ExamQuestion : public DatabaseItem {
    private:
        std::string question;
        ExamAnswer key; //when doing UI and setting up an exam make sure you call ExamANswer("answer", true) to make it a key
        std::vector<ExamAnswer> possibleAnswers;
        int numberOfAnswers;

    public:
        ExamQuestion(std::string question, ExamAnswer correctAnswer, std::vector<ExamAnswer> possibleAnswers = std::vector<ExamAnswer>(), int numberOfAnswers = 4);
        
        virtual ~ExamQuestion() = default;

        std::string getQuestion() const;

        void setQuestion(std::string question);

        bool addExamAnswer(const ExamAnswer &answer);

        std::vector<ExamAnswer> getPossibleAnswers() const;

        /**
         * Checks if the answer given matches the correctAnswer
         */
        bool checkAnswer(const ExamAnswer &answer) const;

        ExamAnswer getKey() const;

        void setKey(ExamAnswer &key);

        int getNumberOfAnswers() const;

        void setNumberOfAnswers(int numberOfAnswers);

        virtual std::string getObjectType() const override;

        virtual Tables getTable() const override;

        virtual std::string getDescription() const override;

        friend bool operator<(const ExamQuestion &q1, const ExamQuestion &q2);
};

#endif