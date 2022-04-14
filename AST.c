/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */

# include "parserDef.h"
#include "utils.h"
# include <stdlib.h>
# include <stdio.h>
#include "AST.h"

void nodeCountAST(parseTree root,int* ans)
{
	if(!root)
	return ;
	*ans=*ans+root->numChildAST;
	for(int i=0;i<root->numChildAST;i++)
	nodeCountAST(&(root->children[i]),ans);
}

int useful(int tokenClass) {
    switch(tokenClass) {
        case TK_MAIN:
        case TK_END:
        case TK_SEM:
        case TK_INPUT:
        case TK_PARAMETER:
        case TK_LIST:
        case TK_SQL:
        case TK_SQR:
        case TK_OUTPUT:
        case TK_COMMA:
        case TK_COLON:
	    case TK_TYPE:
        case TK_DOT:
        case TK_CALL:
        case TK_WITH:
        case TK_PARAMETERS:
        case TK_ASSIGNOP:
        case TK_WHILE:
        case TK_ENDWHILE:
        case TK_IF:
	    case TK_ELSE:
        case TK_ENDIF:
	    case TK_READ:
	    case TK_WRITE:
        case TK_OP:
        case TK_CL:
        case TK_RETURN:
        case TK_ENDRECORD:
        case eps:
            return 0;
            break;
        default:
            return 1;
    }
}

void copy(parseTree dst, parseTree src)
{
	dst->terminal = src->terminal;
	dst->nt = src->nt;
	dst->numChild = src->numChild;
	dst->children = src->children;

    // not a correct copy function as we are moving the pointers to terminal
    // and children nodes, not exactly copying but moving the pointers
    src->terminal = NULL;
    src->children = NULL;

	// dst->ruleNo = src->ruleNo;
	// dst->numChildAST = src->numChildAST;
	// dst->tp = NULL;
}

// void createASTUtils(parseTree curr, parseTree par)
// {
// 	if(curr==NULL)
// 	return ; 
// 	if(curr -> numChildAST == 0)
// 	{
// 		if(!useful(curr->terminal->tokenType))
// 		par->numChildAST--;
// 		return;
// 	}
// 	if(curr->numChildAST == 1 && curr->children[0].numChildAST == 0)
// 	{
// 		if(useful(curr->children[0].terminal->tokenType))
// 		{
// 			copy(curr,&(curr->children[0]));
// 			return;
// 		}
// 	}
	
// 	int count = curr->numChild;
// 	for(int i = 0; i < count; i++)
// 	createASTUtils(&(curr->children[i]),curr);
	
// 	if(curr->numChildAST == 0)
// 		par->numChildAST--;
// 	if(curr->numChildAST == 1 && curr->children[0].numChildAST == 0)
// 	{
// 		for(int i = 0; i < curr->numChild ;i++)
// 		{
// 			if(curr->children[i].numChildAST==0 && curr->children[i].nt==-1)
// 			{
// 				if(useful(curr->children[i].terminal->tokenType))
// 					copy(curr,&(curr->children[i]));
// 			}
// 		}
// 	}
// }

// void buildAST(parseTree ast,parseTree root)
// {
// 	if(!root)
// 	return;
// 	ast->children = malloc((root->numChildAST)*sizeof(parsetree));
// 	int m=0;
// 	for(int i = 0;i < root->numChild;i++) 
// 	{
// 		if(root->children[i].numChildAST!=0)
// 		{
// 			copy(&(ast->children[m]),&(root->children[i]));
// 			buildAST(&(ast->children[m]),&(root->children[i]));
// 			m++;
// 		}
// 		else
// 		{
// 			if(root->children[i].nt==-1 && useful(root->children[i].terminal->tokenType))
// 			{
// 				copy(&(ast->children[m]),&(root->children[i]));
// 				m++;
// 			}
// 		}
// 	}
// }

parseTree createASTDummy(parseTree root){
	if(root==NULL) return NULL;
	
	if(root->numChild > 1){
        // root is a non terminal, call the children recursively
		for(int i=0; i<root->numChild; i++) { 
			if(root->children[i].nt != -1) root->children[i] = *createASTDummy(&(root->children[i]));
			// } else{
			// 	if(useful(root->children[i].terminal->tokenType)){
			// 		continue;
			// 	} else{
			// 		root->numChild--;
			// 		free(&(root->children[i]));
			// 	}
			// }
		}
	} else if(root->numChild == 1 && root->children[0].nt!=-1){
        // root has only one child, which is a non terminal
		//change root to child of root and delete root
        parseTree oldChild = &root->children[0];
		copy(root, &(root->children[0]));
        free(oldChild);
		return createASTDummy(root->children);
	} else if(root->numChild == 1 && root->children[0].nt==-1){
        // root has only one child, which is a terminal
		if(useful(root->children[0].terminal->tokenType)){
			//change pointer to terminal
			copy(root, &(root->children[0]));
			free(root->children);
			return root;
		} else{
			root->numChild--;
			free(root->children);
			return root;
		}
	} else if(root->numChild == 0) return root;
	
	return root;
}
	
parseTree createAST(parseTree root)
{
    // isn't this just a redundant function
	// createASTUtils(root,NULL);
	// parseTree ast = malloc(sizeof(parsetree));
	// copy(ast, root);
	// buildAST(ast,root);
	// return ast;
	return createASTDummy(root);
}

void printAST(parseTree root, nonTerminal parent)
{
    // if the format is same as printParseTree, then its a redundant function
    if(root==NULL) return;

    if(root->nt==-1) {
        // terminal
        printf("Lexeme: %s, LineNo: %lld, TokenName: %s, ValueOfNumber: %s, parentNodeSymbol: %s, isLeafNode: YES, NodeSymbol: %s\n",
        root->terminal->lexeme, root->terminal->lineNum, root->terminal->lexeme, 
        ( (root->terminal->tokenType==TK_INT || root->terminal->tokenType==TK_REAL) ? root->terminal->lexeme : "----" ), 
        getNonTermString(parent), getTermString(root->terminal->tokenType));
        return;
    }

    for(int i=0; i<root->numChild; i++) {
        printParseTree(&root->children[i], root->nt);
        if(!i) {
            printf("Lexeme: ----, LineNo: ----, TokenName: %s, ValueOfNumber: ----, parentNodeSymbol: %s, isLeafNode: NO, NodeSymbol: %s\n",
            getNonTermString(root->nt), ( (parent==program) ? "ROOT" : getNonTermString(parent) ), getNonTermString(root->nt));
        }
    }

}
