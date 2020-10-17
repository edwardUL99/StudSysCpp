#include "headers/Configuration.h"
#include "headers/ConfigException.h"

using std::string;
using std::map;
using std::vector;
using ui::Configuration;

const vector<string> Configuration::allowedConfigParams({"database", "user", "pass", "host", "admin_enabled", "admin_user", "admin_pass"});

Configuration::Configuration() {
    configProcessor = NULL;
    propertyMap = NULL;
}

Configuration::~Configuration() {
    delete configProcessor;
    delete propertyMap;
}

Configuration& Configuration::getInstance() {
    static Configuration configuration;
    return configuration;
}

void Configuration::initialise(string configurationFile) {
    if (propertyMap == NULL) {
        configProcessor = new ConfigFileProcessor(configurationFile);
        propertyMap = new map<string, string>();

        if (!configProcessor->available()) {
            throw ConfigException("The configuration file " + configurationFile + " could not be read");
        } else {
            for (string s : allowedConfigParams) {
                string value = configProcessor->getValue(s);
                if (s == "admin_enabled") {
                    if (value == "")
                        value = "false";
                }
                (*propertyMap)[s] = value;
            }
        }
    }
}

void Configuration::initialise(char **argv) {
    if (propertyMap == NULL) {
        propertyMap = new map<string, string>();
        (*propertyMap)["database"] = argv[2];
        (*propertyMap)["user"] = argv[4];
        (*propertyMap)["pass"] = argv[6];
        (*propertyMap)["host"] = argv[8];
        (*propertyMap)["admin_enabled"] = "true"; // with command line arguments admin_enabled is always true
        (*propertyMap)["admin_user"] = argv[10];
        (*propertyMap)["admin_pass"] = argv[12];
    }
}

string Configuration::getProperty(string parameter) {
    if (propertyMap == NULL) {
        throw ConfigException("The Configuration has not been initialised. Did you call initialise first?");
    } else if (propertyMap->find(parameter) == propertyMap->end()) {
        // if propertyMap doesn't contain the parameter, the parameter isn't found in allowedConfigParams list as initialise would have added it otherwise
        throw ConfigException("The Configuration parameter " + parameter + " is not allowed");
    } else {
        return (*propertyMap)[parameter];
    }
}

bool Configuration::hasProperty(string parameter) {
    if (propertyMap == NULL) {
        throw ConfigException("The Configuration has not been initialised. Did you call initialise first?");
    } else {
        return propertyMap->find(parameter) != propertyMap->end() && propertyMap->at(parameter) != "";
    }
}