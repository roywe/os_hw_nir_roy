//
// Created by USER on 7/6/2024.
//

#include "bank.h"












int main (int argc, char *argv[]) {
	//checking arguments
	if ( argc == 1) {
		cerr << "Bank error: illegal arguments" << endl ;
	}
    // parse argument and check them - number of argument + all the files are valid
    // start bank object
    //  start atm - first without threads passing file and check it can handle it
    // till here essential so we know we are fine
    /////////////////////////////////////////////////////////////////////////////////
    //  after the first level - pthread and stuff (after objects are ready
    log.open("log.txt", std::ios::out | std::ios::trunc);
    if (!log) {
        std::cerr << "Error opening output file!" << std::endl;
        exit(1);
    }
    Bank bank = Bank();

    ATM atm0 = ATM(0, &bank,argv[1], "log.txt");
    atm0.run();







    log.close();
}

