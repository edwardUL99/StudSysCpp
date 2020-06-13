#include "headers/UI.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstring>

using std::cin;
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

int ui::getInt()
{
    int ret;
    cin >> ret;
    cin.ignore();
    return ret;
}

float ui::getFloat()
{
    float ret;
    cin >> ret;
    cin.ignore();
    return ret;
}

void ui::quit() {
    std::cout << "Goodbye, quitting..." << std::endl;
    exit(0);
}