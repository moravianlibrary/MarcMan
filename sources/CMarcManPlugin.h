#ifndef CMARCMANPLUGIN_H
#define CMARCMANPLUGIN_H

#include <string>
#include <vector>
#include "CMarcAnalyzer.h"
#include "CUseScript.h"


#ifdef WIN32

#include <windows.h>
#define MM_EXPORT_PLUGIN(plugin) extern "C" { __declspec(dllexport) CMarcManPlugin * __stdcall create() { return new(plugin); } __declspec(dllexport) void __stdcall destroy(CMarcManPlugin *plugin) { delete(plugin); } }

#else

#define MM_EXPORT_PLUGIN(plugin) extern "C" { CMarcManPlugin * create() { return new(plugin); } void destroy(CMarcManPlugin *plugin) { delete(plugin); } }

#endif

//using namespace std;

class CMarcManPlugin {
    public:
        virtual std::string getName() = 0;
        virtual std::vector<std::string> getFunctionList() = 0;
        virtual int callFunction(CMarcAnalyzer *inputRecord, CMarcAnalyzer *outputRecord, const std::string *functionName, std::vector<M_EXPRESSION_ELEMENT *> *functionArguments, M_EXPRESSION_ELEMENT *returnValue) = 0;
        virtual int load(std::map<std::string, M_EXPRESSION_ELEMENT *> *configuration) { return 0; };
        virtual int unload() { return 0; };
};

#endif
