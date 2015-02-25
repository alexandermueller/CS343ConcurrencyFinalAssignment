#ifndef __VENDINGMACHINE_H__
#define __VENDINGMACHINE_H__

#include "printer.h"
#include "watcard.h"
#include "nameserver.h"

_Task NameServer;

_Task VendingMachine {
    unsigned int id;                // This vending machine's id
    Printer &printer;                                                           
    unsigned int *stock;            // Array of stock levels for each flavour
    unsigned int s_cost;            // The cost of each soda in the machine
    NameServer &nameserver;                                         
    unsigned int max_stock;         // The maximum stock level of each soda
    void main();
  public:
    enum Flavours { CRAB = 0, OCTOPUS, EEL, SALMON, NUMFLAVOURS };// Flavours of soda, where CRAB = 0...NUMFLAVOURS = 4 
    enum Status { BUY, STOCK, FUNDS };                            // Purchase status: successful buy, out of stock, insufficient funds
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, 
                    unsigned int sodaCost, unsigned int maxStockPerFlavour );
    ~VendingMachine();
    Status buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
  private:
    Status status;                  // Holds the status of the last buy for the server to process later
    Flavours flavour_chosen;        // Holds the flavour chosen in the last buy for the server to process later 
};

#endif
