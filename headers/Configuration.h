#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <vector>
#include "studsys/ConfigFileProcessor.h"
#include <map>

namespace ui {
    /**
     * This class provides configuration options to all classes that include it.
     * It is implemented as a singleton class.
     * This class will be used for when command line arguments are completely removed bar the -f argument
     */
    class Configuration {
        private:
            /**
             * The config processor backing this
             */
            ConfigFileProcessor *configProcessor;
            std::map<std::string, std::string> *propertyMap;
            Configuration();

        public:
            ~Configuration();
            /**
             * The config parameters that are allowed
             */
            const static std::vector<std::string> allowedConfigParams;
            /**
             * Gets the singleton instance of the configuration.
             * getInstance().initialise(std::string configFile) should be called once.
             */
            static Configuration& getInstance();
            /**
             * Initialises the Configuration with the specified configuration file.
             * Initialise only works once, after the first call, it's a no-op
             */
            void initialise(std::string configurationFile);
            /**
             * A backwards compatible initialise method to allow command line arguments to initialise with. This will be removed later when all arguments bar -f are removed
             * Initialise only works once, after the first call, it's a no-op.
             * The arguments are expected in the order:
             *  -d <database> -u <user> -p <password> -h <host> -au <admin_username> -ap <admin_password>
             */
            void initialise(char **argv);
            /**
             * Attempts to retrieve the value for the specified parameter.
             * @param parameter the config paramter to retrieve the value for
             * @return the value for the paramter, empty string if not found
             * @throw ConfigException if the parameter is not in the allowedConfigParams list or initalise wasn't called
             */
            std::string getProperty(std::string parameter);
            /**
             * Attempts to check if the config has the specified parameter an it has a value.
             * @param parameter to parameter to check
             * @return true if the paramater is present and has a value
             */
            bool hasProperty(std::string parameter);
    };
}

#endif