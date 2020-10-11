#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include "studsys/StudentSystem.h"
#include "studsys/DatabaseItem.h"
#include <stack>
#include <string>
#include <vector>
#include <map>

class DatabaseItem;

namespace ui
{
    class Page;
    
    /**
     * This class is intended to handle the pages in use by the system and to be the main parent caller of all the pages
     * It is to handle which page to display and to provide the services for pages to call another page
     * 
     * PageManager handles all pointers to any pages that are put through it. Any page added by a call to setNextPage() or start(Page*), the page manager
     * takes ownership of it.
     * 
     * This means that on destruction it also releases the memory taken by these pages.
     * These pages get destroyed whenever popCurrentPage() or the destructor gets called
     */
    class PageManager
    {
        private:
            std::string adminUsername;
            std::string adminPass;
            std::stack<Page*> pages;
            std::map<Page*, std::vector<DatabaseItem*>> sharedEntities; //entities stored by Pages 
            std::map<DatabaseItem*, int> timesStored; //keeps track of the amount of times the DatabaseItem is stored by pages
            StudentSystem system;
            bool run;
            //determines which Page to show next and calls its show() method. This method will be returned from as soon as that page is finished
            void showNextPage();
            /**
             * Attempts to delete the space taken by the entities stored by the specified page
             * Only uses delete if timesStored[item] == 1
             */
            void freeEntities(Page *page);

        public:
            /**
             * DOES NOTHING JUST USED WHEN NEEDED, I.E. DECLARING AS GLOBAL VARIABLE
             * CALL initializeSystem METHOD or else the system will not work. This can be left to the driver main function
             * 
             * This was done to only initalise the pagemanager object once and not copy two times, connecting to the database each time it does it
             */
            PageManager();

            ~PageManager();

            void initializeSystem(std::string database, std::string username, std::string password, std::string host, std::string adminUsername, std::string adminPass);

            StudentSystem &getSystem();

            /**
             * Sets the next page to be shown after the current page is shown.
             * If this is not called, after the current page is shown, it will return to the one before it
             */
            void setNextPage(Page *page);
            /**
             * Call this method to remove the page currently being displayed in the manager
             * This essentially removes the current pge and goes to the previous one that was in the manager before the current one
             */
            void popCurrentPage();
            /**
             * Returns the next page that will be displayed after the current one
             */
            Page* getNextPage();
            /**
             * Starts the page manager with the specified inital page and starts the StudentSystem
             */
            void start(Page *initialPage); 
            /**
             * Starts the page manager with the default initial page which is a Welcome Page and starts the StudentSystem
             */
            void start();
            /**
             * Adds a shared entity to the specified page
             * When these entities are freed depends on if they are in use by other pages
             * But they are guaranteed to be freed eventually before program termination
             */
            void addSharedEntity(Page *page, DatabaseItem *entity);
    };

} // namespace ui

#endif