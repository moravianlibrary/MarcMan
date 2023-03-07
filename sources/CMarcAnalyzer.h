#ifndef CMARCANALYZER_H
#define CMARCANALYZER_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <stdio.h>

extern int codeVersion;

//using namespace std;

#define LOAD_ISO_FORMAT   1
#define LOAD_TEXT_FORMAT  2
#define LOAD_ALEPH_FORMAT 3
#define NEW_ISO_FORMAT    4
#define NEW_TEXT_FORMAT   5

#define MAXBUF           5000
#define LENGTH_ID_ALEPH  9

#define VERSION_TESTING_E     3
 
extern char ISO_SUBFIELD_SEPARATOR[];
extern char ISO_FIELD_SEPARATOR[];
extern char ISO_RECORD_SEPARATOR[];
extern char TEXT_RECORD_SEPARATOR[];
extern char TEXT_SUBFIELD_SEPARATOR[];
extern char TEXT_FIELD_SEPARATOR[];
extern char ALEPH_RECORD_SEPARATOR[];
extern char ALEPH_SUBFIELD_SEPARATOR[];
extern char ALEPH_FIELD_SEPARATOR[];
extern char ALEPH_LAB_IDENTIFIKATOR[];

extern char LINKING_SUBFIELD_IDENTIFICATOR[];
extern char LINKING_SUBFIELD_SEP_ISO[];
extern char LINKING_SUBFIELD_SEP_TXT[];
extern char LINKING_SUBFIELD_SEP_ALEPH[];


class CField;

class CSubfield
{
public:
    int Index;
    std::string* Value;
    std::string* ID;
    CField* LinkingField;
	CSubfield();
    ~CSubfield();
};

class CField {
public:
    std::string* ID;
    std::string* ID1;
    std::string* ID2;
    int Index;
    std::vector<CSubfield*> ptrCollSubf;
	CField()
	{
	    ID=new std::string();
        ID1=new std::string(" ");
        ID2=new std::string(" ");
    }
    ~CField()
    {
        delete ID;
        delete ID1;
        delete ID2;
        for(int i=0;i<ptrCollSubf.size();i++)
        {
          delete ptrCollSubf[i];
        }  
    }
};




class CMarcAnalyzer {
public:
    std::vector<CField*> Fields;
    std::vector<CField*> ControlFields;
    std::vector<std::string*> aNoIdenField;
    std::vector<std::string*> aLinkingField;

    std::string g_Error;    
    std::string* g_Label;
    std::string* g_FieldPrefix;
    std::string* g_Format;
    
    int SetLog(std::ofstream* lg, int LogAllMes);
	std::ofstream* log;
	int LogAll;
    
	CMarcAnalyzer()
	{
       LogAll=0; 
	   g_Label=new std::string("                        ");
       g_FieldPrefix=new std::string("");
	   g_Format=new std::string("");
	}
	
    ~CMarcAnalyzer()
    {
       int i;
       for(i=0;i<aNoIdenField.size();i++)
             delete aNoIdenField[i];
       for(i=0;i<Fields.size();i++)
             delete Fields[i];  
       delete g_Label;
       delete g_FieldPrefix;
       delete g_Format;                
    }

public:
	std::string CheckRecord(std::string Lang);
	void LoadCheck(const char* sPath);
    int SortFields();
    int OpenRecordFromString(std::string* strRecord, int OpenMode);
	std::string* ValueByCSetPossWithPointers(std::string* Command, long loIF, long loIS, long PointerField, long PointerLinkingField, long PointerSubField);
	std::string* ValueByCommandSetPoss(std::string* Command, long loIF, long loIS);
	std::string* ValueByCommand(std::string* Command);
	void SetValueByCommand(std::string* Command, std::string* Value, long IField, long ISubfield, long Add);
	std::string* GetSerializeRecord(long Mode);
	long MaxIndexByCommand(std::string* Command, long FIndex);
    int FillFieldsDepth();
	void EmptyRecord();
    void CloneField(CField* Dest, CField* Sour);

private:
    std::string OrdinalForm(int number, int size);
    int SplitString(std::string* strSplit, std::vector<std::string*> * outArray, char* Separator);
    void Replace(std::string* strSplit, char* findStr, char* repStr);
    int AddOneField(std::string* sField, CField* pField, char* SubSeparator, char* LinkingSeparator);
    bool MayBeLinking(std::string Field); 
    int SerializeRecord(std::string *outString, std::string *Sizes, char* SubfieldSeparator, char* FieldSeparator, int Mode);
    int random_range(int lo, int hi);
    int FillSubfieldsDepth(std::vector<CSubfield*>* pcollS, int Link);
    std::string* GetValueFromC(std::string IDField,std::string LinkField, std::string IDSub,long IndexField,long IndexSubField, int Indikator, int Start, int Stop, long* PointerField, long* PointerSubField, long* PointerLinkingField);
    int RecognizeCommand(std::string* Comm, std::string *LAB, std::string *IDF, std::string *ILF, std::string *IDS, long* IF, long* IS, int* Indic, int* Sta, int* Sto);
    int SetValueFromC(std::string* Value, std::string *IDField,std::string* LinkField, std::string* IDSub,long IndexField,long IndexSubField, int Indikator, int Start, int Stop, int Add);
    long MaxLinkingSubfieldIndex(std::string* sIDF, std::string* sIDL, std::string* sSubID, long FIndex);
    long MaxSubfieldIndex(std::string* sIDF, std::string* sSubID, long FIndex);
    long MaxFieldIndex(std::string* sIDF);
    long MaxLinkingFieldIndex(std::string* sIDF, long IndexField, std::string* sIDL);
    void TrimRight(std::string* sStr, char* ArraySep);
    void TrimLeft(std::string* sStr, char* ArraySep);
    int MarkSections(std::string* strAnalyzeText, std::string* strMarks, char chBegin, char chEnd, char chMarkBegin,  char chMarkEnd, int iInsertEnd);
    int FillMarkSection(std::string* strMark, char chMarkBegin,  char chMarkEnd);
    void CutCommandByType(std::string* strCommand, std::string* strMarkCommand, std::vector<std::string *> *ptrElements, std::vector<std::string *> *ptrMarkElements);    
    int MarkSections2(std::string* strAnalyzeText, std::string* strMarks, char* chBegin, char* chEnd, char chMarkBegin,  char chMarkEnd, int iInsertEnd);
    std::string CheckSubfields( CField* ControlF,  CField* RedF, int WithLinking, std::string Lang);
    std::string CheckOneSubfield( std::string sField, CSubfield* ControlSF,  CSubfield* RecSF, int WithLinking, std::string Lang);
    int CheckChar( std::string sOneChar, std::string sCheckString);
    };

#endif
