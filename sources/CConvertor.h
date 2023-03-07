#ifndef CCONVERTOR_H
#define CCONVERTOR_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "CUseScript.h"
#include "CMarcAnalyzer.h"
#include "CRulesAnalyzer.h"
#include "CPluginManager.h"

//using namespace std;

class CConfigFileReader
{
public:
    int ReadFile(char * sPath, std::map<std::string, M_EXPRESSION_ELEMENT*>* VIt);
	int SetLog(std::ofstream* lg, int LogAllMes);
	std::ofstream* log;
	int LogAll;
    std::map<std::string, M_EXPRESSION_ELEMENT*> VItems;
    ~CConfigFileReader()
    {
      std::map<std::string,M_EXPRESSION_ELEMENT*>::iterator j;
      for(j=VItems.begin();j!=VItems.end();j++)
      {
           // std::cout << "delete: " << j->first << "-" << *(j->second->sName)  << "\n";
            delete j->second;
            //VItems.erase(j);
      }  
	  VItems.clear();
    }
};

class CConfigArray
{
public:
    std::vector<M_EXPRESSION_ELEMENT *> VItems;
    ~CConfigArray()
    {
      for(int j=0;j!=VItems.size();j++)
      {
            delete VItems[j];
      }   
	  VItems.clear();
    }
};

class CConvertor;

class CConvertor
{
public:
    std::map<std::string,M_EXPRESSION_ELEMENT*> Variables;
    std::map<std::string,CConfigFileReader*> Tables;
    std::map<std::string,CConfigArray*> Arrays;
    int LoadAll(int Code);
    int DoneAll();
    int InsertRecord(std::string* Record, int Format);
    int Convert();
	int SetLog(std::ofstream* lg, int LogAllMes);
	std::ofstream* log;
	int LogAll;
    std::string sError;
    std::string* GetOut(int Format);
    int SaveCodeRulesFile(char *sPath);
    CConvertor()
    {
      LogAll=0; 
      MarcIn=new CMarcAnalyzer();
      MarcOut=new CMarcAnalyzer();
      RulesAnalyzer=new CRulesAnalyzer();
      PluginManager=new CPluginManager();
    }
    ~CConvertor()
    {
      delete MarcIn;
      delete MarcOut;
      //delete RulesAnalyzer;
      delete PluginManager;
      std::map<std::string,M_EXPRESSION_ELEMENT*>::iterator j;
      for(j=Variables.begin();j!=Variables.end();j++)
      {
           // std::cout << "delete: " << j->first << "\n"; 
           // std::cout << "uf: " << *(j->second->sName) << "\n";
            delete j->second;
      }      
	  Variables.clear();
      std::map<std::string,CConfigFileReader*>::iterator k;
      for(k=Tables.begin();k!=Tables.end();k++)
      {
         delete k->second;   
       //  Tables.erase(k);
      }   
	  Tables.clear();
      std::map<std::string,CConfigArray*>::iterator d;
      for(d=Arrays.begin();d!=Arrays.end();d++)
      {
         delete d->second;      
       //  Arrays.erase(d);
      }   
	  Arrays.clear();
      /*std::map<std::string,CUseScript*>::iterator w;
      for(w=Codes.begin();w!=Codes.end();w++)
      {
         delete w->second;
      //   Codes.erase(w);  
      }*/   
	  Codes.clear();
    }
//private:
    std::map<std::string,CUseScript*> Codes;
    CMarcAnalyzer* MarcIn;
    CMarcAnalyzer* MarcOut;
    CRulesAnalyzer* RulesAnalyzer;   
    CPluginManager* PluginManager;
    int ActualRule;
    
    int lPointerField;
    int lPointerSubField;
    int lPointerLinkingField;
    int DoingLinking;

    std::string* GroupExpand(std::string* sText);
    int DoElement(std::string* Value, std::string* Command, long iField, long iSubfield);
    int ClearErrors();
    int DoOneRecord();
    int DoRule(CUseScript* UseScript, std::string* sValue, std::string* sFrom, std::string* sTo, std::string* sCommand, long IndexRule, long iField, long iSubfield);
    void TrimRight(std::string* sStr, char* ArraySep);
    void TrimLeft(std::string* sStr, char* ArraySep);    
    int SplitString(std::string* strSplit, std::vector<std::string*> * outArray, char* Separator);
    int CreateVariable(std::string Key, int iType);
    int ChangePossition(std::vector<CSubfield*>* Coll, int Poss1, int Poss2); 
    int SortSubfields(std::vector<CSubfield*>* Coll, std::string* Command);    
    int IsInS(std::vector<std::string*>* St, CSubfield* SubF);   
};

#endif
