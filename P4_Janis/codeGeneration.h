#ifndef CODEGENERATION_H
#define CODEGENERATION_H
#include <string>
#include "node.h"

typedef enum {VAR, LABEL} nameType;

std::string newName(nameType);

void generateAssemblyCode(std::string, node*);

void printToAsm(std::string, std::string);

void generateNodes(node*);

void vars(node*);

void out(node*);

void expr(node*);

void N(node*);

void A(node*);

void M(node*);

void R(node*);

void in(node*);

void iffy(node*);

void loop(node*);

void RO(node*, std::string);

void assign(node*);

void label(node*);

void gotoMarkForHelp(node*);

#endif
