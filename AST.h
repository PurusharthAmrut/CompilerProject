#include "parser.h"

void nodeCountAST(parseTree root,int* ans);
int useful(int node);
void copy(parseTree dst,parseTree src);
parseTree createAST(parseTree root);
void printAST(parseTree root, nonTerminal parent);
void printASTDetails(parseTree ast);