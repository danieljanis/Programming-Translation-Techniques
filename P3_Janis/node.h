/*****************************************************************/
/* Author: Daniel Janis                                           /
/  Course: CS 4280                                                /
/  File: node.h                                                   /
/  Last Edit: 4/21/2020                                          */
/*****************************************************************/

#ifndef NODE_H
#define NODE_H
#include <string>
#include "token.h"

struct node {
	node *childOne;
	node *childTwo;
	node *childThree;
	node *childFour;
	
	std::string label;
	Token tkOne;
	Token tkTwo;
	Token tkThree;
	Token tkFour;
	Token tkFive;
};

// Create new nodes, return the null pointers
node* newNode(std::string); 

// These two print functions are for printing the parse tree
void printPreorder(int, node*);

void prettyPrint(int, node*);

#endif
