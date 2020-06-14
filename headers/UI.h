#ifndef UI_H
#define UI_H

#include <string>
#include <functional>

namespace ui
{

   /**
 * Gets user choice from stdin (cin), trims it to 1 letter, capitalises it and returns it
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
    * An overloaded version of getString() but takes a predicate function which returns a bool and takes a string and then passes the string got from getString into the function and iterates through it while the predicate function returns true
    * If retryMessage is passed this is printed each iteration
    * If the predicate returns false, the function immediately returns the string
    * @param predicate The predicate function which returns a bool. If the  predicate returns true, the function will keep requesting input
    * @param retryMessage an optional message to print each time the predicate returns true
    */
   std::string getString(const std::function<bool(const std::string &)> &predicate, std::string retryMessage = "");

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