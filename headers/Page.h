#ifndef PAGE_H
#define PAGE_H

#include "StudentSystem.h"
#include "UIUtils.h"
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

namespace ui
{

    class Page
    {
    protected:
        StudentSystem &system;

    public:
        Page(StudentSystem &studentSystem);
        virtual ~Page() = default;
        virtual void show() = 0; //page is an abstract class
    };

} // namespace ui

#endif