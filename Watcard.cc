#include "include.h"

using namespace std;

WATCard::WATCard() {
    balance = 0;                                // Initialize the watcard balance to 0
};

/**
 * WATCard::deposit
 * Called by the couriers, deposit increments 
 * the watcard's balance by the given amount.
 **/

void WATCard::deposit(unsigned int amount) {
    balance += amount;
}

/**
 * WATCard::withdraw
 * Called by the vending machines, withdraw 
 * decrements the watcard's balance by the given 
 * amount.
 **/

void WATCard::withdraw(unsigned int amount) {
    balance -= amount;
}

/**
 * WATCard::getBalance
 * Called by the students and vending machines, 
 * getBalance returns the balance on the card.
 **/
 
unsigned int WATCard::getBalance() {
    return balance;
}
