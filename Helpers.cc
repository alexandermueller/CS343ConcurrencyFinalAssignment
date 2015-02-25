#include "include.h" 

using namespace std;

/**
 * usage
 * Returns the appropriate error message when supplied a code and
 * the inappropriate input. Accepts a cstring and an integer, where
 * the cstring is the incorrect input, and the integer is a code 
 * where 1 - 2 are specific errors, and all the rest are unexpected
 * arguments as the default error.
 **/

void usage(const char* cstring, int code) {
    switch (code) {
        case 1 :    // This is useless because of the error catching done in the given ConfigFile.cc
            cerr << "Error: file \'" << cstring << "\' failed to open, or does not exist." << endl; 
            break;
        case 2 :
            cerr << "Error: seed value \'" << cstring << "\' is incorrect. Expected value > 0." << endl;
            break;
        default : 
            cerr << "Error: unexpected arguments. Expected: \"soda [ config-file [ random-seed ] ]\"." << endl; 
            break;
    }
}

/**
 * isNumber
 * Checks if the string is a number. 
 * Returns true if true and false otherwise.
 **/
 
bool isNumber(string number) {
    for (unsigned int i = 0; i < number.length(); ++i){ // Loop through all the digits
        if(isalpha(number[i])) return false;            // If the digit is non-numerical 
    }                                                   // return false
    return true;                                        // Otherwise all the digits are numbers
}                                                                                               
