#include <vector>
#include <iostream>
#include <CMarcManPlugin.h>
#include "CUnicodeDatabase.h"

/* ---------------------------------------------------------
 * Plugin wrapper
 * ---------------------------------------------------------
 */

class CMarcManPlugin_unicode: public CMarcManPlugin {
   private:
      std::string name;
      std::vector<std::string> functionList;
      CUnicodeDatabase unicodeDatabase;

   public:
      virtual std::string getName() { return name; }
      virtual std::vector<std::string> getFunctionList() { return functionList; }
      virtual int callFunction(CMarcAnalyzer *inputRecord, CMarcAnalyzer *outputRecord, const std::string *functionName, std::vector<M_EXPRESSION_ELEMENT *> *functionArguments, M_EXPRESSION_ELEMENT *returnValue );
      virtual int load(std::map<std::string, M_EXPRESSION_ELEMENT *> *configuration);
      CMarcManPlugin_unicode();
};

CMarcManPlugin_unicode::CMarcManPlugin_unicode() {
   name = "unicode";
   functionList.push_back("Utf8ToAscii");
}

int CMarcManPlugin_unicode::load(std::map<std::string, M_EXPRESSION_ELEMENT *> *configuration) {

    if ((*configuration)["TablesDirectory"]) {
        std::string fileName = *((*configuration)["TablesDirectory"]->sName)+CUD_UNICODE_DATA;
        if(unicodeDatabase.load(fileName) != 0) {
            return 1;
        }
    }
    else {
        return 1;
    }

    return 0;
}

int CMarcManPlugin_unicode::callFunction(CMarcAnalyzer *inputRecord, CMarcAnalyzer *outputRecord, const std::string *functionName, std::vector<M_EXPRESSION_ELEMENT *> *functionArguments, M_EXPRESSION_ELEMENT *returnValue ) {

    if(*functionName == "Utf8ToAscii") {
        if(functionArguments->size() == 1 && (*functionArguments)[0]->iType == EX_STRING) {
            returnValue->iType = EX_STRING;
            if(unicodeDatabase.utf8ToAscii( *(returnValue->sName), *(*functionArguments)[0]->sName) == 0)
                return 0;
            else {
                returnValue->iType = EX_ERROR;
                *(returnValue->sName) = "Invalid arguments for function '"+(*functionName)+"' in plugin '"+name+"'";
                return 1;
            }
        }
        else {
            returnValue->iType = EX_ERROR;
            *(returnValue->sName) = "Invalid arguments for function '"+(*functionName)+"' in plugin '"+name+"'";
            return 1;
        }
     }
    else {
        returnValue->iType = EX_ERROR;
        *(returnValue->sName) = "Function '"+(*functionName)+"' not provided by plugin '"+name+"'";
        return 1;
    }
}

/* ---------------------------------------------------------
 * DO NOT EDIT THE LINES BELOW
 * ---------------------------------------------------------
 */

MM_EXPORT_PLUGIN(CMarcManPlugin_unicode)
