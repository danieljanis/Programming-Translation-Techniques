#ifndef TOKEN_H
#define TOKEN_H

/***************************************************************/
/* Author: Daniel Janis                                         /
/  Course: CS 4280                                              /
/  File: token.h                                                /
/  Last Edit: 4/5/2020                                         */
/***************************************************************/

#include <string>

enum tokenOption {ID_TOKEN, INT_TOKEN, OP_TOKEN, EOF_TOKEN, KEYWORD};

struct Token {
        tokenOption tokenType;
        std::string tokenString;
        int lineNum;
};

#endif
