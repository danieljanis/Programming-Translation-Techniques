/***************************************************************/
/* Author: Daniel Janis                                         /
/  Course: CS 4280                                              /
/  File: testScanner.cpp                                        /
/  Last Edit: 4/5/2020                                          /
/                                                               / 
/      main() deals with standard input and file inputs. It     /
/	   also initializes the scanner, scans the file or input,   /
/	   and prints the tokens, along with SCANNER ERRORS until   /
/	   EOF is reached or an ERROR.                             */
/***************************************************************/

#include <ctype.h>
#include <stdio.h>
#include "parser.h"
#include "scanner.h"
#include "testScanner.h"
#include "token.h"

int main(int argc, char *argv[]) {

	const string TOKEN_TYPE[5] = {"idToken", "intToken", "opToken", "eofToken", "keyword"};

	node* root = NULL;

	string fileName;
	int driverReturn;
	
	// if filename was passed,
	if (argc == 2) {
		fileName = argv[1];
		// if no extension, append .sp2020
		if (checkExtension(fileName) == 0) {
			fileName.append(".sp2020");
		}
		// tries to open file,
		ifstream infile(fileName.c_str());
		if(!infile) {
			// creates temporary file (stdin)
			noFile(fileName);
		}
		// It can open the file
		else {
			infile.seekg(0, ios::end);
			if (infile.tellg() == 0) {
				noFile(fileName);
			}
		}
	}
	// If invoked as ./scanner, create a temp file and populate it (stdin)
	else if (argc == 1) {
		fileName = "output.sp2020";
		noFile(fileName);
	}
	// Wrong invokation, exit
	else {
		cout << "\nError, too many arguments, terminating...\n";
		return 1;
	}

	initScan(fileName);
	root = parser();
	printPreorder(0, root);
	// RUN pre-order print on root, this will build the parse tree

	
	//Token newToke;
	//do {
	//	newToke = scan();
	//	cout << "<" << TOKEN_TYPE[newToke.tokenType] << ", " << newToke.tokenString << ", " << newToke.lineNum << ">" << endl; 
	//} while (newToke.tokenType != EOF_TOKEN);
		
	return 0;
}

// Returns 1 if an extension exists, 0 if no extension exists.
int checkExtension(string fileName) {
        string dot = ".";
        // If the dot is at any position but -1 the extension exists
        if (fileName.find(dot) != -1) {
                return 1;
        }
        // Else the extension doesn't exist (no dot)
        else {
                return 0;
        }
}

// Returns 1 if file is empty,
int checkEmpty(string fileName) {
        ifstream infile(fileName.c_str());
        // seekg finds the "end" position of the file,
        // tellg returns the "end" position
        infile.seekg(0, ios::end);
        // Checks if the file is empty:
        if (infile.tellg() == 0) {
                return 1;
        }
        return 0;
}

//Removes the file extension
string removeExtension(string fileName) {
        if (checkExtension(fileName) != 0) {
                // findDot is a value representing where in the string the dot is
                size_t findDot = fileName.find_last_of(".");
                return fileName.substr(0, findDot);
        }
        return fileName;
}

// Creates a file with the given fileName, or provides a temporary one
void noFile(string fileName) {
        string keyb_input;
        ofstream tempFile;
        tempFile.open(fileName.c_str());
        if (checkEmpty(fileName) == 1) {
                cout << "\nPlease enter alphanumerical strings, separated by spaces,\n"
                         << "and when you are finished hit CTRL+D to save the file.\n\n";
        }
        while (getline(cin, keyb_input)) {
                tempFile << keyb_input << "\n";
        }
        cout << "\n\n" << fileName << " has been saved.\n\n";
        tempFile.close();
}
