#ifndef LEXERDEF
#define LEXERDEF

#include "utils.h"

//lookuptable structure
struct node{
	terminal type;
	char* lexeme;
	struct node* next;
};

typedef struct node NodeElement;

struct list{
	NodeElement* head;
};

typedef struct list List;

struct lookUp{
	int num_pos;
	List** positions;
};	

typedef struct lookUp LookUpTable;

typedef struct {
    char lexeme[MAX_LEXEME_SIZE];
    unsigned long long lineNum;
    terminal tokenType;
} tokenInfo;

#endif