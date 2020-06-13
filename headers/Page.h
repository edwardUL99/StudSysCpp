#ifndef PAGE_H
#define PAGE_H

#include "StudentSystem.h"
#include "UI.h"
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

class Page {
    protected:
        StudentSystem &system;

    public:
        Page(StudentSystem &studentSystem);
        virtual ~Page() = default;
        virtual void show() = 0; //page is an abstract class
};

#endif