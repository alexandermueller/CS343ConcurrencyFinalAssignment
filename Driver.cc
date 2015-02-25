#include "include.h" 

using namespace std;

PRNG RandomInt((unsigned int)getpid());
 
/**
 * sodaSim
 * Continues the soda simulation. Prints to std out when finished. 
 **/

void sodaSim(char *file, unsigned int seed) {         
    RandomInt.seed(seed);                                   // Seeds srand
    ConfigParms params;
    processConfigFile(file, params);                        // Process the config paramaters in the config file
    Printer printer(params.numStudents, params.numVendingMachines, params.numCouriers);
    Bank bank(params.numStudents);
    Parent *parent = new Parent(printer, bank, params.numStudents, params.parentalDelay);
    WATCardOffice *office = new WATCardOffice(printer, bank, params.numCouriers);
    NameServer *nameserver = new NameServer(printer, params.numVendingMachines, params.numStudents);
    VendingMachine *vendingmachines[params.numVendingMachines];
    for (unsigned int i = 0; i < params.numVendingMachines; ++i) {
        vendingmachines[i] = new VendingMachine(printer, *nameserver, i, params.sodaCost, params.maxStockPerFlavour);
    }
    BottlingPlant *bp = new BottlingPlant(printer, *nameserver, params.numVendingMachines, params.maxShippedPerFlavour,
                                                                params.maxStockPerFlavour, params.timeBetweenShipments);
    Student *students[params.numStudents];
    for (unsigned int i = 0; i < params.numStudents; ++i) {
        students[i] = new Student(printer, *nameserver, *office, i, params.maxPurchases);
    }
    for (unsigned int i = 0; i < params.numStudents; ++i) delete students[i];
    delete bp;
    for (unsigned int i = 0; i < params.numVendingMachines; ++i) delete vendingmachines[i];
    delete nameserver;
    delete office;
    delete parent;
    for (int i = 0; i < (4 + params.numStudents + params.numVendingMachines + params.numCouriers); ++i) cout << "******\t";
    cout << "******" << endl;
}

/**
 * uMain::main
 * Makes sure the input is correct and proceeds the program, 
 * otherwise throws an appropriate error that is handled by
 * usage, which prints the appropriate messages for all 
 * contexts.
 **/

void uMain::main() {
    if (argc < 4) {                                           // Proceed if there is a proper amount of input
        try {                
            char defaultfile[12] = {'s', 'o', 'd', 'a', '.', 'c', 'o', 'n', 'f', 'i', 'g', '\0'};    
            char* file = argc > 1 ? argv[1] : defaultfile;    // Attempt to check each argument for correctness
            int seed = RandomInt.operator()();                // Default values for all the arguments
            if (argc > 2) {
                if (!isNumber(argv[2])) throw Error(2);
                seed = atoi(argv[2]);
            }
            sodaSim(file, seed);
        }
        catch (uFile::Failure) {                              // Catch the failure to open the config file supplied
            usage(argv[1], 1);                                // Have usage print out to stderr
        }           
        catch (Error &e) {                                    // Catch any errors thrown
            usage(argv[e.usage], e.usage);                    // Pass the error to usage which prints out the appropriate error
        }
    }
    else {                                                    // Otherwise there is an incorrect amount of arguments
        usage("bad arguments", 0);                            // Print to stderr 
    }
}
