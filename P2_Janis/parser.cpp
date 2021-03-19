/***************************************************************/
/* Author: Daniel Janis                                         /
/  Course: CS 4280                                              /
/  File: parser.cpp                                             /
/  Last Edit: 4/21/2020                                         /
/                                                               /  
/	This parser takes in tokens and determines if they are in   /
/   a valid order when considering the BNF for this language    /
/   and table.cpp (which is used to check syntax)               /
/   This file along with tree.cpp are how the parse tree is     /
/   formed and printed                                         */
/***************************************************************/
#include <stdlib.h>
#include <iostream>
#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "node.h"

using namespace std;

static Token tk;

/*  Tokens are added to nodes before scanning for the next token, to preserve the order of tokens. */

/*
	This function checks for an EOF token, when the EOF token is found parsing has completed.
*/
node* parser() {
	tk = scan();
	node* root = program();
	if (tk.tokenType == EOF_TOKEN) {
		return root;
	}
	else {
		cout << "A mistake was made here, no EOF token was found...";
	}
}

/* 
	<program> -> <vars> <block>
*/
node* program() {
	node* temp = newNode("program"); 
	
	temp->childOne = vars();
	// if childOne is null, use childOne in block()
	// mStat() does this same thing, because there is another *possible* empty production
	if (temp->childOne == NULL) {
		temp->childOne = block();
	}
	else {
		temp->childTwo = block();
	}
	return temp;
}

/* 
	<vars> -> empty | declare Identifier := Integer ; <vars>
*/
node* vars() {
	node* temp = newNode("vars");
	if (tk.tokenString.compare("declare") == 0) {
		temp->tkOne = tk;
		tk = scan();
		if (tk.tokenType == ID_TOKEN) {
			temp->tkTwo = tk;
			tk = scan();
			if (tk.tokenString.compare(":=") == 0) {
				temp->tkThree = tk;
				tk = scan();
				if (tk.tokenType == INT_TOKEN) {
					temp->tkFour = tk;
					tk = scan();
					if (tk.tokenString.compare(";") == 0) {
						temp->tkFive = tk;
						tk = scan();
						temp->childOne = vars();
						return temp;
					}
					else {
						errors("Integer", ";");
					}
				}
				else {
					errors(":=", "Integer");
				}		
			}
			else {
				errors("Identifier", ":=");
			}
		}
		else {
			errors("'declare'", "Identifier");
		}	
	}
	else {
		return NULL;
	}
}

/* 
	<block> -> { <vars> <stats> }
*/
node* block() {
	node* temp = newNode("block");

	if (tk.tokenString.compare("{") == 0) {
		temp->tkOne = tk;
		tk = scan();
		temp->childOne = vars();
		if (temp->childOne == NULL) {
			temp->childOne = stats();
		}
		else {
			temp->childTwo = stats();
		}
		if (tk.tokenString.compare("}") == 0) {
			temp->tkTwo = tk;
			tk = scan();
			return temp;
		}
		else {
			errors("{", "}");
		}
	}
	else {
		errors("a block", "{");
	}
}

/* 
	<stats> -> <stat> <mStat>
*/
node* stats() {
	node* temp = newNode("stats");
	temp->childOne = stat();
	temp->childTwo = mStat();
	return temp;
}

/*
	<mStat> -> empty | <stat> <mStat>
*/
node* mStat() {
	node* temp = newNode("mStat");

	if (tk.tokenString.compare("}") == 0) {
		//this is the empty production from <vars>
		return NULL;
	}
	else if (tk.tokenString.compare("{") == 0 ||
			tk.tokenString.compare("in") == 0 ||
			tk.tokenString.compare("out") == 0 ||
			tk.tokenString.compare("iffy") == 0 || 
			tk.tokenString.compare("loop") == 0 ||
			tk.tokenString.compare("label") == 0 ||
			tk.tokenString.compare("goto") == 0 || 
			tk.tokenType == ID_TOKEN) {
		temp->childOne = stat();
		temp->childTwo = mStat();
		return temp;
	}
	else {
		errors("}", "a new block");
	}
}

/*
	<stat> -> <in>; | <out>; | <block> | <if>; | <loop>; | <assign>; | <goto>; | <label>;
*/
node* stat() {
	string statementType = "";
	
	node* temp = newNode("stat");
	bool containsSemiColon = true;

	if (tk.tokenString.compare("in") == 0) {
		temp->childOne = in();
		statementType = "in";
	}
	else if (tk.tokenString.compare("out") == 0) {
		temp->childOne = out();
		statementType = "out";
	}
	else if (tk.tokenString.compare("{") == 0) {
		containsSemiColon = false;
		temp->childOne = block();
		return temp;
	}
	else if (tk.tokenString.compare("iffy") == 0) {
		temp->childOne = iffy();
		statementType = "iffy";
	}
	else if (tk.tokenString.compare("loop") == 0) {
		temp->childOne = loop();
		statementType = "loop";
	}
	else if (tk.tokenType == ID_TOKEN) {
		temp->childOne = assign();
		statementType = "assign";
	}
	else if (tk.tokenString.compare("goto") == 0) {
		temp->childOne = gotoMarkForHelp();
		statementType = "goto";
	}
	else if (tk.tokenString.compare("label") == 0) {
		temp->childOne = label();
		statementType = "label";
	}	
	else {
		errors("a statement", "the beginning of a statement");
	}

	if (tk.tokenString.compare(";") == 0 && containsSemiColon) {
		temp->tkOne = tk;
		tk = scan();
		return temp;
	}
	else if (containsSemiColon) {
		errors("a statement [type: "+statementType+"]", ";");
	}
	else {
		return temp;
	}	
}

/*
	<in> -> in Identifier
*/
node* in() {
	node* temp = newNode("in");	
	temp->tkOne = tk;
	tk = scan();
	if (tk.tokenType == ID_TOKEN) {
		temp->tkTwo = tk;
		tk = scan();
		return temp;
	}
	else {
		errors("'in' statement", "Identifier");
	}
}

/*
	<out> -> out <expr>
*/
node* out() {
	// add token to node HERE (before the scan()), set the pointer of the nodes child of this node to the expression
	node* temp = newNode("out");
	temp->tkOne = tk;
	tk = scan();
	temp->childOne = expr();
	return temp;
}

/*
	<if> -> iffy [ <expr> <RO> <expr> ] then <stat>
*/
node* iffy() {
	node* temp = newNode("iffy");
	// add the token to the node here (before the scan())
	temp->tkOne = tk;
	tk = scan();
	if (tk.tokenString.compare("[") == 0) {
		temp->tkTwo = tk;
		tk = scan();
		temp->childOne = expr();
		temp->childTwo = RO();
		temp->childThree = expr();
		if (tk.tokenString.compare("]") == 0) {
			temp->tkThree = tk;
			tk = scan();
			if (tk.tokenString.compare("then") == 0) {
				temp->tkFour = tk;
				tk = scan();
				temp->childFour = stat();
				return temp;
			}
			else {
				errors("'iffy' statement", "'then'");
			}
		}
		else {
			errors("'iffy' statement", "]");
		}
	}
	else {
		errors("'iffy' statement", "[");
	}
}

/* 
	<loop> -> loop [ <expr> <RO> <expr> ] <stat>
*/
node* loop() {
	node* temp = newNode("loop");
	temp->tkOne = tk;
	tk = scan();
	if (tk.tokenString.compare("[") == 0) {
		temp->tkTwo = tk;
		tk = scan();
		temp->childOne = expr();
		temp->childTwo = RO();
		temp->childThree = expr();
		if (tk.tokenString.compare("]") == 0) {
			temp->tkThree = tk;
			tk = scan();
			temp->childFour = stat();
			return temp;
		}
		else {
			errors("'loop' statement", "]");
		}
	}
	else {
		errors("'loop' statement", "[");
	}
}

/* 
	<assign> -> Identifier := <expr>
*/
node* assign() {
	node* temp = newNode("assign");
	temp->tkOne = tk;
	tk = scan();
	if (tk.tokenString.compare(":=") == 0) {
		temp->tkTwo = tk;
		tk = scan();
		temp->childOne = expr();
		return temp;
	}
	else {
		errors("'assign' statement", ":=");	
	}
}

/* 
	<label> -> label Identifier
*/
node* label() {
	node* temp = newNode("label");
	temp->tkOne = tk;
	tk = scan();
	if (tk.tokenType == ID_TOKEN) {
		temp->tkTwo = tk;
		tk = scan();
		return temp;
	}
	else {
		errors("'label' statement", "Identifier");
	}
}

/* 
	<goto> -> goto Identifier
*/
node* gotoMarkForHelp() {
	// add the token to the node
	node* temp = newNode("goto");
	temp->tkOne = tk;
	tk = scan();
	if (tk.tokenType == ID_TOKEN) {
		temp->tkTwo = tk;
		tk = scan();
		return temp;
	}
	else {
		errors("'goto' statement", "Identifier");
	}	
}

/* 
	<expr> -> <N> - <expr> | <N>
*/
node* expr() {
	node* temp = newNode("expr");
	temp->childOne = N();
	if (tk.tokenString.compare("-") == 0) {
		temp->tkOne = tk;
		tk = scan();
		temp->childTwo = expr();
		return temp;
	}
	else {
		return temp;
	}
}

/* 
	<N> -> <A> / <N> | <A> * <N> | <A>
*/
node* N() {
	node* temp = newNode("N");
	temp->childOne = A();
	if (tk.tokenString.compare("/") == 0) {
		temp->tkOne = tk;
		tk = scan();
		temp->childTwo = N();
		return temp;		
	}
	else if (tk.tokenString.compare("*") == 0) {
		temp->tkOne = tk;
		tk = scan();
		temp->childTwo = N();
		return temp;
	}
	else {
		return temp;
	}
}

/* 
	<A> -> <M> + <A> | <M>
*/
node* A() {
	node* temp = newNode("A");
	temp->childOne = M();
	if (tk.tokenString.compare("+") == 0) {
		temp->tkOne = tk;
		tk = scan();
		temp->childTwo = A();
		return temp;
	}
	else {
		return temp;
	}
}

/* 
	<M> -> * <M> | <R>
*/
node* M() {
	node* temp = newNode("M");
	if (tk.tokenString.compare("*") == 0) {
		temp->tkOne = tk;
		tk = scan();
		temp->childOne = M();
		return temp;
	}
	else {
		temp->childOne = R();
		return temp;
	}
}

/* 
	<R> -> ( <expr> ) | Identifier | Integer
*/
node* R() {
	node* temp = newNode("R");
	if (tk.tokenString.compare("(") == 0) {
		temp->tkOne = tk;
		tk = scan();
		temp->childOne = expr();
		if (tk.tokenString.compare(")") == 0) {
			temp->tkTwo = tk;
			tk = scan();
			return temp;
		}
		else {
			errors("an expression", ")");
		}
	}
	else if (tk.tokenType == ID_TOKEN) {
		temp->tkOne = tk;
		tk = scan();
		return temp;
	}
	else if (tk.tokenType == INT_TOKEN) {
		temp->tkOne = tk;
		tk = scan();
		return temp;
	}
	else {
		errors("an expression", "(, Identifier, or an Integer");	
	}
}

/* 
	<RO> -> < | << | > | >> | == | <>
*/
node* RO() {
	node* temp = newNode("RO");
	if (tk.tokenString.compare("<") == 0) {
		temp->tkOne = tk;
		tk = scan();
		if (tk.tokenString.compare("<") == 0) {
			temp->tkTwo = tk;
			tk = scan();
			return temp;
		}
		else if (tk.tokenString.compare(">") == 0) {
			temp->tkTwo = tk;
			tk = scan();
			return temp;
		}
		else {
			return temp;
		}
	}
	else if (tk.tokenString.compare(">") == 0) {
		temp->tkOne = tk;
		tk = scan();
		if (tk.tokenString.compare(">") == 0) {
			temp->tkTwo = tk;
			tk = scan();
			return temp;
		}
		else {
			return temp;
		}
	}
	else if (tk.tokenString.compare("==") == 0) {
		temp->tkOne = tk;
		tk = scan();
		return temp;
	}
	else {
		errors("relational operator", ">, <, or ==");
	}
}

/*
	Prints error messages, I added an exception case for relational operators since they can occur in iffy or loop statements.
*/
void errors(string message, string expected) {
	if (message.compare("relational operator") == 0) {
		cout << "PARSER ERROR: Missing " << message << " in the 'iffy' or 'loop' statement at line " << tk.lineNum << ".\n Expected ";
		cout << expected << ", got " << tk.tokenString << ".\n";
	}
	else {
		cout << "PARSER ERROR: Working on " << message << " at line " << tk.lineNum << ".\n Expected " << expected << ", got " << tk.tokenString << ".\n";
	}
	exit(1);
}
