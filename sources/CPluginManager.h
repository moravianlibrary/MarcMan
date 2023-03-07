#ifndef CPLUGINMANAGER_H
#define CPLUGINMANAGER_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "CUseScript.h"
#include "CMarcManPlugin.h"
#include "CMarcAnalyzer.h"
#include "dlwrapper.h"

#define CPM_PLUGINS_DIRECTORY    "PluginsDirectory"
#define CPM_PLUGINS              "Plugins"
#define CPM_FUNCTION_DELIMITER   "."

class CPluginManager {
   public:
      CPluginManager();
      ~CPluginManager();
      std::string getErrorMessage();
      void setLog(std::ostream *log, int level);
      int registerPlugins(std::map<std::string, M_EXPRESSION_ELEMENT *> *configuration);
      int unregisterPlugins();
      int loadPlugins(std::map<std::string, M_EXPRESSION_ELEMENT *> *configuration);
      int unloadPlugins();
      int callPluginFunction(CMarcAnalyzer *inputRecord, CMarcAnalyzer *outputRecord, const std::string *name,std::vector<M_EXPRESSION_ELEMENT *> *args, M_EXPRESSION_ELEMENT *retVal);
      int isEnabled();
   private:
      std::ostream *log;
      int logLevel;
      int pluginManagerEnabled;
      std::map <std::string, DLIB_HANDLE> pluginHandles;
      std::map <std::string, CMarcManPlugin *> plugins;
      std::string errorMessage;
      
      void setErrorMessage(std::string message);
      int registerPlugin(const std::string pluginPath);
      int unregisterPlugin(const std::string pluginame);
};

#endif
