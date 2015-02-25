#include "include.h"

using namespace std;

Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay ) : printer(prt), bank(bank), 
                                                                                students(numStudents), delay(parentalDelay) {

    printer.print(Printer::Parent, 'S');                            // Print that the parent has begun executing
}

/**
 * Parent::main
 * The parent periodically deposits a random amount of money from 1 to 3 dollars into
 * a random student's account, as well as announces when a deposit has been made. Stops 
 * looping when the destructor is called. 
 **/

void Parent::main() {
    unsigned int amount = 0, student = 0;
    for (;;) {
        _Accept(~Parent) { break; }                                 // The destructor has been called, so break out of the loop!  
        _Else {                                                     // Otherwise,
            amount = RandomInt.operator()(1,3);                     // randomly generate the deposite amount and
            student = RandomInt.operator()(students - 1);           // the student who will recieve the amount 
            yield(delay);                                           // Yield for the given parentalDelay time
            printer.print(Printer::Parent, 'D', student, amount);   // Print that the deposit is occcurring
            bank.deposit(student, amount);                          // Deposit the amount to the account
        }
    }
    printer.print(Printer::Parent, 'F');                            // Print that the parent is finished executing
}
