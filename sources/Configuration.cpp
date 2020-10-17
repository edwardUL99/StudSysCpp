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
        map<string, string> &mapRef = *propertyMap;
        mapRef["database"] = argv[2];
        mapRef["user"] = argv[4];
        mapRef["pass"] = argv[6];
        mapRef["host"] = argv[8];
        mapRef["admin_enabled"] = "true"; // with command line arguments admin_enabled is always true
        mapRef["admin_user"] = argv[10];
        mapRef["admin_pass"] = argv[12];
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