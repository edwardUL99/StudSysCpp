#include "headers/PageManager.h"
#include "headers/WelcomePage.h"
#include "headers/Page.h"
#include "headers/studsys/StudentSystem.h"

using std::stack;
using std::string;
using ui::Page;
using ui::PageManager;
using std::map;
using std::vector;

PageManager::PageManager()
{
    run = true;
}

PageManager::~PageManager()
{
    map<DatabaseItem*, bool> dontDelete; //keep track of database items already deleted

    for (map<Page*, vector<DatabaseItem*>>::iterator it = sharedEntities.begin(); it != sharedEntities.end(); it++) {
        vector<DatabaseItem*> vect = it->second;
        for (vector<DatabaseItem*>::iterator vIt = vect.begin(); vIt != vect.end(); vIt++) {
            DatabaseItem* item = *vIt;

            if (!dontDelete[item])
                delete *vIt;
            
            if (timesStored[item] > 1) {
                dontDelete[item] = true;
            }
        }
    }

    Page *page;

    while (!pages.empty())
    {
        page = pages.top();
        pages.pop();
        delete page;
    }
}

void PageManager::freeEntities(Page *page) {
    if (sharedEntities.find(page) != sharedEntities.end()) {
        vector<DatabaseItem*> &entities = sharedEntities[page];

        for (vector<DatabaseItem*>::iterator it = entities.begin(); it != entities.end(); it++) {
            DatabaseItem *item = *it;
            if (timesStored[item] <= 1) {
                delete item;
            } else {
                timesStored[item]--; //it's stored in a different vector so don't delete it
            }

            entities.erase(it--);
        }
    }
}

void PageManager::initializeSystem(string database, string username, string password, string host, string adminUsername, string adminPass)
{
    this->system.startSystem(database, username, password, host);
    this->adminUsername = adminUsername;
    this->adminPass = adminPass;
}

StudentSystem &PageManager::getSystem() {
    return system;
}

void PageManager::showNextPage()
{
    if (pages.size() == 0)
    {
        //no more pages to show, so signal to end the program
        run = false;
    }
    else
    {
        Page *nextPage = pages.top();
        nextPage->show();
    }
}

void PageManager::setNextPage(Page *page)
{
    pages.emplace(page);
}

void PageManager::popCurrentPage()
{
    Page *page = pages.top(); //the page to remove
    freeEntities(page);
    pages.pop();
    delete page;
}

Page *PageManager::getNextPage()
{
    if (pages.size() != 0)
    {
        return pages.top();
    }
    else
    {
        return nullptr;
    }
}

void PageManager::start(Page *initialPage)
{
    setNextPage(initialPage);

    while (run)
    {
        showNextPage();
    }
}

void PageManager::start()
{
    WelcomePage *welcome = new WelcomePage(this->system, adminUsername, adminPass);
    start(welcome);
}

void PageManager::addSharedEntity(Page *page, DatabaseItem *entity) {
    sharedEntities[page].push_back(entity);
    if (timesStored[entity] == 0) {
        timesStored[entity] = 1;
    } else {
        timesStored[entity]++;
    }
}