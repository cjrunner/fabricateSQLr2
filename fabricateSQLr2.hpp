//
//  fabricateSQLh.hpp
//  fabricateSQL
//
//  Created by Clifford Campo on 8/8/20.
//  Copyright © 2020 CliffordCampo. All rights reserved.
//
//
//  generate default_template
//  playingWithRegexReplace
//
//  Created by Clifford Campo on 8/7/20.
//  Copyright © 2020 CliffordCampo. All rights reserved.
//
//
#ifndef fabricateSQLh
#define fabricateSQLh
//-------------------
#include <string>
#include <map>
#include <iomanip>
#include <iostream> // Maybe NOT needed because IO NOT done in a dylib.
#include <regex>
#include <array>
#include <string>
#include <cstring>
#include <string.h>
#include <initializer_list>
#include <chrono>
#include <strstream>
#include <sstream>
#include <stddef.h>
#include <fabricateSQLr2.h>
using namespace std;
//"SRorSS" = '>' means doing SunSet; "SRorSS" = "<" means doing SunRise.
/*
typedef struct timings {
    unsigned long constructorTime;
    unsigned long fabricateTime;
    unsigned long deleteTime;
    unsigned long totalTime;
} timings;
*/

class SS {

private:
    static const int zero=0;
    static const unsigned long tokenColumn = 0;
    static const unsigned long replacementColumn = 1;
    const string default_template = \
    "SELECT * FROM tbl_sun_loc_site \
    WHERE lt::DATE='_DATE_' AND siteid = SITEID AND id BETWEEN \
    - BEFOREDATAPOINTS + ( SELECT id FROM (SELECT *,abs(zenithdistance - SUNSETDEF) \
    FROM tbl_sun_loc_site WHERE lt::DATE='_DATE_' AND siteid=SITEID AND events IS NULL AND id SRorSS (SELECT avg(id) FROM tbl_sun_loc_site \
    WHERE lt::DATE='_DATE_' AND siteid = SITEID AND events IS NULL)) a WHERE abs(zenithdistance - SUNSETDEF) = \
    (SELECT min(abs(zenithdistance - SUNSETDEF)) FROM tbl_sun_loc_site WHERE lt::DATE='_DATE_' AND siteid = SITEID AND events IS NULL AND id SRorSS \
    (SELECT avg(id) FROM tbl_sun_loc_site where lt::DATE='_DATE_' AND siteid=SITEID AND events IS NULL))  ) \
    AND     \
    + AFTERDATAPOINTS + ( SELECT id FROM (SELECT *,abs(zenithdistance - SUNSETDEF) \
    FROM tbl_sun_loc_site WHERE lt::DATE='_DATE_' AND siteid=SITEID AND events IS NULL AND id SRorSS (SELECT avg(id) FROM tbl_sun_loc_site \
    WHERE lt::DATE='_DATE_' AND siteid = SITEID AND events IS NULL)) a WHERE abs(zenithdistance - SUNSETDEF) = \
    (SELECT min(abs(zenithdistance - SUNSETDEF)) FROM tbl_sun_loc_site WHERE lt::DATE='_DATE_' AND siteid = SITEID AND events IS NULL AND id SRorSS \
    (SELECT avg(id) FROM tbl_sun_loc_site WHERE lt::DATE='_DATE_' AND siteid = SITEID AND events IS NULL))   ) \
    ORDER BY id;";
    const int len_sunsetSQL_template = (int)default_template.length(); //Static is NOT needed because length of default_template \
    is determined during the execution phase.
    static char *const ptr_workArrayTemplate; //= workArrayTemplate;
    string term = "Term";
    void (*p2)(const string, const string, SS*); //pointer to function, named p1, which will point to ptfss.
    const multimap<string, string>  dictionary  = {
        {"SR", "<"},        //Whereever you see sr replace SR with <
        {"SunRise", "<" },    // und so weiter und so weiter
        {"SUNRISE", "<"},     // und so weiter und so weiter
        {"sunrise", "<"},     // und so weiter und so weiter
        {"SS", ">"},          // und so weiter und so weiter
        {"SunSet", ">"},      // und so weiter und so weiter
        {"SUNSET", ">"},      // und so weiter und so fort
        {"sunset", ">"}, //Whereever you see sunset replace sunset with >
        {"Term",""},
        {"TERM",""}
    };
//  ============================================================================================================================
public:
    const char *sr_or_ss = "SRorSS";
    int i;
    int j;
    int kk;
    const unsigned int number_of_tokens_passed_by_calling_program = 10;
//    static int numberOfPointerArrayElements;
    int inputTemplateLength;
    long resultingStringLength, resultingStringSize;
    unsigned long  arraySize;
//=    unsigned logn templateSize;
    bool debugMode;
    std::strstream buffer;
    std::string sross; 
    std::string inputTemplate;
    std::string result1; 
    string work;
    string work2;
    string workR;
    string workT;
    char **ptrPtrCstrWorkR;
    char **ptrPtrCstrWorkT;
    unsigned int lengthOfCallersTemplate;
    char *workCstring;
    char **ptrPtrCstrWork;
    string  **ptrPtrCopyOfTokensAsString;
    string  **ptrPtrCopyOfReplacementsAsString;
    char **ptrPtrCopyOfReplacements;
    char **ptrPtrCopyOfTokens;
//    char **ptrPtrCstring_WorkArray[numberOfPointerArrayElements];
    char **ptrPtrCstring_WorkArray;
/*
    char **ptrptrcopyOfReplacements[numberOfPointerArrayElements];
    char **ptrptrcopyOfTokens[numberOfPointerArrayElements];
    char *ptrcopyOfReplacements[numberOfPointerArrayElements]; 
    char *ptrcopyOfTokens[numberOfPointerArrayElements];
*/
    string rpl; //Once converted to standard string from c-string, Stick the token's replacement value here
    std::regex tokenAsRegularExpression;
    int caller_supplied_buffer_size = zero;
    int cStringLength;
    int rc = zero;
    int rc1 = zero;
    int maxIndex; 
    std::multimap<std::string, std::string> pos;
    std::chrono::duration<long,std::ratio<1,1000000>> oneMicrosecond();
    const string stringin1 = "one +"; //input string for ptfss;
    const string stringin2 = "two = three"; //input string for ptfss;
    string stringout; //output string for ptfss;
//=    char workArrayTemplate[templateBufferSize]; //Space reserved for holding, as a c-string, standard string found in \
    default_template, above.
    string stringTerm = "Term";
    string stringTERM = "TERM";
    const string *ptrS1 = &stringin1;
    const string *ptrS2 = &stringin2;
//    SS(char *, char *, int, char **, bool);
    SS(char *, int, char **, bool);
    ~SS();
 //   void ptfss(int theindex, const string *instring1, const string *instring2, SS *thisInstanceObject);
//    Returns the amount of data in the output buffer
// Debug mode (Provides output of intermediate results if ture) --------------+
// Amount of data in 1-D array of output data ----------------------+         |
// Pointer to 1-D array of output data ------------------------+    |         |
// Size of the token array ----------------------------+       |    |         |
// Pointer to 1-D Token Array --------------------+    |       |    |         |
// 2-D Array of Token/Replacement Pairs -+        |    |       |    |         |
// Timing Data, if NOT NULL ---+         |        |    |       |    |         |
//                             |         |        |    |       |    |         |
//                             V         V        V    V       V    V         V
    void doFabricateSQLr2(timings *, char **, char *, int, char *, size_t *, bool=true);
    void doFabricateSQLr2(           char **, char *, int, char *, size_t *, bool=false);
//    void doFabricateSQLr2(char **, char *, char *, int, bool);
    static void ptfss(int, const string *, const string *, SS* ); //Because this is the object of a pointer to function, named p1, \
    this function must be stored in static storage.
//    static void (*p1)(int, const string *, const string *, SS*); //pointer to function, named p1, which will point to ptfss.
 //   void (*fp)(int, const string *, const string *, SS* );
    void (*fp)(int, const string *, const string *, SS* );
    typedef struct mm { //Need the mm here or else we get an error message about not allowing pointer to function in "annonymous" struct.
        int index;
// void (*p1)(int, const string *, const string *, SS*); //Needs to have the `(int, const string *, const string *, SS*)`. \
        Just void (*p1) will not do!
        const string *firstString;
        const string *secondString;
    } mm;
    mm array[3];
    mm *ptrarray;
    
//Let's try, just for grins, to include a typedef within the multimap that contains a pointer to function; this approach would \
    encourage table-driven processing
    const map<string, mm> index_ptf {
        {"PTF0", array[0] },
        {"PTF1", array[1] },
        {"PTF2", array[2] }
    };
};
#endif /* fabricateSQLh */
