#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>

struct symbol {
	std::string name;
	std::string type;
	std::string scope;
};

bool variableExists(std::string);

void printSemantic(int, std::string, std::string);

#endif
