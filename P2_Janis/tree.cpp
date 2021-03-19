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
#include "node.h"

using namespace std;

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
	for (int i = 1; i < depth; i++) {
		cout << "--";
	}
	cout << " <" << root->label << "> ";
	cout << root->tkOne.tokenString + " " + root->tkTwo.tokenString + " " + root->tkThree.tokenString
			+ " " + root->tkFour.tokenString + " " + root->tkFive.tokenString;
	cout << "\n";
}
