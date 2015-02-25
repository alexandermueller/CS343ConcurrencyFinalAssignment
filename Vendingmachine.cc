#include "include.h"

using namespace std;

VendingMachine::VendingMachine(Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, 
                               unsigned int maxStockPerFlavour) : printer(prt), nameserver(nameServer), id(id), s_cost(sodaCost), 
                                                                  max_stock(maxStockPerFlavour) {
    printer.print(Printer::Vending, id, 'S', s_cost);               // Print that the vending machine has started executing
    stock = new unsigned int[NUMFLAVOURS];                          // Create an array of size NUMFLAVOURS
    for (int i = 0; i < NUMFLAVOURS; ++i) stock[i] = 0;             // Initialize each flavour to 0
    nameserver.VMregister(this);                                    // Register the machine
} 

VendingMachine::~VendingMachine() {
    delete [] stock;
}

/** 
 * VendingMachine::buy
 * Students call this routine, where it returns 
 * STOCK if there are no more of the particular flavour,
 * FUNDS if there isn't enough money left on the watcard,
 * and BUY if otherwise, and withdraws the soda cost 
 * from the watcard.
 **/

VendingMachine::Status VendingMachine::buy(Flavours flavour, WATCard &card) {                       
    if (!stock[flavour]) return status = STOCK;                     // If the current flavour is empty, return STOCK
    else if (card.getBalance() < s_cost) return status = FUNDS;     // Otherwise if card can't pay the cost of one soda, return FUNDS 
    flavour_chosen = flavour;                                       // Otherwise the flavour chosen is recorded for the server,
    card.withdraw(s_cost);                                          // the appropriate amount is debited from the card,
    return status = BUY;                                            // and BUY is returned
}

/** 
 * VendingMachine::inventory
 * The truck calls this routine, where it returns 
 * a pointer to the stock for it to manipulate
 * directly.
 **/

unsigned int* VendingMachine::inventory() {
    return stock;   
}

/** 
 * VendingMachine::restocked
 * The truck calls this routine, where it only
 * has use as a signal to the vending machine
 * to allow buys to occur again.
 **/

void VendingMachine::restocked() {}

/** 
 * VendingMachine::cost
 * Returns the cost to buy sodas from this machine.
 **/

_Nomutex unsigned int VendingMachine::cost() {
    return s_cost;
}

/** 
 * VendingMachine::cost
 * Returns this machine's id.
 **/

_Nomutex unsigned int VendingMachine::getId() {
    return id;
}

/**
 * VendingMachine::main
 * The vending machine administrates requests from the students and truck for 
 * access to this machine. If the truck is restocking the machine, nothing else
 * can be called until the restock routine is called. Otherwise buys are allowed
 * and the machine automatically decreases the stock. Once the destructor is called, 
 * the vending machine finishes executing.
 **/

void VendingMachine::main() {
    for (;;) {  
        _Accept(~VendingMachine) { break; }                         // Finish executing if the destructor is called
        or _Accept(inventory) {                                     // Otherwise if inventory is called,
            printer.print(Printer::Vending, id, 'r');               // print that the truck is restocking,
            _Accept(restocked);                                     // and wait until one call to restocked occurs
            printer.print(Printer::Vending, id, 'R');               // Print that the truck is finished restocking
        }                             
        or _Accept(buy) { 
            if (status == BUY) {                                    // If the status of the last buy call was BUY,
                                                                    // print that a flavour has been bought and how much is left
                printer.print(Printer::Vending, id, 'B', flavour_chosen, stock[flavour_chosen]);  
                --stock[flavour_chosen];                            // Decrement the chosen flavour's stock accordingly
            }
        }
    }
    printer.print(Printer::Vending, id, 'F');                       // Print that the vending machine is finished executing
}
