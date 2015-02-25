#ifndef __STUDENT_H__
#define __STUDENT_H__

#include "printer.h"
#include "watcard.h"
#include "nameserver.h"
#include "watcardoffice.h"
#include "vendingmachine.h"

_Task Student {
    unsigned int id;                            // Student id
    Printer &printer;           
    WATCardOffice &office;
    NameServer &nameserver;
    WATCard::FWATCard card;
    VendingMachine *machine;     
    unsigned int bottles_to_purchase;           // Amount of bottles the student will purchase until it terminates
    VendingMachine::Flavours favourite_flavour; // Random flavour that the student will primarily purchase
    void main();
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id, unsigned int maxPurchases );
    ~Student();
};

#endif
