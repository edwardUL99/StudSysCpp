#ifndef UI_H
#define UI_H

#include <string>

namespace ui
{

    /**
 * Gets user choice from stdin (cin), trims it to 1 letter and returns it
 * E.g. if the options were (L)ecturer, (S)tudent
 * And user typed L, it would return L,
 * if typed (L), would trim parentheses and return L
 * if typed Lecturer would trim whole word but L
 */
    std::string getChoice();

    /**
 * Gets the entire string, more accurately line, typed by user.
 * Useful for entering names etc
 */
    std::string getString();

    /**
 * Gets string from stdin but hides the input on the terminal
 * Useful for inputting passwords
 */
    std::string getSecureString();

    /**
 * Gets integer from stdin
 */
    int getInt();

    /**
 * Gets float from stdin
 */
    float getFloat();

    /**
 * Quits from the UI and ends the program
 */
    void quit();

    //classes and other global functions/variables here
    //maybe a UI class managing all Pages
    //make sure you implement any methods/classes in UI.cpp
} // namespace ui

#endif