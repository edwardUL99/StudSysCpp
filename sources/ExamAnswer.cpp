#include "headers/ExamAnswer.h"

using std::string;

ExamAnswer::ExamAnswer(string answer, bool key) : answer(answer), key(key) {}

ExamAnswer::ExamAnswer(int answer, bool key) : answer(std::to_string(answer), key) {}

ExamAnswer::ExamAnswer(float answer, bool key) : answer(std::to_string(answer), key) {}

string ExamAnswer::getAnswer() const {
    return answer;
}

void ExamAnswer::setAnswer(string answer) {
    this->answer = answer;
}

void ExamAnswer::setAnswer(int answer) {
    this->setAnswer(std::to_string(answer));
}

void ExamAnswer::setAnswer(float answer) {
    this->setAnswer(std::to_string(answer));
}

bool ExamAnswer::isKey() const {
    return key;
}

std::string ExamAnswer::getObjectType() const {
    return "ExamAnswer";
}

Tables ExamAnswer::getTable() const {
    if (key) {
        return EXAM_ANSWER_KEYS;
    } else {
        return EXAM_ANSWERS;
    }
}

std::string ExamAnswer::getDescription() const {
    if (key) {
        return "Key: " + answer;
    } else {
        return "Answer: " + answer;
    }
}

bool ExamAnswer::operator==(const ExamAnswer &answer) const {
    return this->answer == answer.answer;
}

std::ostream &operator<<(std::ostream &os, const ExamAnswer &answer) {
    os << answer.answer;
    return os;
}
