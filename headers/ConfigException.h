#ifndef CONFIG_EXCEPTION_H
#define CONFIG_EXCEPTION_H

#include <exception>
#include <string>

/**
 * Defines an exception related to config issues
 */
class ConfigException : public std::exception {
    private:
        std::string message;

    public:
        ConfigException(std::string message);
        virtual const char * what() const throw() override;
};

#endif