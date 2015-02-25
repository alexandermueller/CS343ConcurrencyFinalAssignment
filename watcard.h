#ifndef __WATCARD_H__
#define __WATCARD_H__

#include "uFuture.h"

class WATCard {
    unsigned int balance;                   // Balance on the watcard
    WATCard( const WATCard & );             // Prevents copying
    WATCard &operator=( const WATCard & ); 
  public:
    WATCard();
    typedef Future_ISM<WATCard *> FWATCard; // Future watcard pointer
    void deposit( unsigned int amount );
    void withdraw( unsigned int amount );
    unsigned int getBalance();
};

#endif
