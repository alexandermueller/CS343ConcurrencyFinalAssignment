#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include <map>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <uSemaphore.h>
#include <uFuture.h>

// Local include files

#include "args.h"
#include "bank.h"
#include "MPRNG.h"
#include "truck.h"
#include "config.h" 
#include "parent.h"
#include "printer.h"
#include "student.h"
#include "watcard.h"
#include "nameserver.h"
#include "bottlingplant.h"
#include "watcardoffice.h"
#include "vendingmachine.h"
     
extern PRNG RandomInt;           
                                            // Error structure that saves
struct Error {                              // the context of the error
    int usage;                              // for the usage function to 
    Error(int usage) : usage(usage) {}
};  

bool isNumber(std::string number);          // Checks if string contains ints
void usage(const char* cstring, int code);  // Reports errors

#endif
