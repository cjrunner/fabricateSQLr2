// ------------------------------------------------------+
//  fabricateSQLh.cpp                                    |
//  fabricateSQL                                         |
//                                                       |
//  Created by Clifford Campo on 8/8/20.                 |
//  Copyright © 2020 CliffordCampo. All rights reserved. |
//                                                       |
// ------------------------------------------------------+
#include "fabricateSQLr2.hpp"

// extern "C" void fabricateSQLr2(timings *, char **, char *, char *, int, bool=false); //This is used so a "C" program can call \
this c++ program
//
//void fabricateSQL(int, char *, char *, char *, char *, int);
// If true then in debug mode. Default is false ---------------------------------------------------------------------------------+
// Pointer to amount of data fabricateSQLr2 has placed in return buffer (cStringBuffer)--------------------------+               |
// pointer to location where fabricateSQLr2 will place the SQL it just constructed  ----------------+            |               |
// Template size. If bufsize = 0 then use the default template -----------------------+             |            |               |
// Caller's SQL Template ---------------------------------------+                     |             |            |               |
//                                                              |                     |             |            |               |
// token/replacement 2-dimensional array --+                    |                     |             |            |               |
// timing buffer (if needed,+              |                    |                     |             |            |               |
//    can be nullptr if     |              |                    |                     |             |            |               |
//    timing NOT needed)    |              |                    |                     |             |            |               |
//                          |              |                    |                     |             |            |               |
//                          V              V                    V                     V             V            V               V
void fabricateSQLr2(timings *sb, char **theTokensReplacements, char *oSQLt, int bufsize, char *cStringBuffer, size_t *outsize, bool debug) {
    //Note: fabricateSQL serves as an API between the caller and doFabricateSQL, which performs the work of transforming \
    the caller's SQL template into executable SQL.
    // initialize output stream with that output buffer
    if (debug) {
        cout << "0. =================================== entering fabricateSQLr2 in debug mode ===================================" \
        << "\n1. input parameters look like: " ;
        if (sb) {
            cout \
            << "\nsb (buffer for containing performance data): " \
            << sb;
        }
        cout \
        << ":\ntheTokensReplacements " \
        << theTokensReplacements \
        << "\ncStringBuffer: " \
        << cStringBuffer \
        << "\t*cStringBuffer: " \
        << *cStringBuffer \
        << "\nbufsize: " \
        << bufsize \
        << "\ndebug: " \
        << debug \
        << endl;
        if (*oSQLt != 0) {
            cout \
            << "\noSQLt: " \
            << oSQLt \
            << "\t*oSQLt: "
            << *oSQLt \
            << endl;

        } else {
            cout << "*oSQLt is nullptr, therefore will use our own internally defined SQL template." << endl;
        }
    } //End of debug mode output
    
    if (sb) { /* Will record performance data if sb is NOT null but instead points to a valid caller supplied area of memory, \
        as mapped by include file /usr/local/include/fabricateSQL/fabricateSQLr2.h per the timings struct.
    */
        auto start = std::chrono::steady_clock::now();
        auto tpconstructorstart = std::chrono::steady_clock::now();
        SS *ptrSS = new  SS(oSQLt, bufsize, theTokensReplacements, debug); 
        auto tpconstructorend = std::chrono::steady_clock::now();
        
        auto dfs = std::chrono::steady_clock::now();
        ptrSS->doFabricateSQLr2( theTokensReplacements, oSQLt, bufsize, cStringBuffer, outsize, debug);
        auto dfe= std::chrono::steady_clock::now();
        
        auto do_delete_start = std::chrono::steady_clock::now();
        delete ptrSS;   //Delete the instance object , ptrSS, we created before calling doFabricateSQL
        auto tpdodeleteend = std::chrono::steady_clock::now();

        auto end = chrono::steady_clock::now();
        sb->constructorTime = (tpconstructorend-tpconstructorstart).count();
        sb->fabricateTime = (dfe-dfs).count();
        sb->deleteTime = (tpdodeleteend-do_delete_start).count();
        sb->totalTime = (end-start).count();
        if (debug) cout << "a) std::chrono::nanoseconds::period::num " << std::chrono::nanoseconds::period::num \
        << "\nb) std::chrono::nanoseconds::period::den: " << std::chrono::nanoseconds::period::den  << "\n=================\n" \
        << "\nc) It took " << sb->constructorTime \
        << " n-sec to do constructor processing;\nd) It took " \
        << sb->fabricateTime \
        << " n-sec to do doFabricateSQL processing;\ne) It took " \
        << sb->deleteTime \
        << " n-sec to do delete processing.\n" \
        << "f) Total execution time " \
        << sb->totalTime \
        << " n-sec." << endl;
        
//        cout << buffer.str() << "\nand is " << buffer.pcount() << " bytes long." << endl;
//        memcpy(sb, buffer.str(), buffer.pcount() );
//        sb = buffer.str();
    } else {
        SS *ptrSS = new  SS(oSQLt, bufsize, theTokensReplacements, debug);
        ptrSS->doFabricateSQLr2( theTokensReplacements, oSQLt, bufsize, cStringBuffer, outsize, debug);
        delete ptrSS;   //Delete the instance object , ptrSS, we created before calling doFabricateSQL
    } //End of timing buffer setup.
    
} //Return to caller.

void SS::doFabricateSQLr2(char **theTokensReplacements, char *SQLt, int bufsize, char *cStringBuffer, size_t *outsize, bool debug) {
    ptrarray = &array[0];
    array[0].index = 0;
    array[0].firstString = ptrS1;
    array[0].secondString = ptrS2;
    array[1].index = 1;
    array[1].firstString = ptrS1;
    array[1].secondString = ptrS2;
    array[2].index = 2;
    array[2].firstString = ptrS1;
    array[2].secondString = ptrS2;
    fp = &SS::ptfss; //This works! function pointer, named fp, does make it to member function SS::ptfss.
//    ptr_fun(<#_Result (*__f)(_Arg)#>) = SS::ptfss;
//    ptr_fun( (*SS::ptfss)(i, ptrS1, ptrS2, this  ));
//    (ptrarray+0)->p1 = SS::ptfss; //(rc, "The quick Brown Fox Jumped ", "… over the lazy dog!", this);
//    (ptrarray+1)->p1 = SS::ptfss; //(rc, "Every Good Boy … ", "Does Fine!", this);
//    (ptrarray+2)->p1 = SS::ptfss; //(rc, "Hi diddle diddle the cat and the fiddle ", "… And the dish ran away with the spoon!", this);
    for (i = 0; i < maxIndex; i++) { //Beginning of the for loop #2
        result1.clear(); //Now clear the output result1 so we don't append onto the previous output.
        work.clear();
        if(debug) cout << i <<". *ptrPtrCopyOfTokensAsString[" << i << "] looks like: " << *ptrPtrCopyOfTokensAsString[i]  << endl;
        cStringLength = (int)strlen(ptrPtrCopyOfTokens[i]);
        work.assign(ptrPtrCopyOfTokens[i], cStringLength );
        if (debug ) cout << "2. The token is: theTokensReplacements[i][0]: " << theTokensReplacements[i][0] << "\twork: " << work << endl;
        tokenAsRegularExpression.assign( work );
        
        rpl.clear();
        cStringLength = (int)strlen(ptrPtrCopyOfReplacements[i]);
        rpl.assign(ptrPtrCopyOfReplacements[i], cStringLength);
        if (debug ) cout << "3. The replacement for above token is: theTokensReplacements[i][1]: "<< theTokensReplacements[i][1] \
            << "\trpl: " << rpl << endl;
        // Now let regular expression do its thing.
        std::regex_replace (
            back_inserter(result1),  //Destination
            inputTemplate.begin(), inputTemplate.end(),      //Data range
            tokenAsRegularExpression,                     //Token
            rpl           //Replacement value
        );
        if (debug) cout << "4. After regex_replace operation #" << i <<", result1 is " << result1.length() << \
            " bytes long and looks like:\n" << result1 << endl;
        if (debug) cout << "5. ===========================================================================================" << endl;
        inputTemplate.clear(); //Clear the inputTemplate so we don't append onto the back of the inputTemplate
        inputTemplate= result1; //Move regex_replace's output to the beginning of the just-cleared inputTemplate
    }  //End of for loop                                                                                      End of the for loop #1
    
    resultingStringLength = inputTemplate.length();
    resultingStringSize   = inputTemplate.size();
//    inputTemplate.insert(resultingStringLength, 3, '\0'); //Pop in three characters of value 0 (not character zero \
    which has a value of 0x30, what we want is a value of 0x00)
    std::strcpy (cStringBuffer, inputTemplate.c_str() ); //Before returning, convert our result, of type standard string, to \
    type c_string so C programs, which are oblivious to c++ type standard string and lack a c-string-terminating 0x'\0' character, \
    can use this data.
    kk = 0;
// Make sure there are no trailing characters
    cStringBuffer[resultingStringSize+0]='\0';
    cStringBuffer[resultingStringSize+1]='\0';
    cStringBuffer[resultingStringSize+2]='\0';
    *outsize = (int )resultingStringSize;
}  // End of doFabricateSQL

SS::~SS() {
    if (debugMode) {
        auto position = index_ptf.find("PTF1");
        if (position == index_ptf.end()) {
            cerr << "Failed to find PTF1 in dictionary named index_ptf" << endl;
        } else {
            rc = (int)position->second.index;
            cout <<  rc  << ". In class SS destructor, the Pointer to function (position->second.p1) is at location:\t"   << hex << fp << dec << endl;
            cout <<  position->second.index  << ". In class SS destructor, the Pointer to function (position->second.p1) is at location:\t"   << hex << fp << dec << endl;
            //       position->second.p1(1, &stringin1, &stringin2, this);
        }
        fp(rc, &stringin1, &stringin2, this);  //if in debug mode, call the function to which p1 points.
    }
    delete[] ptrPtrCopyOfReplacementsAsString;
    delete[] ptrPtrCopyOfTokensAsString;
    delete[] ptrPtrCstring_WorkArray;
    delete[] ptrPtrCopyOfTokens;        //Free dynamically acquired memory containing the tokens.
    delete[] ptrPtrCopyOfReplacements;  //Free dynamically acquired memory containing the replacements.
    
} //End of destructor

SS::SS(char *templatePointer, int bufsz, char **tokensReplacements, bool debug) {
    //Note: tokensReplacements is a 2-dimensional array where the first column, of each of many rows, contains the token and the \
    second column contains that token's replacement.
    cout << "Just entered the constructor for class named SS" << endl;
//    p1 = SS::ptfss; //p1 is a pointer to function
    
    i = j = kk = maxIndex = rc = rc1 = zero;
    caller_supplied_buffer_size = bufsz; //This is the size of the output buffer where we will construct the resulting c-string \
    with the template's tokens replaced with the values found in the replacements array.
//    number_of_tokens_passed_by_calling_program = n/2; //There is a one-to-one relationship between the tokens in array tokens and \
    in the replacements array.
    debugMode = debug;
    ptrPtrCopyOfReplacements = new char *[number_of_tokens_passed_by_calling_program];
    ptrPtrCopyOfTokens = new char *[number_of_tokens_passed_by_calling_program];
    ptrPtrCstring_WorkArray = new char *[number_of_tokens_passed_by_calling_program];
    ptrPtrCopyOfTokensAsString = new string *[number_of_tokens_passed_by_calling_program];
    ptrPtrCopyOfReplacementsAsString = new string *[number_of_tokens_passed_by_calling_program];
    for (i = 0; i < number_of_tokens_passed_by_calling_program; i++) {
        //Clear the workT and workR string objects so append member function will start at the beginning and not append onto previous
        workT.clear();
        workR.clear();
        //        ptrPtrCstrWorkT = &tokensReplacements[i][0];
        ptrPtrCstrWorkT = (tokensReplacements + i*2 +0);
        ptrPtrCopyOfTokens[i] = *ptrPtrCstrWorkT;
        //
        //        ptrPtrCstrWorkR = &tokensReplacements[i][1];
        ptrPtrCstrWorkR = (tokensReplacements + i*2 + 1);
        ptrPtrCopyOfReplacements[i] = *ptrPtrCstrWorkR;
        
        //       strcpy(ptrPtrCstrWorkR, tokensReplacements[i][1]);
        //       strcpy(ptrPtrCopyOfReplacements[i], ptrPtrCstrWorkR);
        workR.assign(*ptrPtrCstrWorkR);
        ptrPtrCopyOfReplacementsAsString[i] = &workR;
        
        workT.assign(*ptrPtrCstrWorkT);
        ptrPtrCopyOfTokensAsString[i] = &workT;
        if (debug) cout << "A." << i << " sr_or_ss: " << sr_or_ss << "; " << ";\tptrPtrCopyOfReplacements[" << i << "]: " \
            << ptrPtrCopyOfReplacementsAsString[i] << ";\t*ptrPtrCopyOfReplacements[" << i << "]: " \
            << *ptrPtrCopyOfReplacementsAsString[i] <<endl;
        
        if (debug) cout << "B." << i << " sr_or_ss: " << sr_or_ss << ";\tptrPtrCopyOfTokensAsString[" << i << "]:\t" \
            << ptrPtrCopyOfTokensAsString[i] << ";\n\tptrPtrCopyOfReplacements[" << i << "]: " \
            << ptrPtrCopyOfReplacementsAsString[i] << ";\t*ptrPtrCopyOfTokensAsString[" << i << "]:\t" \
            << *ptrPtrCopyOfTokensAsString[i] \
            << ";\n\t*ptrPtrCopyOfReplacements[" << i << "]: " << *ptrPtrCopyOfReplacementsAsString[i] <<endl;
        //        rc =  strcmp (*ptrPtrCstrWork, (const char *)"Term");
        //        rc1 = strcmp (*ptrPtrCstrWork, (const char *)"TERM");
        if ( ( *ptrPtrCopyOfTokensAsString[i] == stringTerm ) || ( *ptrPtrCopyOfTokensAsString[i] == stringTERM ) ) { // Test for \
            the presence of terminating Token/Replacement
            maxIndex = i;
            break;         //Exit from for-loop
        }
        rc = strcmp(sr_or_ss, ptrPtrCopyOfTokens[i] );  //We need to do a `#include <string.h>` for this "oldie-but-goodie" to work.
        if ( rc == 0 ) { //Is this the SRorSS Token? //This will work, too! One or the other.
            sross = *ptrPtrCopyOfReplacements[i];
            if (debug) cout << "sross looks like: " << sross << endl;
            
            work2.clear();
            auto position = dictionary.find( ptrPtrCopyOfReplacements[i] );
            if (position == dictionary.end() ) { //This check is used to determine if we failed to find anything in dictionary.
                cerr << "We failed to find the replacement character for " << ptrPtrCopyOfReplacements[i] << ". " \
                << ptrPtrCopyOfReplacements[i] << "; position->first: '" << position->first << "'; position->second: '"  \
                << position->second << "'.!!" << endl;
            } else {
                work2 = position->second; //work2 should be either a ">" or a "<"
                workCstring = (char *)work2.c_str();
                ptrPtrCstrWorkR = &workCstring;
                ptrPtrCopyOfReplacements[i] = *ptrPtrCstrWorkR;
                if (debug) cout << "*ptrPtrCopyOfReplacements[" << i << "]" << " looks like: " << *ptrPtrCopyOfReplacements[i] << endl;
            } // end of error-check if/else
        } //               End of If/Else
    } //                   End of outer for loop.
    
    if(templatePointer) { //Did caller supply a template pointer?
        lengthOfCallersTemplate = (int)std::strlen( templatePointer );  //Get length of the caller's input template
        if (debug) cout << "Note: the data type of templatePointer is: " << typeid(templatePointer).name() << \
            "\nWhereas the type of the default_template is: " << typeid(default_template).name() << \
            ".\nThe length of the data to which template pointer points is: " <<  lengthOfCallersTemplate << \
            " characters." <<  endl;
        // come here if caller to this dynamic libary (dylib) function has supplied a pointer to templates. \
        Assume [Required] that this template is a ZERO-terminate array of ASCII characters.
        char *ptr_workArrayTemplate = new char[ lengthOfCallersTemplate + (lengthOfCallersTemplate>>3) ]; //allocate a working buffer \
        sized 12½ % greater than the size of the caller's template
        memcpy(ptr_workArrayTemplate, templatePointer, lengthOfCallersTemplate);
        
        inputTemplate.clear(); //Let's start with a clean input Template.
        inputTemplate.append( ptr_workArrayTemplate) ; //Input Template is of data type string. You just can't simply assign a \
        c-string to a standard string string!!
        inputTemplateLength = (int)inputTemplate.length();
        //        templateSize = kk; //Does kk == inputTemplateLenght?? YES! For this example kk == inputTemplateLength == 1222 characters.
        delete[] ptr_workArrayTemplate;  //Must do delete[ ] because we did  `new char [ ... ]`
    } else { //Come here if caller did not supply a template, then we will use the default SQL template.
        if (debug) cout << "Note: the date type of templatePointer is: " << typeid(templatePointer).name() << endl;
        //        for (kk=zero; kk < templateSize; kk++) *(ptr_workArrayTemplate + kk) = inputTemplate[kk];
        inputTemplate = default_template;
        inputTemplateLength = (int)inputTemplate.length();
        //        templateSize = inputTemplate.size();
    }
    
    
} //End of SS constructor
void SS::ptfss(int theindex, const string *instring1, const string *instring2, SS *thisInstanceObject)     { //When we enter this \
    function there is no c++ `this` pointer so we don't know about the SS instance object. NB: static' can only be specified \
    inside the class definition
    char announce[10] = {'S', 'S', ':', ':', 'p', 't', 'f', 's', 's', '\0'};
    char *ptrAnnounce = announce;
    if (thisInstanceObject->debugMode) cout <<  ptrAnnounce << theindex << "\t" <<". instring looks like: "  << *instring1 \
        << ", outstring, which we will return to the caller, looks like: " << *instring2 << endl;
} //End of ptfss, "experimental", member function
