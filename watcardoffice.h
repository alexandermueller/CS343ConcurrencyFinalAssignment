#ifndef __WATCARDOFFICE_H__
#define __WATCARDOFFICE_H__

#include "args.h"
#include "bank.h"
#include "watcard.h"
#include "printer.h"
#include "uFuture.h"

_Task WATCardOffice {
    struct Job {                      // A request made by a student for either transferring to watcard or creating a new one
        Args args;                    // Marshalled arguments
        WATCard::FWATCard result;     // return future
        Job(Args args);                         
    };
    std::vector<Job*> jobs;           // A queue of the jobs still needing to be done
    _Task Courier {                   // Worker task that handles job requests for the office
        Bank &bank;                                                                     
        unsigned int id;              // Courier id
        Printer &printer;   
        WATCardOffice &office;        // Reference to the original office that created it
        void main();    
      public:
        Courier(unsigned int id, Printer &printer, Bank &bank, WATCardOffice &office); 
    };             
    Bank &bank;
    Printer &printer;
    unsigned int s_id;                // Id of the student that made the request (used for printing purposes)
    unsigned int s_amt;               // Amount that the student wanted to transfer onto their watcard (used for printing purposes)
    Courier **couriers;               // Pool of couriers created at the creation of the office
    unsigned int numCouriers;         // Amount of couriers requested to be running
    void main();
  public:
    _Event Lost {};                   // uC++ exception type, like "struct". Is inserted into future watcards when lost
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );                
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );          // Students call this to request a watcard creation
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card ); // Students call this to request a transfer
    Job *requestWork();                                                         // Couriers call this to get new jobs
};

#endif
