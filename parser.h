/* 
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
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
void first(Grammar g, rhsCharNode rcn, long int* firstBitString);
void computeFirst(Grammar g, nonTerminal nt, long int* firstBitString);
void computeFollow(Grammar g, nonTerminal nt);
// void getFollowSets(Grammar g, FollowSet fw, FirstSet fr);
// void findFollow(int NonTerminalNo, int productionNo, Grammar g, FollowSet fw, FirstSet fr,int* flag);
// void createParseTable(FirstSet firstSet,FollowSet followSet,Grammar g,Table t);
void createParseTable(Grammar g, int[][] t);
// void add2(int* ans,int* add,int* flag);
// void parseInputSourceCode(FILE* sourceFile,Table t,Grammar g,parseTree root,int* error);
void parseInputSourceCode(FILE* sourceFile, int [][] t, Grammar g, parseTree root, int* error);
void printParseTree(parseTree root);


#endif
