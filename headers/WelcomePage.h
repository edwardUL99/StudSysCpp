#ifndef WELCOME_PAGE_H
#define WELCOME_PAGE_H

#include "Page.h"
#include "studsys/ConfigFileProcessor.h"
#include <string>

namespace ui
{

    class WelcomePage : public Page
    {
    private:
        static const std::string welcomeText;
        std::string adminUsername;
        std::string adminPass;

    public:
        WelcomePage(StudentSystem &system, std::string adminUsername, std::string adminPass);
        virtual ~WelcomePage() = default;
        virtual void show() override;
    };

} // namespace ui

#endif