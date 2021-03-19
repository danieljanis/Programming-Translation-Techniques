/***************************************************************/
/* Author: Daniel Janis                                         /
/  Course: CS 4280                                              /
/  File: table.cpp                                              /
/  Last Edit: 4/5/2020                                          /
/                                                               /
/        This table, states, character types, and functions are     /
/    used in driver.cpp (setState() and newState())            */
/***************************************************************/

#include <ctype.h>
#include "table.h"

/* States */
static const int INITIAL = 0;
static const int IDENTIFIER = 1;
static const int INTEGER = 2;
static const int OPERATOR = 3;
static const int COLON = 4;
static const int EQUAL = 5;

/* Tokens */
static const int ID_TOKEN = 1000;
static const int INT_TOKEN = 1001;
static const int OP_TOKEN = 1002;

/* Not in alphabet errors */
static const int ALPH_ERR = -1;
static const int ALPH_ID_ERR = -2;
static const int ALPH_INT_ERR = -3;
static const int ALPH_OP_ERR = -4;

/* Equal sign errors */
static const int EQ_LETTER_ERR = -5;
static const int EQ_DIGIT_ERR = -6;
static const int EQ_OP_ERR = -7;
static const int EQ_WS_ERR = -8;
static const int EQ_COMMENT_ERR = -9;

/* Character types */
static const int NOT_ALPH = 0;
static const int LETTER_CHAR = 1;
static const int DIGIT_CHAR = 2;
static const int OP_CHAR = 3;
static const int COLON_CHAR = 4;
static const int EQUAL_CHAR = 5;
static const int WS_CHAR = 6;
static const int COMMENT = 7;

/* FSA table */
const int table[6][8] = {
        {ALPH_ERR, IDENTIFIER, INTEGER, OPERATOR, COLON, EQUAL, INITIAL, INITIAL},
        {ALPH_ID_ERR, IDENTIFIER, IDENTIFIER, ID_TOKEN, ID_TOKEN, ID_TOKEN, ID_TOKEN, ID_TOKEN},
        {ALPH_INT_ERR, INT_TOKEN, INTEGER, INT_TOKEN, INT_TOKEN, INT_TOKEN, INT_TOKEN, INT_TOKEN},
        {ALPH_OP_ERR, OP_TOKEN, OP_TOKEN, OP_TOKEN, OP_TOKEN, OP_TOKEN, OP_TOKEN, OP_TOKEN},
        {ALPH_OP_ERR, OP_TOKEN, OP_TOKEN, OP_TOKEN, OP_TOKEN, OPERATOR, OP_TOKEN, OP_TOKEN},
        {ALPH_OP_ERR, EQ_LETTER_ERR, EQ_DIGIT_ERR, EQ_OP_ERR, EQ_OP_ERR, OPERATOR, EQ_WS_ERR, EQ_COMMENT_ERR}
};

/* Always start at INITIAL (0) state */
static int state = INITIAL;

/* Used for initializing the current state in driver() from driver.cpp */
int setState(char ch) {
        int col = whichCharacter(ch);
        state = table[state][col];
        return state;
}

/* Every state after the initial state */
int newState(char ch) {
        int col = whichCharacter(ch);
        int newState = table[state][col];
        /* if newState is a valid token, reset state to the initial state */
        if (newState >= ID_TOKEN) {
                state = INITIAL;
        }
        return newState;
}

/* Returns which type of character ch was */
int whichCharacter(char ch) {
        if ((ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122)) {
                return LETTER_CHAR;
        }
        if (ch >= 48 && ch <= 57) {
                return DIGIT_CHAR;
        }
        if (ch == 37 || (ch <= 47 && ch >= 40) || ch == 59 || ch == 60 || ch == 62 || ch == 91 || ch == 93 || ch == 123 || ch == 125) {
                return OP_CHAR;
        }
        if (ch == 58) {
                return COLON_CHAR;
        }
        if (ch == 61) {
                return EQUAL_CHAR;
        }
        if (isspace(ch)) {
                return WS_CHAR;
        }
        if (ch == 35) {
                return COMMENT;
        }
        return NOT_ALPH;
}
