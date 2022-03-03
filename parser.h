/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */
 


#ifndef PARSER
#define PARSER

#include "parserDef.h"
#include "lexerDef.h"

// int* calculateFirst(int production, Grammar g, FirstSet firstSet);
// int* add(int* ans, int* addit);
// void buildFirstSet(Grammar g, FirstSet firstSet);
char* getTermString(terminal term);
char* getNonTermString(nonTerminal nonTerm);
terminal checkTerminal(char *tokenArr);
nonTerminal checkNonTerminal(char *tokenArr);
void printGram(Grammar g);
void getGram(char *fname, Grammar g);
void first(Grammar g, rhsChar rcn, long int* firstBitString);
void computeFirst(Grammar g, nonTerminal nt);
void computeFollow(Grammar g, nonTerminal nt);
// void getFollowSets(Grammar g, FollowSet fw, FirstSet fr);
// void findFollow(int NonTerminalNo, int productionNo, Grammar g, FollowSet fw, FirstSet fr,int* flag);
// void createParseTable(FirstSet firstSet,FollowSet followSet,Grammar g,Table t);
void createParseTable(Grammar g, int t[NO_OF_NONTERMINALS][NO_OF_TERMINALS+1]);
// void add2(int* ans,int* add,int* flag);
// void parseInputSourceCode(FILE* sourceFile,Table t,Grammar g,parseTree root,int* error);
void parseInputSourceCode(FILE* sourceFile, int t[NO_OF_NONTERMINALS][NO_OF_TERMINALS+1], Grammar g, parseTree root, int* error);
void printParseTree(parseTree root);


#endif
