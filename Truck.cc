#include "include.h"

using namespace std;

Truck::Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour) : printer(prt), nameserver(nameServer), 
                                                                                                                                             plant(plant), 
                                                                                                                                             num_machines(numVendingMachines),
                                                                                                                                             max_stock(maxStockPerFlavour) {
    printer.print(Printer::Truck, 'S');                       // Print that the truck has started executing
    num_flavours = VendingMachine::NUMFLAVOURS;               // Pull the amount of flavours from the VendingMachine task
    cargo = new unsigned int[num_flavours];                   // Create
    for (int i = 0; i < num_flavours; ++i) cargo[i] = 0;      // and initialize the cargo such that each flavour currently carried is 
    machines = nameserver.getMachineList();                   // Get the machines list from the name server
}

Truck::~Truck() {
    delete [] cargo;
}

/**
 * Truck:main
 * The truck delivers bottles from the plant to each vending machine. It refills its 
 * cargo whenever it runs out of bottles by getting the next shipment from the bottle
 * plant. The truck finishes executing when the bottling plant stops production.
 **/

void Truck::main() {
    unsigned int *inventory;                      // A pointer used to access the inventory of each vending machine
    unsigned int id = 0;                          // The id of the currently accessed vending machine
    unsigned int still_need = 0;                  // How many bottles the vending machine still needs after stocking it
    unsigned int bottles_left = 0;                // How many bottles are left in cargo at any time during the shipping process
    unsigned int bottles_given = 0;               // How many bottles of a flavour are given to the vending machine
    unsigned int bottles_needed = 0;              // How many bottles are needed to fill the current flavour in the machine completely
    for(;;) {
        yield(RandomInt.operator()(1, 10));                                              // Go to timmies for a quick pitstop!
        for (int i = 0; i < num_flavours; ++i) cargo[i] = 0;                             // Throw out any old bottles left over 
        if (plant.getShipment(cargo)) break;                                             // Stop shipping if the plant has closed down
        for (int i = bottles_left = 0; i < num_flavours; ++i) bottles_left += cargo[i];  // Count up all the bottles left in the cargo
        printer.print(Printer::Truck, 'P', bottles_left);        // Print out the amount of bottles given to the truck 
        for (unsigned int i = 0; i < num_machines; ++i) {        // Visit all the vending machines on the list
            printer.print(Printer::Truck, 'd', id, bottles_left);// Print that the delivery to the current vending machine has begun
            id = machines[i]->getId();                           // Get the id of the current machine
            inventory = machines[i]->inventory();                // Get the inventory of the current machine
            for (int j = 0; j < num_flavours; ++j) {             // Stock the current machine with bottles available in the cargo
                if (inventory[j] < max_stock) {                  // Stock the current flavour of this machine if it isn't already full
                    bottles_needed = max_stock - inventory[j];   // Calculate how many bottles for the current flavour are needed
                                                                 // Calculate how many bottles of the current flavour can be given to 
                    bottles_given = cargo[j] > bottles_needed ? bottles_needed : cargo[j]; // the current machine
                    cargo[j] -= bottles_given;                   // Take that many bottles of the current flavour out of the cargo
                    bottles_left -= bottles_given;               // Record how many bottles are left
                    inventory[j] += bottles_given;               // Place those bottles into the vending machine
                                                                 // Update the value of the bottles that the machine still needs but
                    still_need += bottles_given - bottles_needed;// couldn't be given
                }      
            }
            if (still_need) {                                       // If the vending machine still needs bottles to achieve a full  
                printer.print(Printer::Truck, 'U', id, still_need); // stock, print out the amount needed
                still_need = 0;                                     // and reset the count for the next vending machine
            }                                                       // Print that the delivery to the current machine is finished and 
            printer.print(Printer::Truck, 'D', id, bottles_left);   // the amount of cargo left
            machines[i]->restocked();                               // Tell the machine that the truck has finished restocking it
           
            if (!bottles_left) break;          // Stop delivering if the truck ran out of bottles, so that it can restock at the plant
        }
    }
    printer.print(Printer::Truck, 'F');        // Print that the truck is finished executing
}
