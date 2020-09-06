#ifndef MODULE_HOME_PAGE_H
#define MODULE_HOME_PAGE_H

#include "Page.h"
#include "studsys/Account.h"
#include "studsys/Module.h"
#include "studsys/Announcement.h"
#include <vector>

namespace ui
{

    /**
 * This page is the first page you "land" on after selecting a Module
 */
    class ModuleHomePage : public Page
    {
    private:
        Account *account;
        Module *module;
        /**
         * @brief This method displayes students that are registered on this oparticular module
         */
        void displayRegisteredStudents();
        void displayLecturerDetails();
        void printPrompt() const;
        bool isLecturerAccount() const;
        void viewExamGrades();
        void displayGradesInfo();
        std::vector<ExamGrade> getExamGradesByStudent(const std::vector<Exam> &exams, const Student &student);
        std::vector<ExamGrade> getExamGradesByExam(const Exam &exam, const std::vector<Student> &students);
        void calculateModuleGrades();

    public:
        /**
         * @param account the account that logged into this ModuleHomePage (i.e. account that is logged in)
         * @param module the module that this home page is for
         * @param system the backing system this UI is running on
         */
        ModuleHomePage(Account *account, Module *module, StudentSystem &system);
        virtual ~ModuleHomePage() = default;
        virtual void show() override;
    };

} // namespace ui

#endif