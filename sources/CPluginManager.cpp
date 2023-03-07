#include "CConvertor.h"

/* ---------------------------------------------------------
 * Auxiliary functions 
 * ---------------------------------------------------------
 */

//using namespace std;

void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = " ")
{
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the std::vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

/* ---------------------------------------------------------
 * CPluginManager class
 * ---------------------------------------------------------
 */

CPluginManager::CPluginManager() {
    log = &std::cerr;
    logLevel = 0;
    pluginManagerEnabled = 0;
}

CPluginManager::~CPluginManager() {
    if(plugins.size() != 0) {
        unregisterPlugins();
    }
}

int CPluginManager::isEnabled() {
    return pluginManagerEnabled;
}

std::string CPluginManager::getErrorMessage(){
    return errorMessage;
}

void CPluginManager::setErrorMessage(std::string message) {
    errorMessage = message;
}

void CPluginManager::setLog(std::ostream *logStream, int level) {
    log = logStream;
    logLevel = level;
}

int CPluginManager::registerPlugins(std::map<std::string, M_EXPRESSION_ELEMENT *> *configuration) {
    std::vector<std::string> pluginLibraries;
    std::vector<std::string>::iterator iter;
    std::string pluginsDirectory = "";

    if(configuration->count(CPM_PLUGINS_DIRECTORY) > 0) {
        pluginsDirectory = *((*configuration)[CPM_PLUGINS_DIRECTORY]->sName);
    }

    if(configuration->count(CPM_PLUGINS) == 0) {
        return 0;
    }

    pluginManagerEnabled = 1;

    tokenize(*((*configuration)[CPM_PLUGINS]->sName),pluginLibraries,",");

    for (iter = pluginLibraries.begin(); iter != pluginLibraries.end(); iter++) {
        registerPlugin(pluginsDirectory + *iter);
    }

    return 0;
}

int CPluginManager::registerPlugin(const std::string pluginPath) {
    DLIB_HANDLE handle;
    DLIB_HANDLE symbol;
    std::vector<std::string>::iterator iter;
    std::vector<std::string> functionList;
    std::string pluginName;

    if (!(handle = DLIB_OPEN(pluginPath.c_str(),RTLD_NOW))) {
        (*log) << "Unable to open plugin '" << pluginPath << "': " << DLIB_ERROR() << std::endl << std::flush;
    }
    else {
        if(!(symbol=(DLIB_HANDLE)DLIB_SYM(handle, "create"))) {
            (*log) << "Unable to create plugin: " << DLIB_ERROR() << std::endl << std::flush;
        }
        else {
            CMarcManPlugin *plugin = ((CMarcManPlugin*(*)())(symbol))();

            if(plugin) {
                pluginName = plugin->getName();
                functionList = plugin->getFunctionList();

                (*log) << "Plugin name: " << pluginName << std::endl << std::flush;

                for(iter = functionList.begin(); iter != functionList.end(); iter++) {
                    (*log) << "Function: " << pluginName + CPM_FUNCTION_DELIMITER + *iter << std::endl;
                }

                if(plugins[pluginName]) {
                    (*log) << "Plugin '" << pluginName << "' already registered" << std::endl << std::flush;
                    (*log) << "Unregistering previously registered plugin" << std::endl << std::flush;
                    unregisterPlugin(pluginName);
                }

                plugins[pluginName] = plugin;
                pluginHandles[pluginName] = handle;
            }
            else {
                (*log) << "Unable to instantiate plugin in '" << pluginPath << "'" << std::endl << std::flush;
            }
        }
    }

    return 0;
}

int CPluginManager::unregisterPlugins() {
    std::map<std::string, CMarcManPlugin *>::iterator iter;

    if(pluginManagerEnabled) {
        for(iter = plugins.begin(); iter != plugins.end(); iter++) {
            unregisterPlugin((*iter).first);
        }
    }

    return 0;
}

int CPluginManager::unregisterPlugin(const std::string pluginName) {
    DLIB_HANDLE handle;
    DLIB_HANDLE symbol;

    handle = pluginHandles[pluginName];
    if(handle) {
        if(!(symbol = (DLIB_HANDLE)DLIB_SYM(handle, "destroy"))) {
            (*log) << "Unable to destroy plugin '" << pluginName << "': " << DLIB_ERROR() << std::endl << std::flush;
            return 1;
        };
        ((int (*)())(symbol))();
        DLIB_CLOSE(handle);

        plugins.erase(pluginName);
        pluginHandles.erase(pluginName);
    }
    else {
        (*log) << "Unable to unregister plugin '" << pluginName << "': plugin not registered" << std::endl << std::flush;
        return 1;
    }

    return 0; 
}


int CPluginManager::loadPlugins(std::map<std::string, M_EXPRESSION_ELEMENT *> *configuration) {
    std::map<std::string, CMarcManPlugin *>::iterator iter;

    if(!pluginManagerEnabled) 
        return 0;

    for(iter = plugins.begin(); iter != plugins.end(); iter++) {
        if((*iter).second->load(configuration) != 0) {
            setErrorMessage("Unable to load plugin '" + (*iter).second->getName() + "'"); 
            return 1;
        }
    }

    return 0;
}

int CPluginManager::unloadPlugins() {
    std::map<std::string, CMarcManPlugin *>::iterator iter;

    if(!pluginManagerEnabled) 
        return 0;

    for(iter = plugins.begin(); iter != plugins.end(); iter++) {
        if((*iter).second->unload() != 0) {
            setErrorMessage("Unable to unload plugin '" + (*iter).second->getName() + "'"); 
            return 1;
        }
    }

    return 0;
}

int CPluginManager::callPluginFunction(CMarcAnalyzer *inputRecord, CMarcAnalyzer *outputRecord, const std::string *functionName, std::vector<M_EXPRESSION_ELEMENT *> *functionArguments, M_EXPRESSION_ELEMENT *returnValue) {
    std::vector<std::string> tokens;
    std::vector<std::string>::iterator iter;
    CMarcManPlugin *plugin;

    if(!pluginManagerEnabled)
        return 1;

    tokenize(*functionName,tokens,CPM_FUNCTION_DELIMITER);

    if(tokens.size() != 2) return 1;

    iter = tokens.begin();
    if(( plugin = plugins[*iter]) == 0)
        return 1;
    
    return plugins[*iter]->callFunction(inputRecord, outputRecord, &(*(iter+1)), functionArguments, returnValue);
}

