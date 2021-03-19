/*****************************************************************/
/* Author: Daniel Janis                                           /
/  Course: CS 4280                                                /
/  File: tree.cpp                                                 /
/  Last Edit: 4/21/2020                                           /
/                                                                 /
/  newNode() builds new nodes for the parsing,                    /
/                                                                 /
/  printPreorder() and prettyPrint() are for printing the         /
/  parse tree, if it exists.                                     */
/*****************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
#include "node.h"
#include "symbolTable.h"

using namespace std;

static vector<string> symbolTable;

// Create a new node in the tree for the parser to use (initializes its children to NULL and sets the label)
node* newNode(string newWord) {
	node *temp = new node;	
	temp->childOne = NULL;
	temp->childTwo = NULL;
	temp->childThree = NULL;
	temp->childFour = NULL;
	temp->label = newWord;
	return temp;
}

// Similar to P0, this function will recursively print the parse tree using the helper prettyPrint function
void printPreorder(int depth, node* root) {
	if (root != NULL) {
		prettyPrint(depth+1, root);
		printPreorder(depth+1, root->childOne);
		printPreorder(depth+1, root->childTwo);
		printPreorder(depth+1, root->childThree);
		printPreorder(depth+1, root->childFour);
	}
}

// Prints a line of the parse tree when printPreorder() calls this function
void prettyPrint(int depth, node* root) {
	symbol *id = new symbol;
	
	// CHECKING FOR RE-DECLARE
	if (root->label.compare("vars") == 0) {
		if (root->tkTwo.tokenType == ID_TOKEN) {
			id->name = root->tkTwo.tokenString;
			id->type = root->tkTwo.tokenType;
			id->scope = root->label;
			if (variableExists(id->name) == true) {
				cout << "[SEMANTICS ERROR] The Identifier [ " << id->name << " ] from the scope [" << id->scope << "] on line " << root->tkTwo.lineNum << ", was redeclared, BAD!\n";
			}
			else {
				//cout << "Identifier: [ " << id->name << " ] was declared.\n";
				symbolTable.push_back(id->name);
				
			}
		}
	}

	// CHECKING FOR IF THEY'VE BEEN DECLARED OR NOT
	if (root->label.compare("R") == 0) {
		if (root->tkOne.tokenType == ID_TOKEN) {
			id->name = root->tkOne.tokenString;
			id->type = root->tkOne.tokenType;
			id->scope = root->label;
			printSemantic(root->tkOne.lineNum, id->name, id->scope);
		}			
	}
	else if (root->label.compare("in") == 0) {
		if (root->tkTwo.tokenType == ID_TOKEN) {
			id->name = root->tkTwo.tokenString;
			id->type = root->tkTwo.tokenType;
			id->scope = root->label;
			printSemantic(root->tkTwo.lineNum, id->name, id->scope);
		}		
	}
	else if (root->label.compare("assign") == 0) {
		if (root->tkOne.tokenType == ID_TOKEN) {
			id->name = root->tkOne.tokenString;
			id->type = root->tkOne.tokenType;
			id->scope = root->label;
			printSemantic(root->tkOne.lineNum, id->name, id->scope);
		}		
	}
	else if (root->label.compare("label") == 0) {
		if (root->tkTwo.tokenType == ID_TOKEN) {
			id->name = root->tkTwo.tokenString;
			id->type = root->tkTwo.tokenType;
			id->scope = root->label;
			printSemantic(root->tkTwo.lineNum, id->name, id->scope);
		}		
	}
	else if (root->label.compare("goto") == 0) {
		if (root->tkTwo.tokenType == ID_TOKEN) {
			id->name = root->tkTwo.tokenString;
			id->type = root->tkTwo.tokenType;
			id->scope = root->label;
			printSemantic(root->tkTwo.lineNum, id->name, id->scope);
		}	
	}
}

bool variableExists(string var) {
	// std::find takes in the first, last, and what we are searching for
	if (find(symbolTable.begin(), symbolTable.end(), var) != symbolTable.end()) {
		// Returns true if the var was found
		return true;
	}
	else {
		// Returns false if the var was not found
		return false;
	}
}

void printSemantic(int depth, string name, string scope) {
	if (variableExists(name) == true) {
		cout << "Identifier: [ " << name << " ] in the scope [" << scope << "] was previously declared, GOOD\n";
	}
	else {
		cout << "[SEMANTICS ERROR] The Identifer [ " << name << " ] from the scope [" << scope << "] on line " << depth << ", was NOT previously declared, BAD!\n";
	}
}
