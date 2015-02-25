#include "include.h"

using namespace std;

Student::Student(Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id, unsigned int maxPurchases) : 
                                            printer(prt), nameserver(nameServer), office(cardOffice), id(id), 
                                            bottles_to_purchase(RandomInt.operator()(1, maxPurchases)),
                                            favourite_flavour(static_cast<VendingMachine::Flavours>(RandomInt.operator()(3))) { 
    
    printer.print(Printer::Student, id, 'S', favourite_flavour, bottles_to_purchase); // Print that the Student has started
    card = office.create(id, 5);                                    // Create a watcard with 5 dollars
    machine = nameserver.getMachine(id);                            // Get the next machine from the nameserver  
    printer.print(Printer::Student, id, 'V', machine->getId());     // Print that the student is visiting the machine
} 

Student::~Student() {
    delete card;
}

void Student::main() {
    int cost = 0;                                                   // Store the cost of buying from a vending machine
    VendingMachine::Status status;                                  // Store the status of each buy
    for (unsigned int i = 0; i < bottles_to_purchase; ++i) {        // Loop until all the required bottles have been bought
        yield(RandomInt.operator()(1, 10));                         // Yield randomly between 1 to 10
        for (;;) {                                                  // Loop until a successful buy has occurred
            try {                                                   // Attempt to buy/transfer/create
                status = machine->buy(favourite_flavour, *card());  // Record the status of a buy attempt at the current machine
                if (status == VendingMachine::STOCK) {              // If the machine returned STOCK
                    machine = nameserver.getMachine(id);            // Get a new machine from the server
                    printer.print(Printer::Student, id, 'V', machine->getId()); // Print that a new machine is being visited
                }                                           
                else if (status == VendingMachine::FUNDS) {         // If the machine returned FUNDS
                    cost = machine->cost();                         // Record the cost of a bottle
                    card = office.transfer(id, cost + 5, card());   // and transfer the cost plus 5 dollars to the card
                }
                else {                                              // Otherwise the buy was successful
                    printer.print(Printer::Student, id , 'B', card()->getBalance()); // Print that a buy occurred and the card balance
                    break;                                          // Break out of the infinite loop
                }
            }
            catch (WATCardOffice::Lost) {                           // If a card was lost, 
                printer.print(Printer::Student, id, 'L');           // print the status
                card = office.create(id, 5);                        // and attempt to create a new one
            }
        }
    }
    printer.print(Printer::Student, id, 'F');                       // Print that the student is finished executing
}
