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
// #include "symbolTable.h"

/*========symbol table def=======================================================*/
typedef struct paramInfo {
	bool isInputPar;
	bool isOutputPar;
	int parIndex;
} paramInfo;

typedef struct record {
	char* rname;
	char* type;
	int lineNo;
	struct record* next;
} record;

typedef struct TableLoc {
	char* type;
	char* varname;
	int size;
	int offset;
	// If this entry is a record type
	record* ptr;
	// If this entry is a function parameter
	paramInfo* param;

	struct TableLoc *next;
} TableLoc;

typedef struct hashtable {
	// The hashtable is implemented as a linked list of TableLoc entries
	TableLoc* entry;
} hashtable;

typedef struct tableHeader {
	TableLoc** localTable;
	char* fname;
	int numVar;
	int fSize;
	int numInpPar;
	int numOutPar;
	record* inParList;
	record* outParList;
	record* variables;
} tableHeader;

typedef struct symboltable {
	tableHeader** fTable;
	int numFunc;
	record* functions;
} symboltable;

typedef symboltable* symbolTable;
/*===============================================================================*/

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
	tokenInfo* terminal;
	// nt = -1 for leaf nodes
	int nt;
	int numChild;
	struct parsetree* children;

	// int ruleNo;
	int numChildAST;
    tableHeader *tp;
} parsetree;
typedef parsetree* parseTree;

#endif
