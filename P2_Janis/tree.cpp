/*****************************************************************/
/* Author: Daniel Janis                                           /
/  Course: CS 4280                                                /
/  File: tree.cpp                                                 /
/  Last Edit: 2/22/2020                                           /
/                                                                 /
/  buildTree() gets the words from the file/stdin and finds the   /
/  key for each word. The insert() function places each node in   /
/  the binary tree, at either the root, left, or right position.  /
/  The tree is sorted using recursion and printed using the       /
/  functions in the traversals.cpp file                           /
/                                                                */
/*****************************************************************/

#include <iostream>
#include "node.h"

using namespace std;

// create a node constructor/destrutor
// ----> a way to create a new node for my pareser to use
// print PREORDER (traversing and printing the way I want) function
// -----------> start with depth = 0 (have depth=depth+1 counter as a parameter, incrementing in the function call)

// prettyPrint(depth+1);

// Create a new node in the tree
node* newNode(string newWord) {
	// P2:  SET NODE POINTERS TO NULL, set label to whatever gets passed in
	node *temp = new node;	
	temp->childOne = NULL;
	temp->childTwo = NULL;
	temp->childThree = NULL;
	temp->childFour = NULL;
	temp->label = newWord;
	return temp;
}

void printPreorder(int depth, node* root) {
	if (root != NULL) {
		prettyPrint(depth+1, root);
		printPreorder(depth+1, root->childOne);
		printPreorder(depth+1, root->childTwo);
		printPreorder(depth+1, root->childThree);
		printPreorder(depth+1, root->childFour);
	}
}

void prettyPrint(int depth, node* root) {
	for (int i = 1; i < depth; i++) {
		cout << "--";
	}
	cout << " <" << root->label << "> ";
	cout << root->tkOne.tokenString + " " + root->tkTwo.tokenString + " " + root->tkThree.tokenString
			+ " " + root->tkFour.tokenString + " " + root->tkFive.tokenString;
	cout << "\n";
}
