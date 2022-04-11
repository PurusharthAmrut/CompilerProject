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

// void nodeCount(parseTree root,int* ans)
// {
// 	if(!root)
// 	return ;
// 	*ans=*ans+root->numChild;
// 	for(int i=0;i<root->numChild;i++)
// 	nodeCount(&(root->children[i]),ans);
// }

// void nodeCountAST(parseTree root,int* ans)
// {
// 	if(!root)
// 	return ;
// 	*ans=*ans+root->numChildAST;
// 	for(int i=0;i<root->numChildAST;i++)
// 	nodeCountAST(&(root->children[i]),ans);
// }

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

void copy(parseTree dst,parseTree src)
{
	dst->numChild = src->numChild;
	// dst->numChildAST = src->numChildAST;
	// dst->ruleNo = src->ruleNo;
	dst->terminal = src->terminal;
	dst->nt = src->nt;
	dst->children = src->children;
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
	if(!root){
		return NULL;
	}
	if(root->numChild > 1){
		for(int i=0; i<root->numChild; i++){
			//check if root is nt, if yes then call recursively, if no then check if it is useful, if useful then retain the node, otherwise free the node
			if(root->children[i].nt != -1){
				root->children[i] = *createASTDummy(&(root->children[i]));
			} else{
				if(useful(root->children[i].terminal->tokenType)){
					continue;
				} else{
					root->numChild--;
					free(&(root->children[i]));
				}
			}
		}
	} else if(root->numChild == 1 && root->children[0].nt!=1){
		//change root to child of root and delete root
		copy(root, &(root->children[0]));
		free(root->children);
		return createASTDummy(root->children);
	} else if(root->numChild == 1 && root->children[0].nt==-1){
		if(useful(root->children[0].terminal->tokenType)){
			//change pointer to terminal
			copy(root, &(root->children[0]));
			free(root->children);
			return root;
		} else{
			root->numChild--;
			free(root->children);
			return(root);
		}
	} else if(root->numChild == 0){
		return root;
	}
	return root;
}
	
parseTree createAST(parseTree root)
{
	// createASTUtils(root,NULL);
	// parseTree ast = malloc(sizeof(parsetree));
	// copy(ast,root);
	// buildAST(ast,root);
	// return ast;
	return createASTDummy(root);
}

void printAST(parseTree root)
{
	printf("Avengers Assemble!!!\n");
	if(root==NULL) return;

    if(root->nt==-1) {
        // terminal
        printf("Terminal: %s, Lexeme: %s, LineNum: %lld\n", getTermString(root->terminal->tokenType), 
        root->terminal->lexeme, root->terminal->lineNum);
        return;
    }

    printf("Non-Terminal: %s, Children: %d\n", getNonTermString(root->nt), root->numChild);
    for(int i=0; i<root->numChild; i++) {
        printf("%s \t\t Child %d: ", getNonTermString(root->nt), i+1);
        printAST(&root->children[i]);
    }
	// parseTree current;
	// int class;
	// for(int i=0;i<root->numChildAST;i++)
	// {
	// 	current=&(root->children[i]);
	// 	if(!current)
	// 	printf("NULL\n");
	// 	if(current->numChildAST==0 && current->terminal->tokenType==eps)
	// 	continue;
	// 	if(current->numChildAST>0)
	// 	{
	// 		printf("-------\t\t");
	// 		printf("-------\t\t");
	// 		printf("-------\t\t");
	// 		printf("-------\t\t");
	// 	}
	// 	else{
	// 		printf("%s\t\t",current->terminal->lexeme);
	// 		printf("%lld\t\t",current->terminal->lineNum);
	// 		printf("%s\t\t",tokenRepr(current->terminal->tokenType));
	// 		if(current->terminal->tokenType==TK_NUM || current->terminal->tokenType==TK_RNUM)
	// 			printf("%s\t\t",current->terminal->lexeme);
	// 		else printf("-------\t\t");	
	// 	}
	// 	printf("%s\t\t",idRepr(root->nt));
	// 	if(current->numChildAST==0)
	// 	{
	// 		printf("YES\t\t");
	// 		printf("-------\t\t");
	// 	}
	// 	else{
	// 		printf("NO\t\t");
	// 		printf("%s\t\t",idRepr(current->nt));
	// 	}
	// 	printf("\n");
	// 	printAST(current);
	// }
}
