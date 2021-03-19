/***************************************************************/
/* Author: Daniel Janis                                         /
/  Course: CS 4280                                              /
/  File: driver.cpp                                             /
/  Last Edit: 4/5/2020                                          /
/                                                               /
/     This file will return the tokenType and driver() is       /
/     used in the scan() function from scanner.cpp             */
/***************************************************************/

#include <iostream>
#include <stdio.h>
#include "driver.h"
#include "table.h"
#include "scanner.h"

/* Returns the tokenType */
int driver() {
        /* These are reset to start a new Token */
        int currentState = 0;
        int nextState = 0;

        /* used for checking for end of file / also for keeping track of the cur                                                                                                                                                                                                                                             rent character in the line */
        int eofCheck = 0;

        /* While the next state isn't a valid token, */
        while (nextState < 1000) {
                eofCheck = getChar();
                if (eofCheck == 10000) {
                        /* Represents EOF tokenType */
                        return 10000;
                }
                /* Using the table[][] from  table.cpp, sets the current state *                                                                                                                                                                                                                                             /
                currentState = setState(eofCheck);
                if (currentState < 0) {
                        return currentState;
                }
                /* Gets the state of the next character in the string */
                nextState = newState(nextChar());
                if (nextState < 0) {
                        return nextState;
                }
        }
        /* Returns the valid token if it makes it here */
        return (nextState - 1000);
}
