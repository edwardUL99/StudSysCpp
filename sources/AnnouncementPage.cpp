#include "headers/AnnouncementPage.h"
#include "headers/studsys/DuplicateException.h"
#include "headers/UIUtils.h"
#include "headers/studsys/Announcement.h"

using std::string;
using ui::AnnouncementPage;

AnnouncementPage::AnnouncementPage(Account *account, Module *module, StudentSystem &system) : Page(system), account(account), module(*module) {}

void AnnouncementPage::viewAnnouncement(const Announcement &announcement)
{
    string time = system.getAnnouncementTime(announcement);

    Lecturer lecturer = announcement.getLecturer();
    cout << "Lecturer: " << lecturer.getName() << " <" << lecturer.getEmail() << ">" << endl;
    cout << "Time: " << time << endl;
    cout << "Subject: " << announcement.getSubject() << endl;
    cout << "Announcement:\n"
         << announcement.getAnnouncementText() << endl;
}

void AnnouncementPage::createAnnouncement()
{
    Lecturer lecturer = dynamic_cast<LecturerAccount *>(account)->getLecturer();

    cout << "Enter the subject for the announcement (max 250 characters): " << endl;
    string subject = ui::getString(Predicate<string>([](const string &s) -> bool { return s.length() < 1 || s.length() > 250; }), "Please enter a subject that is between 1 and 250 characters:");

    cout << "You can now enter the announcement text line by line. Type <!submit> to finish and to create the announcement or <!cancel> to cancel" << endl;

    string text = "";
    bool run = true;
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

string AnnouncementPage::extractCommand(string &line)
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

void AnnouncementPage::insertNewLine(std::vector<string> &lines, int &lineNumber, int &numberOfLines, bool &submit, bool &cancel)
{
    cout << ++lineNumber << " ";
    string newLine = ui::getString();

    if (newLine == "<!insert>")
    {
        lines.insert(lines.begin() + (lineNumber - 1), "");
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

bool AnnouncementPage::gotoLine(const string &input, int &lineNumber, int numberOfLines)
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

bool AnnouncementPage::processNormalEditing(std::vector<string> &lines, int &lineNumber, int &numberOfLines, bool &submit, bool &cancel)
{
    string line = lines[lineNumber - 1];

    cout << lineNumber << ") " << line << endl;
 
    cout << "~ ";
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
    }
    else
    {
        lineNumber++;
    }

    return true;
}

bool AnnouncementPage::processEndOfAnnouncement(std::vector<string> &lines, int &lineNumber, int &numberOfLines, bool &submit, bool &cancel)
{
    cout << "You are at the end of the original announcement, type <!insert> to insert another line, or <!submit>/<!cancel>" << endl;
    cout << "~ ";
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

void AnnouncementPage::processNormalInput(const string &line, std::vector<string> &lines, string &input, int &lineNumber, int numberOfLines)
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

void AnnouncementPage::deleteLine(std::vector<string> &lines, int &lineNumber, int numberOfLines)
{
    //delete the line you're at, i.e. remove it from the vector of lines, the next lineNumber beneath it will either be the next line, or if the end of the vector, you'll stay at the line
    lines.erase(lines.begin() + (lineNumber - 1)); //erase the line out of the vector
    numberOfLines--;

    if (lineNumber >= numberOfLines)
        lineNumber = numberOfLines; //if lineNumber is not past the last line, move the "cursor" to the very last line
    else
        lineNumber--;
}

bool AnnouncementPage::canEditAnnouncement(const Announcement &announcement)
{
    string announcementLecturer = announcement.getLecturer().getEmail();

    if (announcementLecturer != account->getEmail())
    {
        cout << "You cannot edit another lecturer's announcement, contact " << announcementLecturer << " to request they change it" << endl;
        return false;
    }
    return true;
}

void AnnouncementPage::editSubject(string &subject, const string &oldSubject)
{
    cout << "You are editing Announcement " << oldSubject << endl;
    cout << "\nEnter the new subject: " << endl;
    string newSubject = ui::getString();
    subject = newSubject == "" ? oldSubject : newSubject;
}

std::vector<string> AnnouncementPage::beginEditing(const Announcement &announcement)
{
    return ui::splitString(announcement.getAnnouncementText());
}

void AnnouncementPage::submitAnnouncement(std::vector<string> &lines, const Announcement &oldAnnouncement, const std::string &subject)
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
        cout << "This may be because there's no changes to update or an error occurred" << endl;
    }
}

int AnnouncementPage::getInitialEditLineNumber() {
    cout << "~ ";
    string input = ui::getString();
    string command = extractCommand(input);

    while (command != "<!goto>" && command != "<!cancel>") {
        cout << "Only command allowed presently is <!goto> or <!cancel>" << endl;
        cout << "~ ";
        input = ui::getString();
        command = extractCommand(input);
    }

    if (command == "<!cancel>") {
        return -1;
    }

    try {
        int lineNumber = stoi(input);
        return lineNumber;
    } catch (std::invalid_argument &inv) {
        cout << "Invalid value given for <!goto> command. Value must be a number" << endl;
        return getInitialEditLineNumber();
    }
}

void AnnouncementPage::editAnnouncement(const Announcement &announcement)
{
    if (!canEditAnnouncement(announcement))
        return;
    string subject = "";
    bool submit = false;
    bool cancel = false;

    editSubject(subject, announcement.getSubject());

    std::vector<string> lines = beginEditing(announcement);

    cout << "\nNow you are editing the text. Use command <!goto> line-number to go to a line to edit. When at the line, type <!append> or <!prepend> text to append/prepend that text on, leave blank to leave the same, <!clear> to make it blank, <!delete> to delete it. Type <!insert> to insert new line or <!submit> to save and submit, <!cancel> to cancel" << endl;

    int lineNumber = 1;

    for (string s : lines) {
        cout << lineNumber++ << ") " << s << endl;
    }

    lineNumber = getInitialEditLineNumber();

    if (lineNumber == -1) {
        cout << "Cancelling edit of announcement..." << endl;
        return;
    }

    string line;
    string text;

    bool run = true;
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

void AnnouncementPage::deleteAnnouncement(const Announcement &announcement)
{
    if (!canEditAnnouncement(announcement))
        return; //if the lecturer doesn't have edit rights, they also can't delete

    if (this->system.removeAnnouncement(announcement))
    {
        cout << "Announcement was removed successfully" << endl;
    }
    else
    {
        cout << "Announcement was not removed successfully, please try again later..." << endl;
    }
}

Announcement AnnouncementPage::chooseAnnouncement(std::vector<Announcement> &announcements)
{
    int length = announcements.size();

    if (length > 1)
    {
        int i = 1;
        for (const Announcement &announcement : announcements)
        {
            cout << i << ") " << announcement.getSubject() << "\tLecturer: " << announcement.getLecturer().getName() << ", Updated: " << system.getAnnouncementTime(announcement) << endl;
            i++;
        }
        cout << "Choose a number between 1 and " << length << " to choose the announcement (1 being the newest): " << endl;
        int num = ui::getInt(Predicate<int>([length](const int &x) -> bool { return x < 1 || x > length; }), "Choose a number between 1 and " + std::to_string(length) + ": ");

        ui::flushCinBuffer();

        num--;

        return announcements[num];
    } else {
        cout << "Only 1 announcement in module, choosing it" << endl;
        return announcements[0];
    }
}

void AnnouncementPage::editAnnouncements(std::vector<Announcement> &announcements)
{
    if (announcements.size() == 0)
    {
        cout << "There are no announcements" << endl;
    }
    else
    {
        Announcement announcement = chooseAnnouncement(announcements);

        editAnnouncement(announcement);
    }
}

void AnnouncementPage::viewAnnouncements(std::vector<Announcement> &announcements)
{
    if (announcements.size() == 0)
    {
        cout << "There are no announcements" << endl;
    }
    else
    {
        Announcement announcement = chooseAnnouncement(announcements);

        viewAnnouncement(announcement);
    }
}

void AnnouncementPage::show()
{
    std::vector<Announcement> announcements;

    while (true)
    {
        bool lecturer = dynamic_cast<LecturerAccount *>(account);

        if (lecturer)
            cout << "(N)ew announcement, (E)dit announcement, (D)elete announcement, (V)iew announcements, (B)ack, (Q)uit" << endl;
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
            editAnnouncements(announcements);
        }
        else if (choice == "D" && lecturer)
        {
            announcements = this->system.getModuleAnnouncements(module);
            Announcement announcement = chooseAnnouncement(announcements);
            deleteAnnouncement(announcement);
        }
        else if (choice == "V")
        {
            announcements = this->system.getModuleAnnouncements(module);
            viewAnnouncements(announcements);
        }
        else if (choice == "B")
        {
            ui::pageManager.popCurrentPage();
            break;
        }
        else if (choice == "Q")
        {
            ui::quit();
        }
    }
}