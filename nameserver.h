#ifndef __NAMESERVER_H__
#define __NAMESERVER_H__

#include "printer.h"
#include "vendingmachine.h"

_Task NameServer {
    int s_id;                           // Student id meant for the admin to access
    int *students;                      // Array of machines locations mapped to each student
    Printer &printer;                                                                       
    VendingMachine *machine;            // Vending machine meant for the admin to access 
    unsigned int num_machines;                                                              
    unsigned int num_students;
    VendingMachine **machines;          // Array of all the vending machines
    unsigned int machines_registered;
    void main();        
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
};

#endif
