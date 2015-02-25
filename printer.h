#ifndef __PRINTER_H__
#define __PRINTER_H__

_Cormonitor Printer {
    int p_id;               // Stores the location for the next state to go in the buffer
    int p_state;            // The next state to be buffered
    int p_value1;           // The next value1 to be buffered   
    int p_value2;           // The next value2 to be buffered  
    int total;              // The total amount of "working" tasks
    int students;           // The total amount of students
    int machines;           // The total amount of vending machines
    int couriers;           // The total amount of couriers
    int s_location;         // The location of the student states in the buffer
    int m_location;         // The location of the vending machine state in the buffer
    int c_location;         // The location of the courier states in the buffer
    char *states;           // Buffer for all the states
    int **values;           // Buffer recording all the extra values for each state
    int nonEmptyBuffer();   // Counts how many remaining states there are in the buffer  
    void main();   
  public:
    enum Kind { Parent, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
};

#endif
