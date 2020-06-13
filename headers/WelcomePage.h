#ifndef WELCOME_PAGE_H
#define WELCOME_PAGE_H

#include "Page.h"
#include <string>

class WelcomePage : public Page {
    private:
        static const std::string welcomeText;

    public:
        WelcomePage(StudentSystem &system);
        virtual void show() override;
};

#endif