#ifndef __PARENT_H__
#define __PARENT_H__

#include "bank.h"
#include "printer.h"

_Task Parent {
    int delay;
    Bank &bank;
    int students;       // Amount of students
    Printer &printer;
    void main();
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
};

#endif
