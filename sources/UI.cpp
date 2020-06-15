#include "headers/UI.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstring>

using std::cin;
using std::cout;
using std::endl;
using std::string;

string ui::getChoice()
{

    string choice;

    getline(cin, choice);

    if (choice.length() > 1)
    {
        if (choice[0] == '(')
        {
            choice = choice.substr(1, 1);
        }
        else
        {
            choice = choice.substr(0, 1);
        }
    }

    choice[0] = toupper(choice[0]);

    return choice;
}

string ui::getString()
{
    string ret;
    getline(cin, ret);

    return ret;
}

string ui::getString(const std::function<bool(const string &)> &predicate, const string retryMessage) {
    string str = getString();

    while (predicate(str)) {
        if (retryMessage != "") {
            cout << retryMessage << endl;
        }

        str = getString();
    }

    return str;
}

string ui::getSecureString()
{
    string secure;

    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    getline(cin, secure);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return secure;
}

string ui::getSecureString(const std::function<bool(const string &)> &predicate, const string retryMessage) {
  string pass = getSecureString();

  while (predicate(pass)) {
    if (retryMessage != "") {
      cout << retryMessage << endl;
    }

    pass = getSecureString();
  }

  return pass;
}

int ui::getInt()
{
    int ret;
    cin >> ret;
    cin.ignore();
    return ret;
}

int ui::getInt(const std::function<bool(const int &)> &predicate, const string retryMessage) {
    int num = getInt();

    while (predicate(num)) {
        if (retryMessage != "") {
            cout << retryMessage << endl;
        }

        num = getInt();
    }

    return num;
}

float ui::getFloat()
{
    float ret;
    cin >> ret;
    cin.ignore();
    return ret;
}

float ui::getFloat(const std::function<bool(const float &)> &predicate, const string retryMessage) {
    float num = getInt();

    while (predicate(num)) {
        if (retryMessage != "") {
            cout << retryMessage << endl;
        }

        num = getFloat();
    }

    return num;
}

void ui::quit() {
    std::cout << "Goodbye, quitting..." << std::endl;
    exit(0);
}