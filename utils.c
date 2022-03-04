/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */


#include "utils.h"

char *termArr[] = {"TK_ASSIGNOP", "TK_COMMENT", "TK_FIELDID", "TK_ID", "TK_NUM", "TK_RNUM", "TK_FUNID", "TK_RUID", 
"TK_WITH", "TK_PARAMETERS", "TK_END", "TK_WHILE", "TK_INT", "TK_REAL", "TK_TYPE", "TK_MAIN", "TK_GLOBAL", "TK_PARAMETER", 
"TK_LIST", "TK_SQL", "TK_SQR", "TK_INPUT", "TK_OUTPUT", "TK_SEM", "TK_COLON", "TK_DOT", "TK_COMMA", "TK_ENDWHILE", 
"TK_OP", "TK_CL", "TK_IF", "TK_THEN", "TK_ENDIF", "TK_READ", "TK_WRITE", "TK_RETURN", "TK_PLUS", "TK_MINUS", "TK_MUL", 
"TK_DIV", "TK_CALL", "TK_RECORD", "TK_ENDRECORD", "TK_ELSE", "TK_AND", "TK_OR", "TK_NOT", "TK_LT", "TK_LE", "TK_EQ", 
"TK_GT", "TK_GE", "TK_NE", "TK_UNION", "TK_ENDUNION", "TK_DEFINETYPE", "TK_AS", "TK_EOF", "eps", "dollar", "TK_ERROR"};

char *nonTermArr[] = {"program", "mainFunction", "otherFunctions", "function", "input_par", "output_par", "parameter_list", 
"dataType", "primitiveDatatype", "constructedDatatype", "remaining_list", "stmts", "typeDefinitions", "typeDefinition", 
"defineTypeStmt", "defineTypeStmts", "A", "fieldDefinition", "fieldDefinitions", "moreFields", "declarations", "declaration", 
"global_or_not", "otherStmts", "stmt", "assignmentStmt", "singleOrRecId", "option_single_constructed", "oneExpansion", 
"moreExpansions", "funCallStmt", "outputParameters", "inputParameters", "iterativeStmt", "conditionalStmt", "elsePart", 
"ioStmt", "arithmeticExpression", "expPrime", "term", "termPrime", "factor", "highPrecedenceOperators", "lowPrecedenceOperators", 
"booleanExpression", "var", "logicalOp", "relationalOp", "returnStmt", "optionalReturn", "idList", "more_ids"};

/*
matrix first{
{64, 32768}, 
{32768}, 
{64, 288230376151711744}, 
{64}, 
{2097152}, 
{4194304, 288230376151711744}, 
{4096, 8192, 2199023255552, 9007199254740992}, 
{4096, 8192, 2199023255552, 9007199254740992},
{4096, 8192},
{2199023255552, 9007199254740992},
{67108864, 288230376151711744},
{2199023255552, 9007199254740992, 16384, 8, 2048, 1073741824, 8589934592, 17179869184, 524288, 1099511627776, 34359738368, 36028797018963968},
{2199023255552, 9007199254740992, 288230376151711744},
{2199023255552, 9007199254740992},
{36028797018963968},
{36028797018963968, 288230376151711744},
{2199023255552, 9007199254740992},
{16384},
{16384},
{16384, 288230376151711744},
{16384, 288230376151711744},
{16384},
{16777216, 288230376151711744},
{8, 2048, 1073741824, 8589934592, 17179869184, 524288, 1099511627776, 288230376151711744},
{8, 2048, 1073741824, 8589934592, 17179869184, 524288, 1099511627776},
{8},
{8},
{33554432, 288230376151711744},
{33554432},
{33554432, 288230376151711744},
{524288, 1099511627776},
{524288, 288230376151711744},
{524288},
{2048},
{1073741824},
{8796093022208, 4294967296},
{8589934592, 17179869184},
{268435456, 8, 16, 32},
{68719476736, 137438953472, 288230376151711744},
{268435456, 8, 16, 32},
{288230376151711744, 274877906944, 549755813888},
{268435456, 8, 16, 32},
{274877906944, 549755813888},
{68719476736, 137438953472},
{268435456, 70368744177664, 8, 16, 32},
{8, 16, 32},
{17592186044416, 35184372088832},
{140737488355328, 281474976710656, 562949953421312, 1125899906842624, 2251799813685248, 4503599627370496},
{34359738368},
{524288, 288230376151711744},
{8},
{67108864, 288230376151711744}
};
*/

char* getTermString(terminal term) {
	return termArr[term];
}

char* getNonTermString(nonTerminal nonTerm) {
	return nonTermArr[nonTerm];
}

terminal checkTerminal(char *tokenArr) {
	terminal curr = TK_ASSIGNOP;
	for(int i=0; i<NO_OF_TERMINALS; i++, curr++) {
		if(strcmp(termArr[i], tokenArr)==0) return curr;
	}
	return -1;
	// fprintf(stderr, "%s - Terminal does not exist in ENUM\n", tokenArr);
	// exit(0);
}

nonTerminal checkNonTerminal(char *tokenArr) {
	nonTerminal curr = program;
	for(int i=0; i<NO_OF_NONTERMINALS; i++, curr++) {
		if(strcmp(nonTermArr[i], tokenArr)==0) return curr;
	}
	return -1;
	// fprintf(stderr, "%s - Non Terminal does not exist in ENUM\n", tokenArr);
	// exit(0);
}