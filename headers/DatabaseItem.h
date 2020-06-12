#ifndef DATABASE_ITEM_H
#define DATABASE_ITEM_H

#include <string>
#include "Tables.h"

class DatabaseItem
{
public:
    virtual ~DatabaseItem() = default;
    virtual std::string getObjectType() const = 0;
    virtual Tables getTable() const = 0;
    virtual std::string getDescription() const = 0;
};

#endif // DATABASE_ITEM_H