#ifndef LECTURER_EDIT_PAGE
#define LECTURER_EDIT_PAGE

#include "Page.h"

class Lecturer;

namespace ui {
    class LecturerEditPage : public Page {
        private:
            const Lecturer oldLecturer;
            Lecturer newLecturer;
            void setNameCaps(std::string &str, bool firstLetterUpper);
            std::string nameToEmailAddress(std::string name);
            std::string emailAddressToName(std::string email);
            void updateName(); //this should automatically update email and vice-versa
            void updateName(std::string name);
            void updateAge();
            void updateEmail(std::string email);
            void updateEmail();
            void updateDepartment();
            bool submitChanges();
        
        public:
            LecturerEditPage(const Lecturer &lecturer, StudentSystem &system);
            virtual ~LecturerEditPage() = default;
            virtual void show() override;
            
    };
}

#endif