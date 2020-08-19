#ifndef ANNOUNCEMENT_PAGE_H
#define ANNOUNCEMENT_PAGE_H

#include "Page.h"
#include "studsys/Account.h"
#include "studsys/Module.h"
#include <vector>

namespace ui
{
    class AnnouncementPage : public Page
    {
    private:
        Account *account;
        Module module;
        void viewAnnouncement(const Announcement &announcement);
        void createAnnouncement();
        /**
         * @brief This subroutine is to be used by editAnnouncement to implement the <!insert> command
         * Takes in the vector of lines, creates a new line and inserts it into the lines vector
         * No return value, as the paramters are passed by reference
         * @param lines the vector of lines to work on
         * @param lineNumber is the lineNumber indicating the position of the editor in the announcement
         * @param numberOfLines the number of lines in the announcement. insertNewLine needs to be able to change this variable
         * @param submit A reference to the boolean that indicates whether an announcement should be submitted
         * @param cancel A reference to the boolean that indicates whether editing should be cancelled
         */
        void insertNewLine(std::vector<std::string> &lines, int &lineNumber, int &numberOfLines, bool &submit, bool &cancel);
        /**
         * @brief This subroutine is to be used by editAnnouncementto implement the <!goto> command
         * Takes in the line of input (assumed to be the trimmed input from <!goto>line by extractCommand, giving line), attempts to parse the number
         * It then uses that number and changes the line position in the editor
         * @param input The string inout that was given with the <!goto> command
         * @param lineNumber the reference to the current lineNumber position in the editor
         * @param numberOfLines the number of lines in the announcement being edited
         * @return whether the goto command was successful
         */
        bool gotoLine(const std::string &input, int &lineNumber, int numberOfLines);
        /**
         * @brief This subroutine is used by editAnnouncements to process the normal editing of an announcement, i.e when you are not at the end of the announcement
         * @param lines the vector of lines in the announcement
         * @param lineNumber the current lineNumber position in the announcement
         * @param numberOfLines the number of lines in the announcement
         * @param submit the reference to the submit flag of the editor
         * @param cancel the reference to the cancel flag of the editor
         * @return true if you are to keep editing, false otherwise
         */
        bool processNormalEditing(std::vector<std::string> &lines, int &lineNumber, int &numberOfLines, bool &submit, bool &cancel);
        /**
         * @brief This subroutine provides the processing for what happens when the editor reaches the end of the announcement
         * @param lines the reference to the vector of lines from the announcement
         * @param lineNumber the reference to the current line position in the editor
         * @param numberOfLines the reference to the number of lines in the announcement
         * @param submit the reference to the submit boolean flag in the editor
         * @param cancel the reference to the cancel boolean flag in the editor
         * @return true if you are to keep editing, false otherwise
         */
        bool processEndOfAnnouncement(std::vector<std::string> &lines, int &lineNumber, int &numberOfLines, bool &submit, bool &cancel);
        /**
         * @brief This subroutine provides the provessing for when input provided in editAnnouncement is not a single command with no input value
         * @param line the line content that the editor is currently on
         * @param lines the vector of lines in the announcement
         * @param input the input that was given
         * @param lineNumber the current lineNumber position in the editor
         * @param numberOfLines the number of lines in the announcement
         */
        void processNormalInput(const std::string &line, std::vector<std::string> &lines, std::string &input, int &lineNumber, int numberOfLines);
        /**
         * @brief This subroutine provides the functionality for the <!delete> command
         * @param lines the vector of lines in the announcement
         * @param lineNumber the current lineNumber position in the announcement
         * @param numberOfLines the number of lines in the announcement
         */
        void deleteLine(std::vector<std::string> &lines, int &lineNumber, int numberOfLines);
        /**
         * @brief This subroutine checks if you can edit this announcement
         * @param announcement the announcement to edit
         * @return true if you can edit
         */
        bool canEditAnnouncement(const Announcement &announcement);
        /**
         * @brief This subroutine edits the subject of the announcement
         * @param subject the subject to edit
         * @param oldSubject the old subject
         */
        void editSubject(std::string &subject, const std::string &oldSubject);
        /**
         * @brief This subroutine prints the initial instructions for editing and returns the vector of lines in the announcement
         * @param announcement the announcement to edit
         * @return vector of lines in the announcement
         */
        std::vector<std::string> beginEditing(const Announcement &announcement);
        /**
         * @brief this subroutine submits the edited announcement
         * @param lines the lines of the edited announcement. These lines contain the updated/new lines
         * @param oldAnnouncement the old unedited announcement
         * @param subject the new updated subject
         */
        void submitAnnouncement(std::vector<std::string> &lines, const Announcement &oldAnnouncement, const std::string &subject);
        void editAnnouncement(const Announcement &announcement);
        /**
         * @brief this subroutine removes the selected announcement from the system
         * @param announcement the announcement to remove
         */
        void deleteAnnouncement(const Announcement &announcement);
        //This method can be used to extract inlined commands like <!append> or <!prepend>, it expects them to be found at the start of the line
        std::string extractCommand(std::string &line);
        /**
         * @brief This subroutine is used to enter the edit announcement function.
         * It request which announcement to edit and then calls the editAnnouncement method on it
         * @param announcements the list of announcements for the current module
         */
        void editAnnouncements(std::vector<Announcement> &announcements);
        /**
         * @brief This subroutine provided the function to select which announcement to view
         * @param announcements the list of announcements for the current module
         */
        void viewAnnouncements(std::vector<Announcement> &announcements);
        /**
         * @brief Lists the announcements in the supplied vector in a formatted way and then returns the user chosen announcement
         * @param announcements the vector of announcements to list
         */
        Announcement chooseAnnouncement(std::vector<Announcement> &announcements);

    public:
        AnnouncementPage(Account *account, Module *module, StudentSystem &system);
        virtual ~AnnouncementPage() = default;
        virtual void show() override;
    };
} // namespace ui

#endif