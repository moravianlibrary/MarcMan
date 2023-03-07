#include <iostream>

#define CUD_UNICODE_DATA "Unicode_Data.txt"

//using namespace std;

typedef unsigned int UINT32;
typedef unsigned char UINT8;

enum EGeneralCategory { EGC_default, EGC_Lu, EGC_Ll, EGC_Lt, EGC_Lm, EGC_Lo, EGC_Mn, EGC_Mc, EGC_Me, EGC_Nd, EGC_Nl, EGC_No, EGC_Pc, EGC_Pd, EGC_Ps, EGC_Pe, EGC_Pi, EGC_Pf, EGC_Po, EGC_Sm, EGC_Sc, EGC_Sk, EGC_So, EGC_Zs, EGC_Zl, EGC_Zp, EGC_Cc, EGC_Cf, EGC_Cs, EGC_Co, EGC_Cn };

enum EBidiClass { EBC_default, EBC_L, EBC_LRE, EBC_LRO, EBC_R, EBC_AL, EBC_RLE, EBC_RLO, EBC_PDF, EBC_EN, EBC_ES, EBC_ET, EBC_AN, EBC_CS, EBC_NSM, EBC_BN, EBC_B, EBC_S, EBC_WS, EBC_ON };

enum EDecompositionType { EDT_default, EDT_font, EDT_noBreak, EDT_initial, EDT_medial, EDT_final, EDT_isolated, EDT_circle, EDT_super, EDT_sub, EDT_vertical, EDT_wide, EDT_narrow, EDT_small, EDT_square, EDT_fraction, EDT_compat };

enum ENumericType { ENT_default, ENT_decimalDigit, ENT_digit, ENT_numeric };

enum EBidiMirrored { EBM_default, EBM_Y, EBM_N };

typedef struct Utf8Table {
    UINT8 cmask;
    UINT8 cval;
    short bits;
    UINT32 lval;
    UINT32 lmask;
} SUtf8Table;


class CUnicodeCharacterData {
    public:
        UINT32 code;
        std::string name;
        EGeneralCategory generalCategory;
        unsigned short canonicalCombiningClass;
        EBidiClass bidiClass;
        EDecompositionType decompositionType;
        std::vector<UINT32> decompositionMapping;
        ENumericType numericType;
        std::string numericValue;
        EBidiMirrored bidiMirrored;
        std::string unicode1Name;
        std::string isoComment;
        std::vector<UINT32> simpleUppecaseMapping;
        std::vector<UINT32> simpleLowercaseMapping;
        std::vector<UINT32> simpleTitlecaseMapping;
};

class CUnicodeCharacterDecomposition {
    public:
        UINT32 code;
        std::vector<UINT32> decomposition;
};

class CUnicodeDatabase {
    private:
        std::vector<CUnicodeCharacterData> dataTable;
        std::vector<CUnicodeCharacterDecomposition> decompositionTable;

        static SUtf8Table utf8Table[];

	std::map<std::string, EGeneralCategory> mapGeneralCategory;
	std::map<std::string, EBidiClass> mapBidiClass;
	std::map<std::string, EBidiMirrored> mapBidiMirrored;
	std::map<std::string, EDecompositionType> mapDecompositionType;

        int buildDecompositionTable();
        int parse(CUnicodeCharacterData&, const std::string&);
        int uCharDecompose(std::vector<UINT32>&, const UINT32);
        int unicodeDecompose(std::vector<UINT32>&, const std::vector<UINT32>&);
        int utf8ToUnicode(std::vector<UINT32>&, const std::string&);
        int unicodeToUtf8(std::string&, const std::vector<UINT32>&);
        CUnicodeCharacterData *unicodeCharacterDataFind(UINT32);

    public:
        int load(const std::string&);
        int utf8ToAscii(std::string&, const std::string&);
        CUnicodeDatabase();
};

