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
#include <cstring>
#include <string.h>
#include <initializer_list>
#include <chrono>
using namespace std;
//"SRorSS" = '>' means doing SunSet; "SRorSS" = "<" means doing SunRise.
class SS {
private:
    static const int zero=0;
    static const unsigned long tokenColumn = 0;
    static const unsigned long replacementColumn = 1;
    const string default_template = \
    "SELECT * FROM tbl_sun_loc_site \
    WHERE lt::DATE='_DATE_' AND siteid=SITEID AND id BETWEEN \
    -BEFORESUNSET + ( SELECT id FROM (SELECT *,abs(zenithdistance-SUNSETDEF) \
    FROM tbl_sun_loc_site WHERE lt::DATE='_DATE_' AND siteid=SITEID AND events IS NULL AND id SRorSS (SELECT avg(id) FROM tbl_sun_loc_site \
    WHERE lt::DATE='_DATE_' AND siteid=SITEID AND events IS NULL)) a WHERE abs(zenithdistance-SUNSETDEF) = \
    (SELECT min(abs(zenithdistance-SUNSETDEF)) FROM tbl_sun_loc_site WHERE lt::DATE='_DATE_' AND siteid=SITEID AND events IS NULL AND id SRorSS \
    (SELECT avg(id) FROM tbl_sun_loc_site where lt::DATE='_DATE_' AND siteid=SITEID AND events IS NULL))  ) \
    AND     \
    +AFTERSUNSET + ( SELECT id FROM (SELECT *,abs(zenithdistance-SUNSETDEF) \
    FROM tbl_sun_loc_site WHERE lt::DATE='_DATE_' AND siteid=SITEID AND events IS NULL AND id SRorSS (SELECT avg(id) FROM tbl_sun_loc_site \
    WHERE lt::DATE='_DATE_' AND siteid=SITEID AND events IS NULL)) a WHERE abs(zenithdistance-SUNSETDEF) = \
    (SELECT min(abs(zenithdistance-SUNSETDEF)) FROM tbl_sun_loc_site WHERE lt::DATE='_DATE_' AND siteid=SITEID AND events IS NULL AND id SRorSS \
    (SELECT avg(id) FROM tbl_sun_loc_site WHERE lt::DATE='_DATE_' AND siteid=SITEID AND events IS NULL))   ) \
    ORDER BY id;";
    const int len_sunsetSQL_template = (int)default_template.length(); //Static is NOT needed because length of default_template \
    is determined during the execution phase.
    static char *const ptr_workArrayTemplate; //= workArrayTemplate;

    const multimap<string, string>  dictionary  = {
        {"Term",""},
        {"TERM",""},
        {"SR", "<"},        //Whereever you see sr replace SR with <
        {"SunRise", "<" },    // und so weiter und so weiter
        {"SUNRISE", "<"},     // und so weiter und so weiter
        {"sunrise", "<"},     // und so weiter und so weiter
        {"SS", ">"},          // und so weiter und so weiter
        {"SunSet", ">"},      // und so weiter und so weiter
        {"SUNSET", ">"},      // und so weiter und so fort
        {"sunset", ">"} //Whereever you see sunset replace sunset with >
    };
//
public:

    int i;
    int j;
    int kk; 
    static int number_of_tokens_passed_by_calling_program;
    static int numberOfPointerArrayElements;
    int inputTemplateLength;
    long resultingStringLength, resultingStringSize;
    unsigned long  arraySize;
//=    unsigned logn templateSize;
    bool debugMode;

    std::string sross; 
    std::string inputTemplate;
    std::string result1; 
    string work;
    string work2;
    
    unsigned int lengthOfCallersTemplate;
    const char *sr_or_ss = "SRorSS";
    
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
    std::multimap<std::string, std::string> pos;
    std::chrono::duration<long,std::ratio<1,1000000>> oneMicrosecond();
    
//=    char workArrayTemplate[templateBufferSize]; //Space reserved for holding, as a c-string, standard string found in \
    default_template, above.

    SS(int, char *, int, char **, bool);
    ~SS();
    void doFabricateSQL(int, char **, char *, char *, int, bool);
    string ptfss(string, string );
};



#endif /* fabricateSQLh */
