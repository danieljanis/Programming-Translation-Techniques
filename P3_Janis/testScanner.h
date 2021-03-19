#ifndef TESTSCANNER_H
#define TESTSCANNER_H

/***************************************************************/
/* Author: Daniel Janis                                         /
/  Course: CS 4280                                              /
/  File: testScanner.h                                          /
/  Last Edit: 4/5/2020                                         */
/***************************************************************/

#include <string>
#include <fstream>
#include <iostream>
using namespace std;

int checkExtension(string fileName);

int checkEmpty(string fileName);

string removeExtension(string fileName);

void noFile(string fileName);

void driver(string fileName);

#endif
