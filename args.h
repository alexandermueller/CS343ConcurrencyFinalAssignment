#ifndef __ARGS_H__
#define __ARGS_H__

#include "watcard.h"

struct Args {
    enum Type { Create, Transfer };                                         // The types of jobs that can be done
    Type type;                                                              // Tells which type of job this is
    WATCard *card;                                                          // Placeholder for the student's card 
    unsigned int id;                                                        // Id of the requesting student           
    unsigned int amount;                                                    // The amount needed on the card
    Args(Type type, unsigned int id, unsigned int amount, WATCard *card);
};

#endif
