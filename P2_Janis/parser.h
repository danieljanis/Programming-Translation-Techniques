#ifndef PARSER_H
#define PARSER_H

/***************************************************************/
/* Author: Daniel Janis                                         /
/  Course: CS 4280                                              /
/  File: parser.h                                               /
/  Last Edit: 4/21/2020                                         /
/                                                              */
/***************************************************************/
#include <string>
#include "node.h"

node* parser();

node* program();

node* vars();

node* block();

node* stats();

node* mStat();

node* stat();

node* in();

node* out();

node* iffy();

node* loop();

node* assign();

node* label();

node* gotoMarkForHelp();

node* expr();

node* N();

node* A();

node* M();

node* R();

node* RO();

void errors(std::string, std::string);

#endif
