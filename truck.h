#ifndef __TRUCK_H__
#define __TRUCK_H__

#include "printer.h"
#include "nameserver.h"
#include "bottlingplant.h"
#include "vendingmachine.h"

_Task Truck {
    int num_flavours;           // Amount of flavours in every machine
    Printer &printer;
    unsigned int *cargo;        // The bottles of each flavour that need to be delivered
    BottlingPlant &plant;
    unsigned int max_stock;     // The maximum stock of each flavour in each machine
    NameServer &nameserver;     
    unsigned int num_machines;  
    VendingMachine **machines;  // The list of machines that have to be delivered to
    void main();
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
    ~Truck();
};

#endif
