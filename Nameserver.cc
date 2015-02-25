#include "include.h"

using namespace std;

NameServer::NameServer(Printer &prt, unsigned int numVendingMachines, unsigned int numStudents) : printer(prt), num_machines(numVendingMachines), num_students(numStudents) {
    printer.print(Printer::NameServer, 'S');                                // Print that the nameserver has started running
    s_id = machines_registered = 0;                                         // Init. the id and count of machines to 0
    students = new int[numStudents];                                                         
    for (unsigned int i = 0; i < num_students; ++i) students[i] = i % num_machines; // Create and initialize the mapping array
    machines = new VendingMachine*[num_machines];                           // Create the vending machine array
    for (unsigned int i = 0; i < num_machines; ++i) machines[i] = NULL;
}

NameServer::~NameServer() {
    delete [] students;
    delete [] machines;
}

/**
 * NameServer::VMregister
 * Called by the vending machines, VMregister gives the nameserver
 * a pointer to the vending machine so that the students can find them.
 **/

void NameServer::VMregister(VendingMachine *vendingmachine) {
   machine = vendingmachine;
}

/**
 * NameServer::getMachine
 * Called by the students, getMachine returns the next vending machine
 * that the current student will check. 
 **/

VendingMachine* NameServer::getMachine(unsigned int id) {
    s_id = id;                                                              // Save the student id for later printing from the server
    return machines[students[id]];                                          
} 

/**
 * NameServer::getMachineList
 * Called by the truck, getMachineList returns a list of vending
 * machines for the truck to visit.
 **/

VendingMachine** NameServer::getMachineList() {         
    return machines;
}

/**
 * NameServer::main
 * The nameserver administrates requests from the students and truck for 
 * access to the vending machines as soon as all the vending machines have 
 * registered themselves. It announces vending machine registrations and 
 * requests made by students looking for vending machines. Once the 
 * destructor is called, the nameserver finishes executing.
 **/

void NameServer::main() {
    for(;;) {
        _Accept(~NameServer) { break; }                                     // Break out of the loop when the destructor is called
        or _When(machines_registered != num_machines) _Accept(VMregister) { // Accept VMregister calls until all machines registered 
            unsigned int m_id = machine->getId();                           // Get the machine's id
            printer.print(Printer::NameServer, 'R', m_id);                  // Print that the nameserver has registered a machine
            machines[m_id] = machine;                                       // and record it in the machine array
            ++machines_registered;                                          // and increment the number of machines registeed
        }
        or _Accept(getMachine) {                                            // Otherwise accept calls to getMachine,
            printer.print(Printer::NameServer, 'N', s_id, students[s_id]);  // announce them,
            ++students[s_id] %= num_machines;                               // and then increment the student's next machine
        }
        or _Accept(getMachineList);                                         // Lastly, accept calls to getMachineList  
    }   
    printer.print(Printer::NameServer, 'F');                                // Print that the nameserver has finished executing
}
