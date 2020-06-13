#ifndef STUDENT_ACCOUNT_H
#define STUDENT_ACCOUNT_H

#include "DatabaseItem.h"
#include "Account.h"
#include "Student.h"

#include <string>

class StudentAccount: public DatabaseItem, public Account {
    private:
        Student student;

    public:
        StudentAccount(Student student, std::string pass);
        virtual ~StudentAccount() = default;
        Student getStudent() const;
        virtual std::string getPassword() const;
        virtual void setPassword(std::string pass);
        virtual std::string getObjectType() const override;
        virtual std::string getDescription() const override;
        virtual Tables getTable() const override;
};

#endif