// ------------------------------------------------------+
//  fabricateSQLh.cpp                                    |
//  fabricateSQL                                         |
//                                                       |
//  Created by Clifford Campo on 8/8/20.                 |
//  Copyright © 2020 CliffordCampo. All rights reserved. |
//                                                       |
// ------------------------------------------------------+
#include "fabricateSQLh.hpp"

extern "C" void fabricateSQL(int, char **, char **, char *, char *, int, bool=false);
//
//void fabricateSQL(int, char *, char *, char *, char *, int);
// If true then in debug mode. Default is false -----------------------------------------------------------------------+
// size of the SQL template-containing cStringBuffer. If bufsize = 0 then use the default template -------+            |
// pointer to fabricateSQL's result return buffer   -------------------------------------+                |            |
// Original SQL Template (TBD) -----------------------------------------+                |                |            |
// token replacement array -------------------------------+             |                |                |            |
// token array ----------------------+                    |             |                |                |            |
// # of Tokens -------+              |                    |             |                |                |            |
//                    |              |                    |             |                |                |            |
//                    |              |                    |             |                |                |            |
//                    |              |                    |             |                |                |            |
//                    V              V                    V             V                V                V            V
void fabricateSQL(int asz, char **theTokens, char **replacements, char *oSQLt, char *cStringBuffer, int bufsize, bool debug) {
    //Note: fabricateSQL serves as a between the caller and doFabricateSQL, which performs the work of transforming \
    the caller's SQL template into executable SQL.
    ;
    if (debug) { 
        cout << "0. =================================== fabricateSQL =============================================" << \
        "\n1. input parameters look like:\nasz: " << asz << ":\ntheTokens " << theTokens << "\t*theTokens " << *theTokens << \
        "\nreplacements: " << replacements << "\t*replacements: " << *replacements << "\noSQLt: " << oSQLt << "\t*oSQLt: " << \
        *oSQLt << "\ncStringBuffer: " << cStringBuffer << "\t*cStringBuffer: " << *cStringBuffer << "\nbufsize: " << bufsize << \
        "\ndebug: " << debug << endl;
    }
    
    
    SS *ptrSS = new  SS(asz, oSQLt, bufsize, theTokens, replacements, debug);
    ptrSS->doFabricateSQL(asz, theTokens, replacements, oSQLt, cStringBuffer, bufsize, debug);
    delete ptrSS;   //Delete the instance object , ptrSS, we created before calling doFabricateSQL
} //Return to caller.

void SS::doFabricateSQL(int asz, char **theTokens, char **replacements, char *SQLt, char *cStringBuffer, int bufsize, bool debug) {

    for (i = 0; i < number_of_tokens_passed_by_calling_program; i++) { //Beginning of the for loop #2 
        result1.clear(); //Now clear the output result1 so we don't append onto the previous output. 
        work.clear(); 
        cStringLength = (int)strlen(ptrcopyOfTokens[i]); 
        work.assign(ptrcopyOfTokens[i], cStringLength );
        if (debug ) cout << "2. theTokens[i]: " << theTokens[i] << "\twork: " << work << endl;
        tokenAsRegularExpression.assign( work );    

        rpl.clear(); 
        cStringLength = (int)strlen(ptrcopyOfReplacements[i]);
        rpl.append(ptrcopyOfReplacements[i], cStringLength); 
        if (debug ) cout << "3. replacements[i]: " << replacements[i] << "\trpl: " << rpl << endl;
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
    ;  //Destructor seems to be called when the fabricateSQL function executes the delete instruction for deleting the instance \
    object
 //Delete dynamically allocated storage for holding a copy of the caller's template.
} //End of destructor

SS::SS(int n, char *templatePointer, int bufsz, char **tokens, char **replacements, bool debug) {
    i = j = kk = zero; 
    caller_supplied_buffer_size = bufsz; //This is the size of the output buffer where we will construct the resulting c-string \
    with the template's tokens replaced with the values found in the replacements array.
    number_of_tokens_passed_by_calling_program = n; //There is a one-to-one relationship between the tokens in array tokens and \
    in the replacements array.
    debugMode = debug; 

    for (i = 0; i < n; i++) {
        ptrptrcopyOfReplacements[i] = &replacements[i];
        ptrcopyOfReplacements[i] = replacements[i];
        
        if (debug) cout << "A. sr_or_ss: " << sr_or_ss << "; " << ";\tptrcopyOfReplacements[" << i << "]: " \
            << ptrcopyOfReplacements[i] << endl;
        
        ptrptrcopyOfTokens[i] = &tokens[i];
        ptrcopyOfTokens[i] = tokens[i];

        if (debug) cout << "B. sr_or_ss: " << sr_or_ss << ";\tptrcopyOfTokens[" << i << "]:\t" << ptrcopyOfTokens[i] << \
        ";\n\tptrcopyOfReplacements[" << i << "]: " << ptrcopyOfReplacements[i] << endl;
        
        rc = strcmp(sr_or_ss, ptrcopyOfTokens[i] );  //Need to do a #include <string.h> for this oldie-but-goodie to work.
        if ( rc == 0 ) { //Is this the SRorSS Token? //This will work, too! One or the other.
            sross = string(ptrcopyOfReplacements[i]); 
            if (debug) cout << "sross looks like: " << sross << endl;
            
            work2.clear();
            auto position = dictionary.find( string(ptrcopyOfReplacements[i]) );
            work2 = position->second; //work2 should be either a ">" or a "<"

            std::strcpy(cstring_workArray, &work2.c_str()[0] ); 

            std::strcpy(ptrcopyOfReplacements[i], cstring_workArray);
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
        delete[] ptr_workArrayTemplate;
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
