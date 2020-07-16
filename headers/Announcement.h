#ifndef ANNOUNCEMENT_H
#define ANNOUNCEMENT_H

#include "DatabaseItem.h"
#include "Module.h"
#include "Lecturer.h"

//will keep time separate and have a struct as part of UI as e.g struct ui__announcement
//DatabaseManager provides no update method so it's best to remove the current one and add the new announcement when updating
class Announcement : public DatabaseItem
{
    private:
        Module module;
        Lecturer lecturer;
        std::string announcement;

    public:
        Announcement(const Module &module, const Lecturer &lecturer, std::string announcement);
        virtual ~Announcement() = default;
        Module getModule() const;
        void setModule(const Module &module);
        Lecturer getLecturer() const;
        void setLecturer(const Lecturer &lecturer);
        std::string getAnnouncementText() const;
        void setAnnouncementText(std::string announcement);
        virtual std::string getObjectType() const override;
        virtual Tables getTable() const override;
        virtual std::string getDescription() const override;
};

#endif