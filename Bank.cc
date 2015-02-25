#include "include.h"

using namespace std;

Bank::Bank(unsigned int numStudents) {
    accounts = new unsigned int[numStudents];                       // Create 
    for (unsigned int i = 0; i < numStudents; ++i) accounts[i] = 0; // and initialize all the student accounts to 0
}

Bank::~Bank() {
    delete [] accounts;
}

/**
 * Bank::deposit
 * The parent calls this to deposit funds into a random 
 * student's bank account.
 **/

void Bank::deposit( unsigned int id, unsigned int amount ) {
    accounts[id] += amount;
}

/**
 * Bank::withdraw
 * Couriers call this to add funds to the student's watcard.
 * They must wait if there is insufficient funds in the account.
 * When the requested amount is available they can then proceed
 * to withdraw that amount from the student's account.
 **/

void Bank::withdraw( unsigned int id, unsigned int amount ) {
    while (accounts[id] < amount) _Accept(deposit);                 // Wait until the proper amount is in the account
    accounts[id] -= amount;
}
