/* 
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
 */
 
#ifndef PARSERDEF
#define PARSERDEF

#include <stdbool.h>
#include "lexerDef.h"
//#include "symbolTable.h"
struct tablePointer;
 
#define NONTERMINAL_OFFSET 12345
#define NO_OF_TERMINALS 59 //excluding TK_ERROR and dollar
#define N0_OF_NONTERMINALS 52
#define MAX_RULE_LEN 10
#define NO_OF_PRODUCTIONS 90
#define MAX_ID_SIZE 30
#define RECORD_DATATYPE 50000
#define MAX_TOKEN_LENGTH 30

typedef enum { 
	program,
	mainFunction,
	otherFunctions,
	function,
	input_par,
	output_par,
	parameter_list,
	dataType,
	primitiveDatatype,
	constructedDatatype,
	remaining_list,
	stmts,
	typeDefinitions,
	typeDefinition,
	defineTypeStmt,
	defineTypeStmts,
	A,
	fieldDefinition,
	fieldDefinitions,
	moreFields,
	declarations,
	declaration,
	global_or_not,
	otherStmts,
	stmt,
	assignmentStmt,
	singleOrRecId,
	option_single_constructed,
	oneExpansion,
	moreExpansions,
	funCallStmt,
	outputParameters,
	inputParameters,
	iterativeStmt,
	conditionalStmt,
	elsePart,
	ioStmt,
	arithmeticExpression,
	expPrime,
	term,
	termPrime,
	factor,
	highPrecedenceOperators,
	lowPrecedenceOperators,
	booleanExpression,
	var,
	logicalOp,
	relationalOp,
	returnStmt,
	optionalReturn,
	idList,
	more_ids
}nonTerminal;

char *nonTermArr[] = {"program", "mainFunction", "otherFunctions", "function", "input_par", "output_par", "parameter_list", 
"dataType", "primitiveDatatype", "constructedDatatype", "remaining_list", "stmts", "typeDefinitions", "typeDefinition", 
"defineTypeStmt", "defineTypeStmts", "A", "fieldDefinition", "fieldDefinitions", "moreFields", "declarations", "declaration", 
"global_or_not", "otherStmts", "stmt", "assignmentStmt", "singleOrRecId", "option_single_constructed", "oneExpansion", 
"moreExpansions", "funCallStmt", "outputParameters", "inputParameters", "iterativeStmt", "conditionalStmt", "elsePart", 
"ioStmt", "arithmeticExpression", "expPrime", "term", "termPrime", "factor", "highPrecedenceOperators", "lowPrecedenceOperators", 
"booleanExpression", "var", "logicalOp", "relationalOp", "returnStmt", "optionalReturn", "idList", "more_ids"};

typedef union symbol {
	terminal t;
	nonTerminal nt;
} symbol;

typedef struct rhsCharNode {
	symbol s;
	unsigned int tag : 1;
	struct rhsCharNode* next;
};

typedef struct rhsCharNode* rhsCharNode;

struct lhsChar {
	rhsCharNode* heads;
	int numRules;
	long int first;
	long int follow;
	unsigned int isNullable : 1;
};
typedef struct lhsChar* lhsChar;
typedef lhsChar Grammar;

typedef struct tableEntry
{
	int nonTerm;
	int productionNum;
	int syn;
}tableEntry;

typedef FirstSet* first;
typedef FollowSet* follow;
typedef tableEntry Table[NO_OF_NONTERMINALS][NO_OF_TERMINALS];

typedef struct parsetree
{
	int numChild;
	int numChildAST;
	tokenInfo* terminal;
	int ruleNo;
	int nonTerminal;
	struct parsetree* children;
	struct tablePointer* tp;
} parsetree;
typedef parsetree* parseTree;

#endif
