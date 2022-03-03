/* 
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
 */
 
#ifndef PARSERDEF
#define PARSERDEF

#include "lexerDef.h"
//#include "symbolTable.h"
struct tablePointer;

typedef union symbol {
	terminal t;
	nonTerminal nt;
} symbol;

struct rhsCharNode {
	symbol s;
	unsigned int tag : 1;
	struct rhsCharNode* next;
};

typedef struct rhsCharNode* rhsChar;

struct lhsChar{
	rhsChar heads;
	int numRules;
	long int first;
	long int follow;
	unsigned int isNullable : 1;
};
typedef struct lhsChar* lhs;
typedef lhs Grammar;

// typedef struct tableEntry
// {
// 	int nonTerm;
// 	int productionNum;
// 	int syn;
// }tableEntry;

// typedef tableEntry Table[NO_OF_NONTERMINALS][NO_OF_TERMINALS];

typedef struct parsetree
{
	int numChild;
	tokenInfo* terminal;
	int ruleNo;
	int nonTerminal;
	struct parsetree* children;
	struct tablePointer* tp;
} parsetree;
typedef parsetree* parseTree;

#endif
