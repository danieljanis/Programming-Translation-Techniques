#ifndef SCANNER_H
#define SCANNER_H

/***************************************************************/
/* Author: Daniel Janis                                         /
/  Course: CS 4280                                              /
/  File: scanner.h                                              /
/  Last Edit: 4/5/2020                                         */
/***************************************************************/

#include <string>
#include "token.h"

void initScan(std::string fileName);

Token scan();

void checkErrors(int errorNum);

void skipComment();

bool checkForKeyword();

void nextLine();

int getChar();

char nextChar();

#endif
