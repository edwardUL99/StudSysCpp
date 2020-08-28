#ifndef WELCOME_PAGE_H
#define WELCOME_PAGE_H

#include "Page.h"
#include <string>

namespace ui
{

    class WelcomePage : public Page
    {
    private:
        static const std::string welcomeText;

    public:
        WelcomePage(StudentSystem &system);
        virtual ~WelcomePage() = default;
        virtual void show() override;
    };

} // namespace ui

#endif