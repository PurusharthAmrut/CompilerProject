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
struct tablePointer ; 
 
// #define NONTERMINAL_OFFSET 12345
#define NO_OF_NONTERMINALS 52
#define NONTERMINAL_OFFSET 12345
#define NO_OF_TERMINALS 60
#define MAX_RULE_LEN 10
#define NO_OF_PRODUCTIONS 90
#define MAX_ID_SIZE 30
#define RECORD_DATATYPE 50000

union symbol {
	terminal t;
	nonTerminal nt;
};
typedef union symbol* symbol;

struct rhsCharNode {
	symbol s;
	int tag;
	struct rhsCharNode* next;
};
typedef struct rhsCharNode* rhsCharNode;

struct rules {
	rhsCharNode head;
	int first;
};
typedef struct rules** Grammar;

typedef struct tableEntry
{
	int nonTerm;
	int productionNum;
	int syn;
}tableEntry;

typedef first* FirstSet;
typedef first* FollowSet;
typedef tableEntry Table[NO_OF_NONTERMINALS][NO_OF_TERMINALS];

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
