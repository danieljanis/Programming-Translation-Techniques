/***************************************************************/
/* Author: Daniel Janis                                         /
/  Course: CS 4280                                              /
/  File: scanner.cpp                                            /
/  Last Edit: 4/21/2020                                         /
/                                                               /
/	 Functions in this file are used in main() and driver()    */
/***************************************************************/

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "scanner.h"
#include "driver.h"

using namespace std;

static ifstream infile;
static int lineCount;
static int stringPos;
static string currentLine;
static string currentToken;
static bool eof = false;

/* This function starts the scanning process, it does the following: 
*		1. opens the file,
*		2. sets the line count and position of the string
*		3. gets the first line
*		4. initializes the first token
*/
void initScan(string fileName) {
	infile.open(fileName.c_str());
	lineCount = 0;
	stringPos = 0;
	// the comment below was originally what I had here
	//getline(infile, currentLine);
	nextLine();
	currentToken = "";
	if (eof) {
		cout << "\nSCANNER ERROR: Found an empty line(s) at the start of the program.\n";
		exit(1);
	}
}

/* This scan() function takes care of everything after initScan() 
* 	- handles KEYWORDS, EOF, and all tokens
*/
Token scan() {
	Token newToke;
	int tokenType;
	/* eof is only true if the nextLine() function can't find another line */
	if (!eof) {
		tokenType = driver();

		/* If tokenType is a negative, then an error occurred,
		*	checkErrors() prints the SCANNER ERROR's */
		if (tokenType < 0) {
			checkErrors(tokenType);
		}
		
		/* If tokenType is 10000, the file has no more lines to scan 
		* 	- returns the EOF token*/
		if (tokenType == 10000) {
			newToke.tokenType = EOF_TOKEN;
			newToke.tokenString = "EOF";
			newToke.lineNum = lineCount;
			return newToke;
		}

		/* When we don't have an error, or EOF, initialize the token */
		newToke.tokenType = (tokenOption)tokenType;
		newToke.tokenString = currentToken;
		newToke.lineNum = lineCount;

		/* This handles KEYWORD's */
		if (newToke.tokenType == ID_TOKEN && checkForKeyword()) {
			newToke.tokenType = KEYWORD;
		}
		
		/* This gets the next line in the file */
		if (stringPos == currentLine.size()) {
			nextLine();
		}

		/* Reset this string for the next token */
		currentToken = "";
		return newToke;
	}
	/* eof has been flipped to true, there are no more lines */
	else {
		newToke.tokenType = EOF_TOKEN;
		newToke.tokenString = "EOF";
		newToke.lineNum = lineCount;	
		return newToke;
	}
}

/* When tokenType is negative (in the scan() function) an error has been found and this function prints it and exits */
void checkErrors(int errorNum) {
	const string ERROR_MESSAGE[9] = {"Found a character that wasn't in alphabet.\n",
									"Found a character not in alphabet while building Identifier.\n",
									"Found a character not in alphabet while building Integer.\n",
									"Found a character not in alphabet while building Operator.\n",
									"Found invalid letter after equal sign.\n",
									"Found invalid digit after equal sign.\n",
									"Found invalid operator after equal sign.\n",
									"Found invalid whitespace after equal sign.\n",
									"Found invalid comment after equal sign.\n"};
									
	errorNum = (errorNum * -1) - 1;
	cout << "\nSCANNER ERROR:\n" << ERROR_MESSAGE[errorNum] << "(error at line: " << lineCount << ", index: " << stringPos << ")\n\n";
	exit(1);
}

/* Used in the scan() function to check if an Identifier is a KEYWORD */
bool checkForKeyword() {
	if (currentToken.compare("label") == 0) {
		return true;	
	}
	else if (currentToken.compare("goto") == 0) {
		return true;
	}
	else if (currentToken.compare("loop") == 0) {
		return true;
	}
	else if (currentToken.compare("void") == 0) {
		return true;
	}
	else if (currentToken.compare("declare") == 0) {
		return true;
	}
	else if (currentToken.compare("return") == 0) {
		return true;
	}
	else if (currentToken.compare("in") == 0) {
		return true;
	}
	else if (currentToken.compare("out") == 0) {
		return true;
	}
	else if (currentToken.compare("program") == 0) {
		return true;
	}
	else if (currentToken.compare("iffy") == 0) {
		return true;
	}
	else if (currentToken.compare("then") == 0) {
		return true;
	}
	else if (currentToken.compare("assign") == 0) {
		return true;
	}
	else if (currentToken.compare("data") == 0) {
		return true;
	}
	else return false;
}

/* This function sets the end of file (eof) if found,
*	otherwise it will reset the index of the line and increment the line counter */
void nextLine() {

	/* This do-while handles EOF and tailing '\n' characters */
	do {
		if (!getline(infile, currentLine)) {
			eof = true;
		}
		lineCount++;
	} while (currentLine.empty() && eof == false);
	//lineCount++; <--- (EDITED FOR P2) This was causing an issue and counting too many lines
	stringPos = 0;	
}

/* This function handles comments - called from getChar() */
void skipComment() {
	int commentLine = lineCount;
	int commentPos = stringPos;
	/* Gets new lines from the file until the closing '#' is found */
	do {
		stringPos++;
		if (stringPos == currentLine.size()) {
			nextLine();
			/* Handles if an EOF is found before a comment is closed*/
			if (eof) {
				cout << "\nSCANNER ERROR: The comment opened on line " << commentLine << " at index " << commentPos << " was never closed.\n\n";
				exit(1);
			}
		}
	} while (currentLine[stringPos] != '#');
}

/* Gets the current character, helps handle comments and EOF */
int getChar() {
	while (isspace(currentLine[stringPos]) || currentLine[stringPos] == '#') {
		if (currentLine[stringPos] == '#') {
			skipComment();
		}
		stringPos++;
		
		if (stringPos == currentLine.size()) {
			nextLine();
		}
		if (eof) {
			return 10000;
		}
	}
	/* Stores the currentToken and returns the current character */
	currentToken += currentLine[stringPos];
	return currentLine[stringPos];
}

/* Gets the next character */
char nextChar() {
	if (stringPos < currentLine.size()-1) {
		return currentLine[++stringPos];
	}
	else {
		// Lets scan() know we've found the end of the string
		stringPos++;
		return '\n';
	}
}
