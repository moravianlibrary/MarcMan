#ifndef CRULESANALYZER_H
#define CRULESANALYZER_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <stdio.h>

//using namespace std;

#define MAXBUF 5000

//#define ADMIN_VERSION

extern int codeVersion;

#define VERSION_LIGHT         0
#define VERSION_STANDARD      1
#define VERSION_ADMIN         2
#define VERSION_TESTING       3

#define MARCMAN_VERSION       "2.3"

extern char* CUTTRIMED;

class CRuleItem 
{
public:
    std::string LAB;
    std::string IDF;
    std::string ILF;
    std::string IDS;
    std::string From;
    std::string To;
    long INDIC;
    long Start;
    long Stop;
    long g_Yes;
};


class CRule
{
public:
	std::string Command;
    std::string From;
    std::string To;
    std::vector<CRuleItem *> CollRulesItem;
    CRule()
	{
	}
    ~CRule()
    {
       for(int i=0;i<CollRulesItem.size();i++)
          delete CollRulesItem[i];
    }
};

class CCollRulesByC 
{
public:
	std::vector<int> IndexColl;
	std::vector<int> IndexInRuleColl;
    std::vector<CRule *> CCollection;
    void Add(CRule *ptrRule, int Index, int IndexInItem)
    {
       CCollection.push_back(ptrRule);
       IndexColl.push_back(Index);
       IndexInRuleColl.push_back(IndexInItem);
    }
};

class CRulesAnalyzer
{
public:
    std::string g_Error;
    std::vector<CRule*> CollRules;
    std::vector<CRule*> CollRulesByC;
    std::map<std::string,CCollRulesByC*> mapRulByC;
    int SetLog(std::ofstream* lg, int LogAllMes);
	std::ofstream* log;
	int LogAll;

	CRulesAnalyzer()
	{
		LogAll=0;
	}
    ~CRulesAnalyzer()
    {

       for(int i=0;i<CollRules.size();i++)
          delete CollRules[i];
       std::map<std::string,CCollRulesByC*>::iterator g;   
       for(g=mapRulByC.begin();g!=mapRulByC.end();g++)
       {
          delete g->second;
          //mapRulByC.erase(g);   
       }   
	   mapRulByC.clear();
    }

public:
	CCollRulesByC* GetRulesByC(std::string* Command);
	int Update();
	int OpenFromFile(const char* sPath);
    int IsForC(std::string* Comm);	   
    int OpenFromCodeFile(const char* sPath);	    
private:
    int CodeStringChar(char* sRet, int size);
    int MarkSections(std::string* strAnalyzeText, std::string* strMarks, char chBegin, char chEnd, char chMarkBegin,  char chMarkEnd, int iInsertEnd);
    int FillMarkSection(std::string* strMark, char chMarkBegin,  char chMarkEnd);
    void CutCommandByType(std::string* strCommand, std::string* strMarkCommand, std::vector<std::string *> *ptrElements, std::vector<std::string *> *ptrMarkElements);    
    int MarkSections2(std::string* strAnalyzeText, std::string* strMarks, char* chBegin, char* chEnd, char chMarkBegin,  char chMarkEnd, int iInsertEnd);
    int RecognizeCommand(std::string* Comm, std::string *LAB, std::string *IDF, std::string *ILF, std::string *IDS, long* IF, long* IS, int* Indic, int* Sta, int* Sto);    
    int SplitString(std::string* strSplit, std::vector<std::string*> * outArray, char* Separator);
    void TrimRight(std::string* sStr, char* ArraySep);
    void TrimLeft(std::string* sStr, char* ArraySep);    
    std::string OrdinalForm(int number, int size);    
    int LoadCommand(std::string* Comm, CCollRulesByC* newCOM);
    };

#endif
