/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */
 



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parserDef.h"
#include "lexer.h"
#include "utils.h"
#include "Stack.h"

#define EXCLUDE_EPS -288230376151711745L
#define INCLUDE_EPS 288230376151711744L

bool check[NO_OF_NONTERMINALS];

void printGram(Grammar g) {
	for(int i=0; i<NO_OF_NONTERMINALS; i++) {
		printf("%d\n", g[i].numRules);
		for(int j=0; j<g[i].numRules; j++) {
			rhsChar curr = g[i].heads[j];
			while(curr!=NULL) {
				if(curr->tag==0) printf("%s ", getTermString(curr->s.t));
				else printf("%s ", getNonTermString(curr->s.nt));
				curr = curr->next;
			}
			printf("\n");
		}
	}
}

void getGram(char *fname, Grammar g) {
    FILE *fp;
    fp = fopen(fname, "r");
    if(fp==NULL) {
        fprintf(stderr, "File %s file not found error\n", fname);
        exit(0);
    }

    for(int i=0; i<NO_OF_NONTERMINALS; i++) {
        int numRules;
        fscanf(fp, "%d", &numRules);
        // printf("%d\n", numRules);

        g[i].heads = malloc(numRules*sizeof(rhsChar));
        g[i].numRules = numRules;
        g[i].first = 0;
        g[i].follow = 0;
		g[i].isNullable = 0;

        for(int j=0; j<numRules; j++) {
            rhsChar curr = malloc(sizeof(struct rhsCharNode));
            curr->next = NULL;

            char *nonTerm = malloc(MAX_TOKEN_LENGTH*sizeof(char));
            fscanf(fp, "%s", nonTerm);
            int numTokens;
            fscanf(fp, "%d", &numTokens);
            // printf("%s %d ", nonTerm, numTokens);

            curr->tag = 1;
            curr->s.nt = checkNonTerminal(nonTerm);
            if(curr->s.nt==-1) {
                fprintf(stderr, "%s - Non Terminal does not exist in ENUM\n", nonTerm);
				exit(0);
            }

			if(curr->s.nt==eps) g[i].isNullable = 1;

            rhsChar prev = curr;

            for(int k=0; k<numTokens; k++) {
                rhsChar tmp = malloc(sizeof(struct rhsCharNode));
                tmp->next = NULL;

                char *token = malloc(MAX_TOKEN_LENGTH*sizeof(char));
                fscanf(fp, "%s", token);
                // printf("%s ", token);

                int tokenIdx = checkTerminal(token);
                if(tokenIdx!=-1) {
                    tmp->tag = 0;
                    tmp->s.t = tokenIdx;
                }else {
                    tokenIdx = checkNonTerminal(token);
                    if(tokenIdx==-1) {
                        fprintf(stderr, "%s - Non Terminal does not exist in ENUM\n", token);
						exit(0);
                    }
                    tmp->tag = 1;
                    tmp->s.nt = tokenIdx;
                }

                prev->next = tmp;
                prev = tmp;
            }
            g[i].heads[j] = *curr;

            // printf("\n");
        }
    }

	printf("Loaded Grammar!!!\n");

    fclose(fp);
}
//------------------------------------------------------------
//FIRST and FOLLOW

void first(Grammar g, rhsChar rcn, long int* firstBitString) {
    // this funcyion assumes that the caller has already checked whether
    // the first set for nt has been previosly computed before calling this function	
    if (rcn->tag==0) { //symbol is a terminal
        *firstBitString = *firstBitString | 1<<(rcn->s.t);
    } else { //symbol is a non-terminal => further digging needed
        lhs lc = g[rcn->s.nt];
        if (lc->first == 0) { //first set isn't initialized yet
            computeFirst(g, rcn->s.nt);
        }
        
        if (lc->isNullable) {
            *firstBitString = *firstBitString | (lc->first & EXCLUDE_EPS); //exclude eps
            if (rcn->next != NULL)
                first(g, rcn->next, firstBitString);
            else
                //This was the last symbol in the rule
                *firstBitString = *firstBitString | INCLUDE_EPS;            
        } else {
            //no need to compute first sets of further nonTerminals in the rule
            *firstBitString = *firstBitString | (lc->first);
        }
    }
    return;
}

void computeFirst(Grammar g, nonTerminal nt) {
    //this function assumes
    lhs lc = g[nt];
    for (int i=0; i<lc->numRules; i++) {
        first(g, lc->heads[i], &(lc->first));
    }
}

void computeFollow(Grammar g, nonTerminal nt) {
    // the function assumes that the caller has first checked if the follow set
    // of nt has already been computed
    rhsChar rcn, nextrcn;
    long int firstOfNext;
    for (int i=0; i<NO_OF_NONTERMINALS; i++) {
        for (int j=0; j<g[i].numRules; j++) {
            rcn = g[i].heads[j];
            while (rcn!=NULL) {
                if (rcn->tag==1 && rcn->s.nt==nt) {
                    if (rcn->next !=NULL) {
                        nextrcn = rcn->next;
                        first(g, nextrcn, &firstOfNext);
                        if ((firstOfNext>>eps) & 1 == 1) {
                            // first set of subsequent string contains eps
                            firstOfNext = firstOfNext & EXCLUDE_EPS;
                            g[nt].follow = g[nt].follow | firstOfNext;
                            if (i!=nt) // if the nonTerminal on LHS is different than nt
                            {
                                if (g[i].follow == 0)
                                    computeFollow(g, i);
                                g[nt].follow = g[nt].follow | g[i].follow;                             
                            }
                        } else {
                            g[nt].follow = g[nt].follow | firstOfNext;
                        }
                    } else {
                        //This is the last character in the RHS of the production
                        if (i!=nt) // if the nonTerminal on LHS is different than nt
                        {
                            if (g[i].follow == 0)
                                computeFollow(g, i);
                            g[nt].follow = g[nt].follow | g[i].follow;                             
                        }
                    }
                }
                rcn = rcn->next;
            }
        }
    }
}


void createParseTable(Grammar g, int t[][]) {
	for(int i = 0; i < NO_OF_NONTERMINALS; i++)
        for(int j = 0; j < NO_OF_TERMINALS; j++)
        {
        	t[i][j] = -1;
        }

	int shiftFirst;
	int shiftFollow;
	long int first;
	long int follow;
	for (int i=0; i<NO_OF_NONTERMINALS; i++) {
		for (int j=0; j<g[i]->numRules; j++) {
			first = g[i].first;
			follow = g[i].follow;
			for (shiftFirst = 0; shiftFirst<NO_OF_TERMINALS; shiftFirst++) {
				if ((first>>shiftFirst) & 1 == 1) {
					t[i][shiftFirst] = j;
				}
			}
			if((first>>eps) & 1 == 1){
				for (shiftFollow= 0; shiftFollow<NO_OF_TERMINALS; shiftFollow++)
				{
					if((follow>>shiftFollow) & 1 == 1){
						t[i][shiftFollow] = j;
					}
				}
			}
			if(((first>>eps) & 1 == 1) && ((follow>>dollar)  & 1 == 1)){
				t[i][dollar] = j;
			}
		}
	}
}

// void parseInputSourceCode(FILE* sourceFile,Table t,Grammar g,parseTree root,int* error)
// {
// 	root->nonTerminal=program;
// 	root->numChild=2;
// 	parseTree leaf=NULL,parent=NULL,current;
// 	Stack stack=newStack();
// 	tokenInfo token ;
// 	push(stack,TK_EOF,leaf);
// 	push(stack,program,root);
// 	int flag=1,terminal,nonTermID,productionNo,ruleLen;
// 	Key top;
// 	int* rule;
// 	do{
// 		if(flag)
// 		getNextToken(sourceFile,&token);
// 		if(token.tokenType==TK_ERROR)
// 		{
// 			printf("\n\nLEXICAL ERROR AT LINE NO: %lld   %s\n",token.lineNum,token.lexeme);
// 			//if(top1(stack)->parent->id < NO_OF_TERMINALS)
// 			//{
// 				pop(stack);
// 				continue;
// 			///}
// 		}
// 		if(token.tokenType==TK_COMMENT)
// 		continue;
// 		if(token.tokenType==TK_EOF)
// 		break ;
// 		flag=0 ;
// 		top=top1(stack) ;
// 		terminal=token.tokenType ;
// 		current=top->parent;
// 		if(top->id<NO_OF_TERMINALS)
// 		{
// 			if(top->id!=terminal && top->id!=eps)
// 			{
// 				printf("\n\nPARSER ERROR AT LINE NO: %lld The token %s for lexeme %s does not match with the expected token %s\n",token.lineNum,tokenRepr(terminal),token.lexeme,tokenRepr(top->id));
// 				*error=1;
// 				pop(stack);
// 			}
// 			else{
// 				pop(stack);
// 				current->terminal->lineNum=token.lineNum;
// 				current->terminal->tokenType=top->id;
// 				if(top->id==eps)
// 				{
// 					continue;
// 				}
// 				int lenn=0;
// 				while(token.lexeme[lenn]!='\0')
// 				{
// 					current->terminal->lexeme[lenn]=token.lexeme[lenn];
// 					lenn++;
// 				}
// 				for(int i=lenn;i<MAX_LEXEME_SIZE;i++)
// 				current->terminal->lexeme[i]='\0';
// 				flag=1;
// 			}
// 		}
// 		else{
// 			nonTermID=t[top->id-NONTERMINAL_OFFSET][terminal].nonTerm;
// 			productionNo=t[top->id-NONTERMINAL_OFFSET][terminal].productionNum;
// 			if(nonTermID==-1 || productionNo==-1)
// 			{
// 				printf("\n\nPARSING ERROR AT LINE NO: %lld\n",token.lineNum);
// 				*error =1;
// 				getNextToken(sourceFile,&token);
// 				if(token.tokenType==TK_ERROR)
// 				printf("\n\nERROR2:LINE NO: %lld\n",token.lineNum);
// 				while(token.tokenType!=TK_EOF && t[top->id-NONTERMINAL_OFFSET][token.tokenType].syn==-1)
// 				{
// 					getNextToken(sourceFile,&token);
// 					if(token.tokenType==TK_ERROR)
// 					printf("\n\nERROR2:LINE NO: %lld\n",token.lineNum);
// 				}
				
// 				if(token.tokenType==TK_EOF)
// 					return;
				
// 				if(t[top->id-NONTERMINAL_OFFSET][token.tokenType].syn==1)
// 				pop(stack);
// 				continue;
// 			}
// 			rule=g[nonTermID].rules[productionNo];
// 			ruleLen=rule[0];
// 			current->children = malloc(ruleLen*sizeof(parsetree));
// 			current->numChild = ruleLen;
// 			current->numChildAST = ruleLen;
// 			current->nonTerminal=top->id;
			
// 			for(int i=1;i<=ruleLen;i++)
// 			{
// 				if(rule[i]<NO_OF_TERMINALS)
// 				{
// 					current->children[i-1].terminal=malloc(sizeof(tokenInfo));
// 					current->children[i-1].numChild=0 ;
// 					current->children[i-1].numChildAST=0;
// 					current->children[i-1].nonTerminal=-1;
// 					current->children[i-1].children=NULL;
// 					current->children[i-1].tp = NULL;
// 				}
// 				else{
// 					current->children[i].nonTerminal=rule[i];
// 				}
// 			}
// 			pop(stack);
// 			for(int i=ruleLen;i>0;i--)
// 			{
// 				//if(rule[i]!=eps)
// 				push(stack,rule[i],&(current->children[i-1]));
// 			}
// 		}
// 	} while(token.tokenType!=TK_EOF);
// }

void parseInputSourceCode(FILE* sourceFile, int t[][], Grammar g, parseTree root, int* error){
	Stack parseStack=newStack();
	Stack tempStack = newStack();
	tokenInfo token;
	parseTree leaf=NULL, current, parent;
	Key start = newKey(program, 1, root);
	Key end = newKey(dollar, 1, NULL);
	push(parseStack, end);
	push(parseStack, start);
	Key k, child, temp;
	int productionIndex, ruleLen, i;
	rhsChar rcn;
	do {
		getNextToken(sourceFile, &token);
		k = top1(parseStack);
		//top of stack is a nonTerminal
		if (k->tag==1) {		
			if(t[(k->id).nt][token.tokenType] > 0) {
				//this parsetree node is now going to expand and
				//bear children, hence naming it parent :)
				parent = k->subtree;
				parent->nt = (k->id).nt;
				productionIndex = t[(k->id).nt][token.tokenType];
				pop(parseStack);
				rcn = g[(k->id).nt].heads[productionIndex];
				ruleLen = 0;
				while (rcn->next != NULL) {
					//currently, there is no parsetree corresponding
					//to this ky object
					//pushing into temporary stack for order reversal
					push(tempStack, newKey(rcn->s, rcn->tag, NULL));
					ruleLen++;
					rcn = rcn->next;
				}
				parent->children = (parsetree*)malloc(ruleLen*sizeof(parsetree));
				for (int i=ruleLen-1; i>=0; i--) {
					child = top1(tempStack);
					pop(tempStack);
					//storing the location of this node
					//(in the parsetree) in the key object
					child->subtree = &(parent->children)[i];
					//populating the parsetree data structure
					//using the key obtained from the stack
					if (child->tag == 0) {
						(parent->children)[i].terminal = child->id.t;
						(parent->children)[i].numChild = 0;
						(parent->children)[i].nt = -1;
						(parent->children)[i].children = NULL;
					} else
						(parent->children)[i].nt = child->id.nt;
					push(parseStack, child);
				}
			} else {
				//enter recovery mode
				*error = 1;
			}
		} else {
			if (k->id.t == token.tokenType)
				pop(parseStack);
			else {
				//enter recovery mode
				*error = 1;
			}				
		}
	} while (token.tokenType != TK_EOF);

}

void printParseTree(parseTree root)
{
	parseTree current;
	int class;
	for(int i=0;i<root->numChild;i++)
	{
		current=&(root->children[i]);
		if(!current)
		printf("NULL\n");
		if(current->numChild==0 && current->terminal->tokenType==eps)
		continue;
		if(current->numChild>0)
		{
			printf("-------\t\t");
			printf("-------\t\t");
			printf("-------\t\t");
			printf("-------\t\t");
		}
		else{
			printf("%s\t\t",current->terminal->lexeme);
			printf("%lld\t\t",current->terminal->lineNum);
			printf("%s\t\t",tokenRepr(current->terminal->tokenType));
			if(current->terminal->tokenType==TK_NUM || current->terminal->tokenType==TK_RNUM)
				printf("%s\t\t",current->terminal->lexeme);
			else printf("-------\t\t");	
		}
		printf("%s\t\t",idRepr(root->nonTerminal));
		if(current->numChild==0)
		{
			printf("YES\t\t");
			printf("-------\t\t");
		}
		else{
			printf("NO\t\t");
			printf("%s\t\t",idRepr(current->nonTerminal));
		}
		printf("\n");
		printParseTree(current);
	}
}

