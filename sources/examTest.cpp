#include "headers/ExamCreatePage.h"

int main() {
    StudentSystem system;
    ExamCreatePage create(system.getModule("CS4115"), system);

    create.show();

    return 0;
}