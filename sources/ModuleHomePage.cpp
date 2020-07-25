#include "headers/ModuleHomePage.h"
#include "headers/ExamSelectorPage.h"
#include "headers/UIUtils.h"
#include "headers/Administration.h"
#include "headers/NotFoundException.h"
#include "headers/DuplicateException.h"

#include <vector>
using std::string;
using std::vector;
using ui::ModuleHomePage;

ModuleHomePage::ModuleHomePage(Account &account, Module &module, StudentSystem &system) : Page(system), account(account), module(module) {}

void ModuleHomePage::viewAnnouncement(const Announcement &announcement)
{
    string time = system.getAnnouncementTime(announcement);

    Lecturer lecturer = announcement.getLecturer();
    cout << "Lecturer: " << lecturer.getName() << " <" << lecturer.getEmail() << ">" << endl;
    cout << "Time: " << time << endl;
    cout << "Subject: " << announcement.getSubject() << endl;
    cout << "Announcement:\n"
         << announcement.getAnnouncementText() << endl;
}

void ModuleHomePage::createAnnouncement()
{
    Lecturer lecturer = dynamic_cast<LecturerAccount &>(account).getLecturer();

    cout << "Enter the subject for the announcement (max 250 characters): " << endl;
    string subject = ui::getString(Predicate<string>([](const string &s) -> bool { return s.length() < 1 || s.length() > 250; }), "Please enter a subject that is between 1 and 250 characters:");

    cout << "You can now enter the announcement text line by line. Type <!submit> to finish and to create the announcement or <!cancel> to cancel" << endl;

    string text = "";
    bool run = true;
    bool submit = false;
    bool cancel = false;

    while (run)
    {
        string temp = ui::getString();

        if (temp != "<!submit>" && temp != "<!cancel>")
        {
            text += temp + "\n";
        }
        else if (temp == "<!submit>")
        {
            submit = true;
            run = false;
        }
        else if (temp == "<!cancel>")
        {
            cancel = true;
            run = false;
        }
    }

    if (cancel)
    {
        cout << "Announcement cancelled" << endl;
    }
    else
    {
        text = text.substr(0, text.length() - 1); //trim off the last \n

        Announcement announcement(module, lecturer, subject, text);

        try
        {
            if (this->system.addAnnouncement(announcement))
            {
                cout << "Announcement was created successfully" << endl;
            }
            else
            {
                cout << "Announcement was not created successfully, please try again later" << endl;
            }
        }
        catch (DuplicateException &d)
        {
            cout << "Announcement already exists" << endl;
        }
    }
}

string ModuleHomePage::extractCommand(string &line)
{
    string startTag = "<!";
    string endTag = ">";

    std::size_t pos = line.find(startTag); //find the start character first;

    if (pos == std::string::npos || pos != 0)
    {
        //no command found or its not at the start
        return "";
    }
    else
    {
        std::size_t pos1 = line.find(endTag);

        if ((pos1 - pos) > 10)
        {
            //no command is longer than 10 between end tag and <!, so this command is most likely badly formed so return no command
            return "";
        }
        else
        {
            string command = line.substr(pos, pos1 + 1);
            line = line.substr(pos1 + 1);
            return command;
        }
    }
}

void ModuleHomePage::insertNewLine(std::vector<string> &lines, int &lineNumber, int &numberOfLines, bool &submit, bool &cancel)
{
    cout << ++lineNumber << " ";
    string newLine = ui::getString();

    //text += line + "\n";
    //lines.insert(lines.begin() + (lineNumber - 1), line);

    if (newLine == "<!insert>") //need to test this recursion
    {
        lines.push_back("");
        numberOfLines++;
        insertNewLine(lines, lineNumber, numberOfLines, submit, cancel); //you want to insert again, so recursively call the insertNewLIne subroutine
    }
    else if (newLine == "<!submit>")
    {
        submit = true;
        return; //added so that we can break out of recursion if inert was issued numerous times
    }
    else if (newLine == "<!cancel>")
    {
        cancel = true;
        return;
    }
    else
    {
        //text += newLine + "\n";
        lines.insert(lines.begin() + (lineNumber - 1), newLine);
        numberOfLines++;
    }
}

bool ModuleHomePage::gotoLine(const string &input, int &lineNumber, int numberOfLines)
{
    if (input.size() == 0 || input == "")
    {
        cout << "No line number specified, continuing with line " << lineNumber << endl;
        return false;
    }
    else
    {
        try
        {
            int tempLineNumber = stoi(input);

            if (tempLineNumber <= 0 || tempLineNumber > numberOfLines)
            {
                cout << "The entered line number needs to be greater than 0 and less than " << numberOfLines << " continuing from line " << lineNumber << endl;
                return false;
            }
            else
            {
                lineNumber = tempLineNumber;
            }

            return true;
        }
        catch (std::invalid_argument &inv)
        {
            cout << "Invalid format for <!goto> specified, syntax is <!goto>line-number, continuing from line " << lineNumber << endl;
            return false;
        }
    }
}

bool ModuleHomePage::processNormalEditing(std::vector<string> &lines, int &lineNumber, int &numberOfLines, bool &submit, bool &cancel)
{
    string line = lines[lineNumber - 1];

    cout << line << endl;

    cout << lineNumber << " ";
    string input = ui::getString();

    if (input == "<!submit>")
    {
        submit = true;
        return false;
    }
    else if (input == "<!cancel>")
    {
        cancel = true;
        return false;
    }
    else if (input == "<!insert>")
    {
        insertNewLine(lines, lineNumber, numberOfLines, submit, cancel);
    }
    else if (input == "<!delete>")
    {
        deleteLine(lines, lineNumber, numberOfLines);
    }
    else if (input == "<!clear>")
    {
        //clear the line, don't delete it. i.e. make it a blank line
        lines[lineNumber - 1] = ""; //clear it by changing it to a blank
    }
    else if (input != "")
    {
        processNormalInput(line, lines, input, lineNumber, numberOfLines);
    } else {
        lineNumber++;
    }

    return true;
}

bool ModuleHomePage::processEndOfAnnouncement(std::vector<string> &lines, int &lineNumber, int &numberOfLines, bool &submit, bool &cancel)
{
    cout << "You are at the end of the original announcement, type <!insert> to insert another line, or <!submit>/<!cancel>" << endl;
    string input = ui::getString();

    if (input == "<!insert>")
    {
        insertNewLine(lines, lineNumber, numberOfLines, submit, cancel);
    }
    else if (input == "<!submit>")
    {
        submit = true;
        return false;
    }
    else if (input == "<!cancel>")
    {
        cancel = true;
        return false;
    }
    else if (extractCommand(input) == "<!goto>")
    {
        gotoLine(input, lineNumber, numberOfLines);
    }

    return true;
}

void ModuleHomePage::processNormalInput(const string &line, std::vector<string> &lines, string &input, int &lineNumber, int numberOfLines)
{
    string command = extractCommand(input);

    bool insertInput = true;

    if (command == "<!append>")
    {
        input = line + input; //add input to the end of the existing line;
    }
    else if (command == "<!prepend>")
    {
        input = input + line;
    }
    else if (command == "<!goto>")
    {
        gotoLine(input, lineNumber, numberOfLines);
        insertInput = false;
    }

    //text += input + "\n";
    if (insertInput)
    {
        lines[lineNumber - 1] = input;
        lineNumber++;
    }
}

void ModuleHomePage::deleteLine(std::vector<string> &lines, int &lineNumber, int numberOfLines)
{
    //delete the line you're at, i.e. remove it from the vector of lines, the next lineNumber beneath it will either be the next line, or if the end of the vector, you'll stay at the line
    lines.erase(lines.begin() + (lineNumber - 1)); //erase the line out of the vector
    numberOfLines--;

    if (lineNumber >= numberOfLines)
        lineNumber = numberOfLines; //if lineNumber is not past the last line, move the "cursor" to the very last line
    else
        lineNumber--;
}

bool ModuleHomePage::canEditAnnouncement(const Announcement &announcement)
{
    string announcementLecturer = announcement.getLecturer().getEmail();

    if (announcementLecturer != account.getEmail())
    {
        cout << "You cannot edit another lecturer's announcement, contact " << announcementLecturer << " to request they change it" << endl;
        return false;
    }
    return true;
}

void ModuleHomePage::editSubject(string &subject, const string &oldSubject)
{
    cout << "You are editing Announcement " << oldSubject << endl;
    cout << "\nEnter the new subject: " << endl;
    string newSubject = ui::getString();
    subject = newSubject == "" ? oldSubject : newSubject;
}

std::vector<string> ModuleHomePage::beginEditing(const Announcement &announcement)
{
    cout << "\nNow you are editing the text. To leave a line the same, press enter, to edit a line, type a new line (you can use <!append> or <!prepend> at the start of the line to append or prepend the existing line onto the new line, <!goto>line to change line) to change it, <!clear> to make a line blank or <!delete> to delete the current line, or to submit now and exit edit, type <!submit>, to cancel edits, type <!cancel>" << endl;
    return ui::splitString(announcement.getAnnouncementText());
}

void ModuleHomePage::submitAnnouncement(std::vector<string> &lines, const Announcement &oldAnnouncement, const std::string &subject)
{
    string text = ui::rejoinString(lines);
    Module announcementModule = oldAnnouncement.getModule();
    Announcement updatedAnnouncement(oldAnnouncement.getID(), announcementModule, oldAnnouncement.getLecturer(), subject, text);

    if (this->system.updateAnnouncement(updatedAnnouncement.getID(), announcementModule.getCode(), updatedAnnouncement))
    {
        cout << "The announcement has been updated successfully" << endl;
    }
    else
    {
        cout << "The announcement has not been updated successfully, please try again later" << endl;
    }
}

void ModuleHomePage::editAnnouncement(const Announcement &announcement)
{
    if (!canEditAnnouncement(announcement))
        return;
    string subject = "";
    bool submit = false;
    bool cancel = false;

    editSubject(subject, announcement.getSubject());

    std::vector<string> lines = beginEditing(announcement);

    string line;
    string text;

    bool run = true;
    int lineNumber = 1;
    int numLines = lines.size();

    while (run)
    {
        if (lineNumber <= numLines)
        {
            run = processNormalEditing(lines, lineNumber, numLines, submit, cancel);
        }
        else
        {
            run = processEndOfAnnouncement(lines, lineNumber, numLines, submit, cancel);
        }
    }

    if (submit)
    {
        submitAnnouncement(lines, announcement, subject);
    }
    else if (cancel)
    {
        cout << "Discarding changes to the announcement..." << endl;
    }
}

void ModuleHomePage::show()
{
    string code = module.getCode();

    cout << "Welcome to the homepage for Module " << code << "\n"
         << endl;

    bool run = true;

    while (run)
    {
        cout << "View: (A)nnouncements, (T)asks, (E)xams, (S)tudent list, (L)ecturer details; Change (M)odule settings, (B)ack, (Q)uit" << endl;

        string choice = ui::getChoice();

        if (choice == "A")
        {
            vector<Announcement> announcements;

            while (true)
            {
                bool lecturer = false;
                try
                {
                    LecturerAccount &lecturerAcc = dynamic_cast<LecturerAccount &>(account);
                    lecturer = true;
                }
                catch (std::bad_cast &b)
                {
                }

                if (lecturer)
                    cout << "(N)ew announcement, (E)dit announcement, (V)iew announcements, (B)ack, (Q)uit" << endl;
                else
                    cout << "(V)iew announcements, (B)ack, (Q)uit" << endl;

                string choice = ui::getChoice();

                if (choice == "N" && lecturer)
                {
                    createAnnouncement();
                }
                else if (choice == "E" && lecturer)
                {
                    announcements = this->system.getModuleAnnouncements(module);
                    int length = announcements.size();

                    if (length == 0)
                    {
                        cout << "There are no announcements" << endl;
                    }
                    else
                    {
                        int i = 1;
                        for (const Announcement &announcement : announcements)
                        {
                            cout << i << ") " << announcement.getSubject() << endl;
                            i++;
                        }
                        cout << "Choose a number between 1 and " << length << " to choose which announcement to edit (1 being the newest): " << endl;
                        int num = ui::getInt(Predicate<int>([length](const int &x) -> bool { return x < 1 || x > length; }), "Choose a number between 1 and " + std::to_string(length) + ": ");

                        ui::flushCinBuffer();

                        num -= 1;

                        editAnnouncement(announcements[num]);
                    }
                }
                else if (choice == "V")
                {
                    announcements = this->system.getModuleAnnouncements(module);
                    int length = announcements.size();

                    if (length == 0)
                    {
                        cout << "There are no announcements" << endl;
                    }
                    else
                    {
                        int i = 1;
                        for (const Announcement &announcement : announcements)
                        {
                            cout << i << ") " << announcement.getSubject() << endl;
                            i++;
                        }
                        cout << "Choose a number between 1 and " << length << " to choose which announcement to view (1 being the newest): " << endl;
                        int num = ui::getInt(Predicate<int>([length](const int &x) -> bool { return x < 1 || x > length; }), "Choose a number between 1 and " + std::to_string(length) + ": ");

                        ui::flushCinBuffer();

                        num -= 1;

                        viewAnnouncement(announcements[num]);
                    }
                }
                else if (choice == "B")
                {
                    break;
                }
                else if (choice == "Q")
                {
                    ui::quit();
                }
            }
        }
        else if (choice == "T")
        {
            cout << "Not implemented yet" << endl;
        }
        else if (choice == "E")
        {
            ExamSelectorPage *selectorPage = new ExamSelectorPage(this->account, this->module, this->system);
            ui::pageManager.setNextPage(selectorPage);
            run = false;
        }
        else if (choice == "S")
        {
            cout << "Students registered on Module " << code << ":" << endl;

            int i = 1;

            for (const Student &student : this->system.getStudentsRegisteredOnModule(module))
            {
                cout << "\t" << i++ << ") " << student.getName() << " - " << student.getID() << " - " << student.getEmail() << endl;
            }

            bool lecturer = false;

            try
            {
                LecturerAccount &lecturerAccount = dynamic_cast<LecturerAccount &>(account);
                lecturer = true; //if the cast succeeded, it is a lecturer account
            }
            catch (std::bad_cast &e)
            {
            }

            if (lecturer)
            {
                while (true)
                {
                    cout << "Would you like to register a student on the module? (R)egister, (D)e-register, (N)o" << endl;

                    choice = ui::getChoice();

                    if (choice == "R")
                    {
                        Administration admin(this->system);
                        admin.registerStudent(module.getCode());
                        break;
                    }
                    else if (choice == "D")
                    {
                        cout << "Enter the Student ID for the student to de-register: " << endl;

                        int id = ui::getInt(ui::intltezeropred, ui::ltezeroretrymsg);

                        try
                        {
                            Student student = this->system.getStudent(id);

                            if (this->system.unregisterStudentModule(student, module))
                            {
                                cout << "Student " << id << " de-registered from module " << module.getCode() << " successfully" << endl;
                            }
                            else
                            {
                                cout << "Student " << id << " de-registered from module " << module.getCode() << " unsuccessfully, please try again later" << endl;
                            }
                        }
                        catch (NotFoundException &nf)
                        {
                            cout << "Student " << id << " does not exist" << endl;
                        }

                        break;
                    }
                    else if (choice == "N")
                    {
                        break;
                    }
                }
            }
        }
        else if (choice == "L")
        {
            Lecturer lecturer = module.getLecturer();

            cout << "The lecturer for this module is: " << endl;
            cout << "\tName: " << lecturer.getName() << endl;
            cout << "\tDepartment: " << lecturer.getDepartment() << endl;
            cout << "\tE-mail Address: " << lecturer.getEmail() << endl;
        }
        else if (choice == "M")
        {
            cout << "Not implemented yet" << endl;
        }
        else if (choice == "B")
        {
            cout << "Going back to module selector page\n"
                 << endl;
            run = false;
            ui::pageManager.popCurrentPage();
        }
        else if (choice == "Q")
        {
            ui::quit();
        }
    }
}