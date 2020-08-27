// ------------------------------------------------------+
//  fabricateSQLh.cpp                                    |
//  fabricateSQL                                         |
//                                                       |
//  Created by Clifford Campo on 8/8/20.                 |
//  Copyright © 2020 CliffordCampo. All rights reserved. |
//                                                       |
// ------------------------------------------------------+
#include "fabricateSQLh.hpp"

extern "C" void fabricateSQLr2(int, char **, char *, char *, int, bool=false);
//
//void fabricateSQL(int, char *, char *, char *, char *, int);
// If true then in debug mode. Default is false ------------------------------------------------------------------+
// size of the SQL template-containing cStringBuffer. If bufsize = 0 then use the default template --+            |
// pointer to fabricateSQL's result return buffer   -----------------------------------+             |            |
// Original SQL Template (TBD) ---------------------------+                            |             |            |
//                                                        |                            |             |            |
// token/replacement array ----------+                    |                            |             |            |
// # of Tokens -------+              |                    |                            |             |            |
//                    |              |                    |                            |             |            |
//                    |              |                    |                            |             |            |
//                    |              |                    |                            |             |            |
//                    V              V                    V                            V             V            V
void fabricateSQLr2(int asz, char **theTokensReplacements,  char *oSQLt, char *cStringBuffer, int bufsize, bool debug) {
    //Note: fabricateSQL serves as a between the caller and doFabricateSQL, which performs the work of transforming \
    the caller's SQL template into executable SQL.
    ;
    if (debug) { 
        cout << "0. =================================== entering fabricateSQL =============================================" \
        << "\n1. input parameters look like:\nasz: " \
        << asz \
        << ":\ntheTokensReplacements " \
        << theTokensReplacements \
        << "\noSQLt: " \
        << oSQLt
        << "\t*oSQLt: "
        << *oSQLt \
        << "\ncStringBuffer: " \
        << cStringBuffer \
        << "\t*cStringBuffer: " \
        << *cStringBuffer \
        << "\nbufsize: " \
        << bufsize \
        << "\ndebug: " \
        << debug \
        << endl;
    }
/*
 std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
 auto duration = now.time_since_epoch();
 auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
 

    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto microsec = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
*/
    if (debug) {
        auto start = chrono::steady_clock::now();
        auto tpconstructorstart = std::chrono::steady_clock::now();
        SS *ptrSS = new  SS(asz, oSQLt, bufsize, theTokensReplacements, debug);
        auto tpconstructorend = std::chrono::steady_clock::now();
    
        auto dfs = std::chrono::steady_clock::now();
        ptrSS->doFabricateSQL(asz, theTokensReplacements, oSQLt, cStringBuffer, bufsize, debug);
        auto dfe= chrono::steady_clock::now();
    
        auto do_delete_start = std::chrono::steady_clock::now();
        delete ptrSS;   //Delete the instance object , ptrSS, we created before calling doFabricateSQL
        auto tpdodeleteend = std::chrono::steady_clock::now();
    
        auto end = chrono::steady_clock::now();
        clog << "a) std::chrono::nanoseconds::period::num " << std::chrono::nanoseconds::period::num \
            << "\nb) std::chrono::nanoseconds::period::den: " << std::chrono::nanoseconds::period::den  << "\n=====================\n" \
            << std::chrono::duration_cast<std::chrono::nanoseconds>(tpconstructorend - tpconstructorstart).count() << "\nc) It took " \
            << " n-sec to do constructor processing;\nd) It took " \
            << std::chrono::duration_cast<std::chrono::nanoseconds>(dfe - dfs).count() \
            << " n-sec to do doFabricateSQL processing;\ne) It took " \
            << std::chrono::duration_cast<std::chrono::nanoseconds>(tpdodeleteend - do_delete_start).count()  \
            << " n-sec to do delete processing.\n" \
            << "f) Total execution time " \
            << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() \
            << " n-sec." << endl;
    } else {
        SS *ptrSS = new  SS(asz, oSQLt, bufsize, theTokensReplacements, debug);
        ptrSS->doFabricateSQL(asz, theTokensReplacements, oSQLt, cStringBuffer, bufsize, debug);
        delete ptrSS;   //Delete the instance object , ptrSS, we created before calling doFabricateSQL
    }
} //Return to caller.

void SS::doFabricateSQL(int asz, char **theTokensReplacements, char *SQLt, char *cStringBuffer, int bufsize, bool debug) {

    for (i = 0; i < number_of_tokens_passed_by_calling_program; i++) { //Beginning of the for loop #2 
        result1.clear(); //Now clear the output result1 so we don't append onto the previous output. 
        work.clear(); 
        cStringLength = (int)strlen(ptrPtrCopyOfTokens[i]);
        work.assign(ptrPtrCopyOfTokens[i], cStringLength );
        if (debug ) cout << "2. theTokensReplacements[i][0]: " << theTokensReplacements[i][1] << "\twork: " << work << endl;
        tokenAsRegularExpression.assign( work );    

        rpl.clear(); 
        cStringLength = (int)strlen(ptrPtrCopyOfReplacements[i]);
        rpl.append(ptrPtrCopyOfReplacements[i], cStringLength); 
        if (debug ) cout << "3. theTokensReplacements[i][1]: " << theTokensReplacements[i][1] << "\trpl: " << rpl << endl;
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
    }  //                                                                                                    End of the for loop #2
    
    resultingStringLength = inputTemplate.length();
    resultingStringSize   = inputTemplate.size();
    inputTemplate.insert(resultingStringLength, 3, '\0'); //Pop in three characters of value 0 (not character zero \
    which has a value of 0x30, what we want is a value of 0x00)
    std::strcpy (cStringBuffer, inputTemplate.c_str() ); //Before returning, convert our result, of type standard string, to \
    type c_string so C programs, which are oblivious to c++ type standard string and lack a c-string-terminating 0x'\0' character, \
    can use this data.
    kk = 0;
    while ( kk <=  resultingStringSize ) {
        if ( cStringBuffer[kk] != ';') { //cStringBuffer is supplied by the caller. It is the caller's responsibility to make sure \
            cStringBuffer is sufficiently sized to hole the resulting stirng found in
            kk++;
            continue;
        } else {
            cStringBuffer[kk+1] = '\0';
            cStringBuffer[kk+2] = '\0';
            cStringBuffer[kk+3] = '\0';
            break;
        }
    }
}  // End of doFabricateSQL
   
SS::~SS() {
    delete ptrPtrCopyOfReplacementsAsString;
    delete ptrPtrCopyOfTokensAsString;
    delete ptrPtrCstring_WorkArray;
    delete ptrPtrCopyOfTokens;        //Free dynamically acquired memory containing the replacements
    delete ptrPtrCopyOfReplacements;  //Free dynamically acquired memory containing the replacements
    
} //End of destructor

SS::SS(int n, char *templatePointer, int bufsz, char **tokensReplacements, bool debug) {
    i = j = kk = zero; 
    caller_supplied_buffer_size = bufsz; //This is the size of the output buffer where we will construct the resulting c-string \
    with the template's tokens replaced with the values found in the replacements array.
    number_of_tokens_passed_by_calling_program = n; //There is a one-to-one relationship between the tokens in array tokens and \
    in the replacements array.
    debugMode = debug;
    ptrPtrCopyOfReplacements = new char *[number_of_tokens_passed_by_calling_program];
    ptrPtrCopyOfTokens = new char *[number_of_tokens_passed_by_calling_program];
    ptrPtrCstring_WorkArray = new char *[number_of_tokens_passed_by_calling_program];
    ptrPtrCopyOfTokensAsString = new string *[number_of_tokens_passed_by_calling_program];
    ptrPtrCopyOfReplacementsAsString = new string *[number_of_tokens_passed_by_calling_program];
    for (i = 0; i < number_of_tokens_passed_by_calling_program; i++) {
        
        ptrPtrCopyOfTokens[i] = &tokensReplacements[i][tokenColumn];
        ptrPtrCopyOfReplacements[i] = &tokensReplacements[i][replacementColumn];
        
        if (debug) cout << "A. sr_or_ss: " << sr_or_ss << "; " << ";\tptrPtrCopyOfReplacements[" << i << "]: " \
            << ptrPtrCopyOfReplacements[i] << endl;

        if (debug) cout << "B. sr_or_ss: " << sr_or_ss << ";\tptrPtrCopyOfTokens[" << i << "]:\t" << ptrPtrCopyOfTokens[i] << \
        ";\n\tptrPtrCopyOfReplacements[" << i << "]: " << ptrPtrCopyOfReplacements[i] << endl;
        
        rc = strcmp(sr_or_ss, ptrPtrCopyOfTokens[i] );  //Need to do a #include <string.h> for this oldie-but-goodie to work.
        if ( rc == 0 ) { //Is this the SRorSS Token? //This will work, too! One or the other.
            sross = string(ptrPtrCopyOfReplacements[i]);
            if (debug) cout << "sross looks like: " << sross << endl;
            
            work2.clear();
            auto position = dictionary.find( string(ptrPtrCopyOfReplacements[i]) );
            if (position == dictionary.end() ) { //This check is used to determine if we failed to find anything in dictionary.
                cerr << "We failed to find the replacement character for " << ptrPtrCopyOfReplacements[i] << ". " \
                << ptrPtrCopyOfReplacements[i] << "; position->first: '" << position->first << "'; position->second: '"  \
                << position->second << "'.!!" << endl;
            } else {
                work2 = position->second; //work2 should be either a ">" or a "<"
                std::strcpy(ptrPtrCstring_WorkArray[i], &work2.c_str()[0] );

                std::strcpy(ptrPtrCopyOfReplacements[i], ptrPtrCstring_WorkArray[i]);
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
    
    
} //End of constructor
string SS::ptfss(string instring, string outstring) { 
    ;
    return (outstring);
}