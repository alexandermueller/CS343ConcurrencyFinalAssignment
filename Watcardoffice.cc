#include "include.h"

using namespace std;


WATCardOffice::Job::Job(Args args) : args(args) {}

WATCardOffice::Courier::Courier(unsigned int id, Printer &printer, Bank &bank, WATCardOffice &office) : id(id), printer(printer), bank(bank), office(office) {
    printer.print(Printer::Courier, id, 'S');                                                                   // Print that this courier has started running
}

/**
 * Courier::main
 * Each courier loops until their destructor is called or they are woken up and there
 * are no more jobs remaining, meaning the office is closing. They request for jobs and 
 * then do any that are returned, but there's a 1/6 chance that they will lose the watcard. If
 * it is lost, they insert an event into the future watcard, otherwise they put the new
 * watcard pointer into the future watcard.
 **/

void WATCardOffice::Courier::main() {
    for(;;) {
    _Accept(~Courier) { break; }                    // Accept calls to the destructor so that it can get out of the loop
        _Else {
            Job *job = office.requestWork();        // Request a new job               
            if (!job) break;                        // If the job is NULL, break out of the loop and finish executing
            WATCard *card = job->args.card;         // Retrieve the card from the job arguments
            if (job->args.type == Args::Create) {   // If the job is of type Create, then create a new watcard
                assert(!job->args.card);            // Make sure the card has nothing in it already
                card = new WATCard();
            } 
            printer.print(Printer::Courier, id, 't', job->args.id, job->args.amount);   // Print that the transfer has started
            bank.withdraw(id, job->args.amount);    // Withdraw the necessary funds from the bank
            card->deposit(job->args.amount);        // and then deposit that amount into the watcard
            if (!RandomInt.operator()(5)) {         // If the random number between [0,5] comes out as 0, then lose the
                job->result.exception(new Lost);    // watcard, and insert an exception into the future
                delete card;                        // Destroy the watcard
            }   
            else job->result.delivery(card);        // Otherwise the transfer was a success, so deliver the card
            printer.print(Printer::Courier, id, 'T', job->args.id, job->args.amount);   // Print out that the transfer was completed
            delete job;                                                                                         
        }    
        
    }
    printer.print(Printer::Courier, id, 'F');       // Print out that this Courier is now finished executing
}

WATCardOffice::WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers) : printer(prt), bank(bank), 
                                                                                   numCouriers(numCouriers) {
    printer.print(Printer::WATCardOffice, 'S');     // Print out that the watcardoffice is now executing
    couriers = new Courier*[numCouriers];           // Create a pool of new couriers of size numCouriers          
    for (unsigned int i = 0; i < numCouriers; ++i) couriers[i] = new Courier(i, printer, bank, *this);  
    s_id = s_amt = 0;                               // Initialize the s_id and s_amt
}

WATCardOffice::~WATCardOffice() {
    for (unsigned int i = 0; i < numCouriers; ++i) delete couriers[i];  // Delete the Couriers and
    delete [] couriers;                                                 // courier pool
}

/**
 * WATCardOffice::create
 * Student calls this to request a watcard to be created and the desired amount
 * transferred onto it. Returns a future watcard.
 **/

WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {                                 
s_id = sid;                                         // Save the id of the student 
    s_amt = amount;                                 // and the requested amount
    Args args(Args::Create, sid, amount, NULL);     // Initialize the arguments for the job
    Job *job = new Job(args);                       // Create a new job
    jobs.push_back(job);                            // and add it to the job queue
    return job->result;                             // Return the future watcard
}

/**
 * WATCardOffice::transfer
 * Student calls this to request a transfer of a desired amount to
 * their watcard. Returns a future watcard.
 **/

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard *card) {
    s_id = sid;                                     // Save the id of the student
    s_amt = amount;                                 // and the requested amount to be transferred
    Args args(Args::Transfer, sid, amount, card);   // Initialize the arguments for the job
    Job *job = new Job(args);                       // Create a new job
    jobs.push_back(job);                            // and add it to the job queue
    return job->result;                             // Return the future watcard
}

/**
 * WATCardOffice::requestWork
 * Couriers call this to get new jobs. This returns a job from the job
 * queue if one exists, otherwise it returns NULL, signalling the couriers
 * that they must finish executing.
 **/

WATCardOffice::Job* WATCardOffice::requestWork() {
    Job* job = !jobs.size() ? NULL : jobs.back();   // If no jobs exist, make job NULL, otherwise hand it a job
    if(job) jobs.pop_back();                        // Pop the job from the queue if it exists
    return job;                                                                                                                                           
}

/**
 * WATCardOffice::main
 * The office controls the flow of requests for jobs depending on their availability,
 * as well as announces when a create, transfer, or job request has been made. Stops 
 * looping when the destructor is called.
 **/

void WATCardOffice::main() {
    for(;;) {
        _Accept(~WATCardOffice) {                   // Break out of the loop if the destructor is called 
            for(unsigned int i = 0; i < numCouriers; ++i) _Accept(requestWork); // Alllow the couriers waiting 
            break;                                  // for jobs to pass through the rourine
        }                                           // Only accepts job requests when there are jobs to be handed out
        or _When(jobs.size()) _Accept(requestWork) { printer.print(Printer::WATCardOffice, 'W'); }              
        or _Accept(create) { printer.print(Printer::WATCardOffice, 'C', s_id, s_amt); }     // Otherwise accept calls to create 
        or _Accept(transfer) { printer.print(Printer::WATCardOffice, 'T', s_id, s_amt); }   // and transfer 
    }
    printer.print(Printer::WATCardOffice, 'F');     // Print that the office is now finished executing
}
