/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
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

typedef struct lhsChar{
	rhsChar* heads;
	int numRules;
	long long int first;
	long long int follow;
	unsigned int isNullable : 1;
} lhsChar;

typedef lhsChar* Grammar;

// typedef struct tableEntry
// {
// 	int nonTerm;
// 	int productionNum;
// 	int syn;
// }tableEntry;

// typedef tableEntry Table[NO_OF_NONTERMINALS][NO_OF_TERMINALS];

typedef struct parsetree
{	
	// int numChildAST;
	tokenInfo* terminal;
	// int ruleNo;
	// nt = -1 for leaf nodes
	int nt;
	int numChild;
	struct parsetree* children;
	// struct tablePointer* tp;
} parsetree;
typedef parsetree* parseTree;

#endif
