#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "DatabaseItem.h"
#include <string>

//None of this is implemented yet, for now have separate methods for Lecturer and StudentAccounts
class Account {
    protected:
        std::string name;
        std::string email;
        std::string pass;

    public:
        Account(std::string name, std::string email, std::string pass);
        virtual ~Account() = default;
        std::string getName() const; 
        void setName(std::string name);
        std::string getEmail() const;
        virtual std::string getPassword() const;
        virtual void setPassword(std::string pass);
};

#endif