#include "headers/UIUtils.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <climits>

using std::cin;
using std::cout;
using std::endl;
using std::string;

void ui::flushCinBuffer() {
    if (cin.fail()) {
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }
} 

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

string ui::getString(const Predicate<string> &predicate, const string retryMessage) {
    string str = getString();

    while (predicate(str)) {
        if (retryMessage != "") {
            cout << retryMessage << endl;
        }

        str = getString();
    }

    return str;
}

void ui::toUpper(string &str) {
    for (int i = 0; i < str.length(); i++) {
        char &ch = str[i];
        ch = toupper(ch);
    }
}

string ui::getCourseCode() {
    string code = getString(ccodepred, ccoderetrymsg);
    toUpper(code);
    return code;
}

string ui::getModuleCode() {
    string code = getString(mcodepred, mcoderetrymsg);
    toUpper(code);
    return code;
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

string ui::getSecureString(const Predicate<string> &predicate, const string retryMessage) {
  string pass = getSecureString();

  while (predicate(pass)) {
    if (retryMessage != "") {
      cout << retryMessage << endl;
    }

    pass = getSecureString();
  }

  return pass;
}

std::vector<string> ui::splitString(const string &s, char delim) {
    string sCopy = s;
    std::vector<string> tokens;
    std::string token;

    std::size_t pos = 0;

    while ((pos = sCopy.find(delim)) != std::string::npos) { //end if there's no string left
        tokens.push_back(sCopy.substr(0, pos)); //take the tok from start to just before delimiter
        sCopy = sCopy.substr(pos + 1); //remove first token and delimiter
    }

    tokens.push_back(sCopy); //add the last token to the vector

    return tokens;
}

string ui::rejoinString(const std::vector<string> &split, char delim) {
    string ret = "";

    for (string s : split) {
        if (s.find(delim) == std::string::npos) {
            ret += s + delim;
        } else {
            ret += s;
        }
    }

    ret = ret.substr(0, ret.size() - 1); //trim off last delimiter

    return ret;
}

int ui::getInt()
{
    flushCinBuffer();
    int ret;
    cin >> ret;
    cin.ignore();
    return ret;
}

int ui::getInt(const Predicate<int> &predicate, const string retryMessage) {
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
    flushCinBuffer();
    float ret;
    cin >> ret;
    cin.ignore();
    return ret;
}

float ui::getFloat(const Predicate<float> &predicate, const string retryMessage) {
    float num = getFloat();

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
    throw ExitSignal();
}

const char * ui::ExitSignal::what() const throw() {
    return "Exit Signal not caught";
}