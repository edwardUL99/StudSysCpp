#ifndef EXAM_ANSWER_H
#define EXAM_ANSWER_H

#include <iostream>
#include <string>
#include "DatabaseItem.h"

/**
 * Represents a possible answer to an exam question
 */
class ExamAnswer : public DatabaseItem {
    private:
        std::string answer;
        bool key;

    public:
        ExamAnswer(std::string answer = "Not answered", bool key = false);
        ExamAnswer(int answer, bool key = false);
        ExamAnswer(float answer, bool key = false);
        
        virtual ~ExamAnswer() = default;

        std::string getAnswer() const;

        void setAnswer(std::string answer);
        void setAnswer(int answer);
        void setAnswer(float answer);

        bool isKey() const;

        virtual std::string getObjectType() const override;
        virtual Tables getTable() const override;
        virtual std::string getDescription() const override;

        bool operator==(const ExamAnswer &answer) const;

        friend std::ostream &operator<<(std::ostream &os, const ExamAnswer &answer);
};

#endif