#include "include.h"
using namespace std;

/**
 * Printer::Printer
 * Generate the printer as well as kickstart main to print out the columns for
 * the initialization step.
 **/
 
Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) : 
                                                                total(5 + numStudents + numVendingMachines + numCouriers),
                                                                students(numStudents), machines(numVendingMachines), 
                                                                couriers(numCouriers), s_location(5), m_location(5 + numStudents), 
                                                                c_location(5 + numStudents + numVendingMachines) { 

    resume(); 
}

Printer::~Printer() {
    delete [] states;
    for (int i = 0; i < total; ++i) { 
        delete [] values[i];
    }
    delete [] values;
}

/**
 * Printer::nonEmptyBuffer
 * Returns the amount of items left in the buffer if non empty,
 * otherwise returns 0.
 **/
 
int Printer::nonEmptyBuffer() {
    int full = 0;                                         
    for (int i = 0; i < total; ++i) if (states[i]) ++full;
    return full;
}

/**
 * Printer::main
 * Main is the coroutine function that loads the state in the appropriate 
 * spot in the state buffer. If a buffer position has to be overwritten, then
 * the buffer is flushed, outputted to stdout, and the new value is inserted . 
 **/

void Printer::main() {
    states = new char[total];                                           // Initialize the states array
    values = new int*[total];                                           // and values arrays
    assert(states && values);                                           // Make sure that these aren't NULL
    
    cout << "Parent\tWATOff\tNames\tTruck\tPlant\t";                    // Print out the intended task for each column
    for (int i = 0; i < students; ++i) cout << "Stud" << i << "\t";
    for (int i = 0; i < machines; ++i) cout << "Mach" << i << "\t";
    for (int i = 0; i < couriers - 1; ++i) cout << "Cour" << i << "\t";
    cout << "Cour" << couriers - 1 << endl;
    
    for (int i = 0; i < total; ++i) {                                   // Set all the arrays to 0 and print out the row spacers
        if (i < (total - 1)) cout << "******\t";
        else cout << "******" << endl; 
        states[i] = 0;
        values[i] = new int[2];
        values[i][0] = values[i][1] = -1;            
    }
    
    suspend();
    
    for (;;) {                                                    
        if (states[p_id] || p_state == 'F' && nonEmptyBuffer()) {       // If the buffer position is already filled, or the state given is 'F', flush the buffer  
            for (int i = 0; i < total; ++i) {                           // Iterate through all the buffer positions 
                cout << states[i];                                      // Print the state
                if (values[i][0] > -1) {                                // and print out the values if they exist in the values buffer                
                    cout << values[i][0];
                    if (values[i][1] > -1) cout << "," << values[i][1]; 
                }  
                cout << "\t";                                           
                states[i] = 0;                                          // Empty the state buffer
                values[i][0] = values[i][1] = -1;                       // and the value buffers
            }
            cout << endl;
        }

        assert(!states[p_id] && values[p_id][0] && values[p_id][1]);    // Make sure that the buffers are clear at the right position
       
        states[p_id] = p_state;                                         // Insert the new state
        values[p_id][0] = p_value1;                                     // and value amounts into their respective buffers
        values[p_id][1] = p_value2;
        
        if (p_state == 'F') {            
            for (int i = 0; i < total; ++i) {                           // Print out the finish state, 
                if (!states[i]) cout << "...";                          // as well as "..." for every other state in the buffer
                else cout << states[i];
                cout << "\t";
                states[i] = 0;                                          // Empty the state buffer
                values[i][0] = values[i][1] = -1;                       // and the value buffers
            }
            cout << endl;
        }
        suspend();
    }
}

/**
 * Printer::print
 * The print routines below generalize to the one at the bottom, so
 * they call that one and let it handle the data instead. 
 **/

void Printer::print( Kind kind, char state )                                { print(kind, 0, state, -1, -1); }
void Printer::print( Kind kind, char state, int value1 )                    { print(kind, 0, state, value1, -1); }
void Printer::print( Kind kind, char state, int value1, int value2 )        { print(kind, 0, state, value1, value2); }
void Printer::print( Kind kind, unsigned int lid, char state )              { print(kind, lid, state, -1, -1); }
void Printer::print( Kind kind, unsigned int lid, char state, int value1 )  { print(kind, lid, state, value1, -1); }

/**
 * Printer::print
 * Used for all general state prints.
 * It feeds the states, values, and ids into their holders and lets main process them.
 **/

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
    if (kind == Parent)             p_id = 0;
    else if (kind == WATCardOffice) p_id = 1; 
    else if (kind == NameServer)    p_id = 2;
    else if (kind == Truck)         p_id = 3;
    else if (kind == BottlingPlant) p_id = 4; 
    else if (kind == Student)       p_id = s_location + lid;
    else if (kind == Vending)       p_id = m_location + lid;
    else if (kind == Courier)       p_id = c_location + lid;
    p_state = state;
    p_value1 = value1;
    p_value2 = value2;
    resume();
} 
