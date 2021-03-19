/*****************************************************************/
/* Author: Daniel Janis                                           /
/  Course: CS 4280                                                /
/  File: node.h                                                   /
/  Last Edit: 2/21/2020                                          */
/*****************************************************************/

#ifndef NODE_H
#define NODE_H
#include <vector>
#include <string.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

struct node {
	node *left;
	node *right;

	int depth;
	string word;
	string key;
	vector<string> strings;
};

// For tree.cpp
struct node *buildTree(string fileName);

struct node *newNode(int depth, string newKey, string newWord);

struct node *insert(node* node, string key, string word, int depth);

void pushWords(node* node, string word);

// For main.cpp
int checkExtension(string fileName);

int checkEmpty(string fileName);

string removeExtension(string fileName);

void noFile(string fileName);

#endif
