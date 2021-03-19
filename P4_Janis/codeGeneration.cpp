#include <vector>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include "codeGeneration.h"

using namespace std;

/* temp files, variable names, and label names initialized here and
   stored in the vector initials */
static int LabelCntr = 0;
static int VarCntr = 0;
static ofstream asmFile;
static vector<string> initials;

/* This function creates the temporary labels and variables for all values
   which were not known but needed to be used in advance in the .asm */
string newName(nameType type) {
	stringstream newName;
	if (type == VAR) {
		newName << "T" << VarCntr++;
		initials.push_back(newName.str());
		initials.push_back("0");
	}
	else {
		newName << "L" << LabelCntr++;		
	}
	return newName.str();
}

/* This function is called at the end of the .asm file to load in the temp values
   from the newName() function and end the .asm execution */
void generateAssemblyCode(string fileName, node* root) {
	asmFile.open(fileName.c_str());
	if (!asmFile) {
		cout << "Could not open .asm file for writing...\n";
		return;
	}
	generateNodes(root);
	asmFile << "STOP\n";
	for (int i = 0; i < initials.size(); i+=2) {
		asmFile << initials.at(i) << " " << initials.at(i+1) << "\n";
	}
	asmFile.close();
}

/* This function is called to let the other functions below it 
   a way of generating the nodes and to determine which function needs to be called */
void generateNodes(node* root) {
	if (root) {
		if (root->label.compare("vars") == 0) {
			vars(root);
		}
		else if (root->label.compare("out") == 0) {
			out(root);	
		}
		else if (root->label.compare("expr") == 0) {
			expr(root);	
		}
		else if (root->label.compare("N") == 0) {
			N(root);	
		}
		else if (root->label.compare("A") == 0) {
			A(root);	
		}
		else if (root->label.compare("M") == 0) {
			M(root);	
		}
		else if (root->label.compare("R") == 0) {
			R(root);	
		}
		else if (root->label.compare("in") == 0) {
			in(root);	
		}
		else if (root->label.compare("iffy") == 0) {
			iffy(root);
		}
		else if (root->label.compare("loop") == 0) {
			loop(root);
		}
		else if (root->label.compare("assign") == 0) {
			assign(root);
		}
		else if (root->label.compare("label") == 0) {
			label(root);		
		}
		else if (root->label.compare("goto") == 0) {
			gotoMarkForHelp(root);
		}
		else {
			generateNodes(root->childOne);
			generateNodes(root->childTwo);
			generateNodes(root->childThree);
			generateNodes(root->childFour);
		}		
	}
}

/* Grabs the identifier and the initial integer value and stores them 
   for final output */
void vars(node* root) {
	initials.push_back(root->tkTwo.tokenString);
	initials.push_back(root->tkFour.tokenString);
	generateNodes(root->childOne);
}

/* Stores the temp variable from <expr> and writes it to the console */
void out(node* root) {
	string temp = newName(VAR);
	generateNodes(root->childOne);
	asmFile << "STORE " << temp << endl;
	asmFile << "WRITE " << temp << endl;
}

/* Stores the temp variable and specifically allows for subtraction here,
   other operations are lower on the parse tree */
void expr(node* root) {
	if (root->tkOne.tokenString.compare("-") == 0) {
		generateNodes(root->childTwo);
		string temp = newName(VAR);
		asmFile << "STORE " << temp << endl;
		generateNodes(root->childOne);
		asmFile << "SUB " << temp << endl;
	}
	else {
		generateNodes(root->childOne);
	}
}

/* Stores a temp variable and specifically either divides here, multiplies here,
   or continues to lower on in the parse tree */
void N(node* root) {
	if (root->tkOne.tokenString.compare("/") == 0) {
		generateNodes(root->childTwo);
		string temp = newName(VAR);
		asmFile << "STORE " << temp << endl;
		generateNodes(root->childOne);
		asmFile << "DIV " << temp << endl;
	}
	else if (root->tkOne.tokenString.compare("*") == 0) {
		generateNodes(root->childTwo);
		string temp = newName(VAR);
		asmFile << "STORE " << temp << endl;
		generateNodes(root->childOne);
		asmFile << "MULT " << temp << endl;
	}
	else {
		generateNodes(root->childOne);
	}
}

/* Stores a temp variable and specifically adds here,
   or continue to lower on in the parse tree */
void A(node* root) {
	if (root->tkOne.tokenString.compare("+") == 0) {
		generateNodes(root->childTwo);
		string temp = newName(VAR);
		asmFile << "STORE " << temp << endl;
		generateNodes(root->childOne);
		asmFile << "ADD " << temp << endl;
	}
	else {
		generateNodes(root->childOne);
	}
}
/* Checks for negative or moves to lower on in the parse tree */
void M(node* root) {
	if (root->tkOne.tokenString.compare("*") == 0) {
		generateNodes(root->childOne);
		asmFile << "MULT -1\n";
	}
	else {
		generateNodes(root->childOne);	
	}
}

/* Loads the Identifier/Integer into 'memory' or restarts <expr> inside the ( ) */
void R(node* root) {
	if (root->childOne) {
		generateNodes(root->childOne);
	}
	else {
		asmFile << "LOAD " << root->tkOne.tokenString << endl;
	}
}

/* For reading user input to store into 'memory'  */
void in(node* root) {
	asmFile << "READ " << root->tkTwo.tokenString << endl;
}

/* Compares 2 different expressions using RO. If this comparison is false,
   the statement will be skipped */
void iffy(node* root) {
	generateNodes(root->childOne);
	string temp = newName(VAR);
	asmFile << "STORE " << temp << endl;
	generateNodes(root->childThree);
	asmFile << "SUB " << temp << endl;
	string tempL = newName(LABEL);
	RO(root->childTwo, tempL);
	generateNodes(root->childFour);
	asmFile << tempL << ": NOOP\n";
}

/* Compares 2 different expressions using RO and repeats until this comparison is false. 
   Otherwise it will just loop. If the loop is initially false, skips the code. */
void loop(node* root) {
	string tempLabel = newName(LABEL);
	asmFile << tempLabel << ": NOOP\n";
	generateNodes(root->childOne);
	string temp = newName(VAR);
	asmFile << "STORE " << temp << endl;
	generateNodes(root->childThree);
	asmFile << "SUB " << temp << endl;
	string tempL = newName(LABEL);
	RO(root->childTwo, tempL);
	generateNodes(root->childFour);
	asmFile << "BR " << tempLabel << endl;
	asmFile << tempL << ": NOOP\n";
}

/* Handles all of the greater than, lesser than, equal to comparisons in .asm */
void RO(node* root, string tempL) {
	if (root->tkOne.tokenString.compare("==") == 0) {
		asmFile << "BRNEG " << tempL << endl;
		asmFile << "BRPOS " << tempL << endl;
	}
	else if (root->tkOne.tokenString.compare("<") == 0) {
		if (root->tkTwo.tokenString.compare("<") == 0) {
			asmFile << "BRNEG " << tempL << endl;
		}
		else if (root->tkTwo.tokenString.compare(">") == 0) {
			asmFile << "BRZERO " << tempL << endl;
		}
		else {
			asmFile << "BRZNEG " << tempL << endl;
		}
	}
	else if (root->tkOne.tokenString.compare(">") == 0) {
		if (root->tkTwo.tokenString.compare(">") == 0) {
			asmFile << "BRPOS " << tempL << endl;
		}
		else {
			asmFile << "BRZPOS " << tempL << endl;
		}
	}
}

/* Stores the value of an expression into 'memory' */
void assign(node* root) {
	generateNodes(root->childOne);
	asmFile << "STORE " << root->tkOne.tokenString << endl;
}

/* A place for the GOTO to go to*/
void label(node* root) {
	asmFile << root->tkTwo.tokenString << ": NOOP\n";
}

/* Can GOTO a label */ 
void gotoMarkForHelp(node* root) {
	asmFile << "BR " << root->tkTwo.tokenString << "\n";
}
