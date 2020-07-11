#ifndef UI_H
#define UI_H

#include <string>
#include "Predicate.h"
#include "PageManager.h"

namespace ui
{
  extern PageManager pageManager;

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

  //do the same as this with int and float

  /**
    * An overloaded version of getString() but takes a predicate function which returns a bool and takes a string and then passes the string got from getString into the function and iterates through it while the predicate function returns true
    * If retryMessage is passed this is printed each iteration
    * If the predicate returns false, the function immediately returns the string
    * @param predicate The predicate function which returns a bool. If the  predicate returns true, the function will keep requesting input
    * @param retryMessage an optional message to print each time the predicate returns true
    */
  std::string getString(const Predicate<std::string> &predicate, std::string retryMessage = "");

  /**
    * An std::function object for checking if a string entered as a module code is not 6 characters long
    */
  const Predicate<std::string> mcodepred([](const std::string &s) -> bool { return s.length() != 6; });

  /**
   * A message used for prompting the user to enter the correct module code of 6 characters long
   */
  const std::string mcoderetrymsg = "Please re-enter a module code that is 6 characters long: ";

  /**
    * An std::function object for checking if a string entered as a course code is not 5 characters long
    */
  const Predicate<std::string> ccodepred([](const std::string &s) -> bool { return s.length() != 5; });

  /**
   * A message used for prompting the user to enter the correct course code of 5 characters long
   */
  const std::string ccoderetrymsg = "Please re-enter a course code that is 5 characters long: ";

  /**
   * An std::function object for checking if the string is empty
   */
  const Predicate<std::string> emptystrpred([](const std::string &s) -> bool { return s == ""; });

  /**
   * A message used for prompting the user to enter a string that is not blank
   */
  const std::string emptystrretrymsg = "Please re-enter a string that is not empty: ";

  /**
   * Add others here as well as overloaded Int and Float with predicates
   */

  /**
 * Gets string from stdin but hides the input on the terminal
 * Useful for inputting passwords
 */
  std::string getSecureString();

  /**
  * Gets a secure string and then uses the predicate function to loop around if it returns true when passed the string and optionally prints the retryMessage if provided
  * @param predicate The predicate function. If it returns true the function will request input until the predicate returns false
  * @param retryMessage The optional message to prompt the user
  */
  std::string getSecureString(const Predicate<std::string> &predicate, std::string retryMessage = "");

  /*
  * A default predicate which you can use to check if password length is between 8 and 16 characters
  */
  const Predicate<std::string> passlengthpred([](const std::string &s) -> bool { int length = s.length(); return length < 8 || length > 16; });

  const std::string passlengthretrymsg = "Please re-enter a password that is between 8 and 16 characters long: ";

  /**
 * Gets integer from stdin
 */
  int getInt();

  /**
   * Gets an int and uses the predicate to determine if it should try again
   * @param predicate The predicate function. If it returns true the function will request input until the predicate returns false
   * @param retryMessage The optional message to prompt the user
   */
  int getInt(const Predicate<int> &predicate, const std::string retryMessage = "");


  /**
   * A predicate that can be used to retry input if the int input was <= 0
   */
  const Predicate<int> intltezeropred([](const int &x) -> bool { return x <= 0; });


  /**
   * A retry message to prompt user to re-enter an int > 0
   */
  const std::string ltezeroretrymsg = "Please re-enter a number that is greater than 0: ";

  /**
 * Gets float from stdin
 */
  float getFloat();

  /**
   * Gets float and then uses predicate to decide if it needs to be re-entered
   * @param predicate The predicate function. If it returns true the function will request input until the predicate returns false
   * @param retryMessage The optional message to prompt the user
  */
  float getFloat(const Predicate<float> &predicate, const std::string retryMessage = "");

  const Predicate<float> floatltezeropred([](const float &x) -> bool { return x <= 0; });

  /**
 * Quits from the UI and ends the program
 */
  void quit();

  //classes and other global functions/variables here
  //maybe a UI class managing all Pages
  //make sure you implement any methods/classes in UI.cpp
} // namespace ui

#endif