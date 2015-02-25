#include "include.h"

using namespace std;

BottlingPlant::BottlingPlant(Printer &prt, NameServer &nameServer, unsigned int numVendingMachines, 
                             unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments) : 
                             printer(prt), nameserver(nameServer), num_machines(numVendingMachines), 
                             max_shipped(maxShippedPerFlavour), max_stock(maxStockPerFlavour), time(timeBetweenShipments) {
   
    printer.print(Printer::BottlingPlant, 'S');                             // Print that the bottling plant is starting to execute
    num_flavours = VendingMachine::NUMFLAVOURS;                             // Pull the number of flavours from the VendingMachine task
    production = new unsigned int[num_flavours];                            // Create
    for (int i = 0; i < num_flavours; ++i) production[i] = 0;               // and instantiate the amount of bottles produced to 0
    truck = new Truck(printer, nameserver, *this, num_machines, max_stock); // Create the truck
}

BottlingPlant::~BottlingPlant() {
    delete truck;
}

/**
 * BottlingPlant::getShipment
 * Loads all the bottles made in the current production run into the truck's cargo.
 * Returns true if the bottling plant is closed, otherwise it returns false after
 * loading up the truck.
 **/

bool BottlingPlant::getShipment(unsigned int cargo[]) {
    if (production) {                                                       // If the plant hasn't closed yet,
        for (int i = 0; i < num_flavours; ++i) cargo[i] += production[i];   // load up the produced bottles into cargo
        return false;                                                       // and return false. (This seems very counter-intuitive!)
    }   
    return true;                                                            // Otherwise, production has stopped so return true.
}

/**
 * BottlingPlant::main
 * The bottling plant makes a production run and waits for the truck to pick up the produced bottles.
 * This repeats until the destructor is called, in which the bottling plant closes down the production
 * and tells the truck it's closed.
 **/

void BottlingPlant::main() {
    int produced = 0;                                           // Amount of bottles produced each production run
    for (;;) {                                                  // Produce a random amount [0, max_shipped] of bottles per flavour 
        for (int i = 0; i < num_flavours; ++i) produced += production[i] = RandomInt.operator()(max_shipped);   
        printer.print(Printer::BottlingPlant, 'G', produced);   // Print the amount of bottles generated in the current production run
        produced = 0;                                           // Reset the count of bottles produced
        _Accept(~BottlingPlant) {                               // Continue running if the destructor is called...
            delete [] production;                               // and delete the production,
            production = NULL;                                  // set it to NULL for the truck to know production has stopped,
            _Accept(getShipment);                               // wait for the truck to accept the next shipment,
            break;                                              // and break out of the loop
        }
        or _Accept(getShipment);                                // ... OR continue running if the truck is here to get the shipment 
        printer.print(Printer::BottlingPlant, 'P');             // Print that the shipment has been picked up,
        yield(time);                                            // and yield to simulate time between shipments
    }
    printer.print(Printer::BottlingPlant, 'F');                 // Print that the plant is done executing
}
