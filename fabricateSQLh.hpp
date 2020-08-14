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
using namespace std;
//"SRorSS" = '>' means doing SunSet; "SRorSS" = "<" means doing SunRise.
class SS {
private:
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
//=    static const int templateBufferSize = 2500; //Needs static because the value, to the right of the `=` sign, \
    is assigned during compilation
//=    static const int arrayBufferSize = 32;
//=    static const int arrayOfCopy = 20; //Enough space to hold 20 pointers to tokens and 20 pointers to replacements.
    static char *const ptr_workArrayTemplate; //= workArrayTemplate;
    static const int zero=0;
    static const int numberOfPointerArrayElements = 8;
    const multimap<string, string>  dictionary  = {
        {"SR", "<"},
        {"SunRise", "<" },
        {"SUNRISE", "<"},
        {"SS", ">"},
        {"SunSet", ">"},
        {"SUNSET", ">"}
    };
//
public:

    //=char workArrayRepl[arrayBufferSize] ={0};
    //=char *ptr_workArrayRepl;
    //=char workArrayToken[arrayBufferSize] = {0};
    //=char *ptrOfCopyOfTokenArray[arrayOfCopy];
    //=char *ptrOfCopyOfReplacementArray[arrayOfCopy] = {0};
    //=char *ptr_workArrayToken;
    int i;
    int j;
    int kk; 
    int number_of_tokens_passed_by_calling_program;
    int inputTemplateLength;
    long resultingStringLength, resultingStringSize;
    unsigned long  arraySize, templateSize;
    bool debugMode;
    //=char lastChar;
    //=static long *pos; /*Declaration of variable 'position' with deduced type 'auto' requires an initializer */
    std::string sross; 
    std::string inputTemplate;
    std::string result1; 
    string work;
    string work2;
    //=string stdStringArrayOfReplacementStrings[numberOfPointerArrayElements];
    unsigned int lengthOfCallersTemplate;
    const char *sr_or_ss = "SRorSS";
    char cstring_workArray[numberOfPointerArrayElements];
    //=char *ptr_workArray;
    char **ptrptrcopyOfReplacements[numberOfPointerArrayElements];
    char **ptrptrcopyOfTokens[numberOfPointerArrayElements];
    char *ptrcopyOfReplacements[numberOfPointerArrayElements]; 
    char *ptrcopyOfTokens[numberOfPointerArrayElements];
    string rpl; //Once converted to standard string from c-string, Stick the token's replacement value here
    std::regex tokenAsRegularExpression;
    int caller_supplied_buffer_size = zero;
    int cStringLength;
    int rc = zero;
//=    char workArrayTemplate[templateBufferSize]; //Space reserved for holding, as a c-string, standard string found in \
    default_template, above.

    SS(int, char *, int, char **, char **, bool);
    ~SS();
    void doFabricateSQL(int, char **, char **, char *, char *, int, bool);
    string ptfss(string, string );
};



#endif /* fabricateSQLh */
