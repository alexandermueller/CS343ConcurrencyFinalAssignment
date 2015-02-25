#ifndef __BOTTLINGPLANT_H__
#define __BOTTLINGPLANT_H__

#include "truck.h"
#include "printer.h"
#include "nameserver.h"

_Task Truck;

_Task BottlingPlant {
    Truck *truck;                   
    int num_flavours;           // Number of flavours available
    Printer &printer;
    unsigned int time;          // Time between shipments
    NameServer &nameserver;                                                        
    unsigned int max_stock;     // Maximum stock per flavour
    unsigned int max_shipped;   // Maximum shipped per flavour
    unsigned int *production;   // Array of number of bottles produced for each flavour    
    unsigned int num_machines;  // Number of vending machines
    void main();
  public:
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    ~BottlingPlant();
    bool getShipment( unsigned int cargo[] );
};

#endif
