/* 
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
 */
 



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parserDef.h"
#include "lexer.h"
#include "utils.h"
#include "Stack.h"

bool check[NO_OF_NONTERMINALS];

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

void printGram(Grammar g) {
	for(int i=0; i<NO_OF_NONTERMINALS; i++) {
		printf("%d\n", g[i].numRules);
		for(int j=0; j<g[i].numRules; j++) {
			rhsCharNode curr = g[i].heads[j];
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
    fp = fopen(filepath, "r");
    if(fp==NULL) {
        fprintf(stderr, "File %s file not found error\n", fname);
        exit(0);
    }

    for(int i=0; i<NO_OF_NONTERMINALS; i++) {
        int numRules;
        fscanf(fp, "%d", &numRules);
        // printf("%d\n", numRules);

        g[i].heads = malloc(numRules*sizeof(rhsCharNode));
        g[i].numRules = numRules;
        g[i].first = 0;
        g[i].follow = 0;
		g[i].isNullable = 0;

        for(int j=0; j<numRules; j++) {
            rhsCharNode curr = malloc(sizeof(struct rhsCharNode));
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

            rhsCharNode prev = curr;

            for(int k=0; k<numTokens; k++) {
                rhsCharNode tmp = malloc(sizeof(struct rhsCharNode));
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
            g[i].heads[j] = curr;

            // printf("\n");
        }
    }

	printf("Loaded Grammar!!!\n");

    fclose(fp);
}
//------------------------------------------------------------

int* add(int* ans, int* addit){
	for(int i=0;i<=NO_OF_TERMINALS;i++)
	if(addit[i] && i!=eps) ans[i]=1;
	return ans;
}

int* calculateFirst(int production,Grammar g,FirstSet firstSet)
{
	int* ans=(int*)malloc(NO_OF_TERMINALS*sizeof(int));
	int* temp;
	nonTerminal nonTermSpec=g[production];
	int** arr=nonTermSpec.rules;
	int arrSize=nonTermSpec.numRules;
	for(int i=0;i<arrSize;i++)
	{
		if(arr[i][1]<NO_OF_TERMINALS)
		 	ans[arr[i][1]]=1;
		else{
			int m=1;
			while(m<=arr[i][0])
			{
				if(arr[i][m]<NO_OF_TERMINALS)
				{
					ans[arr[i][m]]=1;
					break;
				}
				if(!check[arr[i][m]-NONTERMINAL_OFFSET])
				{
					temp=calculateFirst(arr[i][m]-NONTERMINAL_OFFSET,g,firstSet);
					(firstSet[arr[i][m]-NONTERMINAL_OFFSET]).first=temp;
					check[arr[i][m]-NONTERMINAL_OFFSET]=1;
				}
				ans=add(ans,firstSet[arr[i][m]-NONTERMINAL_OFFSET].first);
				if(!temp[eps])
				break;
				m++;
			}
			if(m>arr[i][0])
			ans[eps]=1;
		}
	}
	firstSet[production].first=ans;
	check[production]=1;
	return ans;
}
		
void buildFirstSet(Grammar g,FirstSet firstSet)
{
	int* useless;
	for(int i=0;i<NO_OF_NONTERMINALS;i++)
	if(!check[i])
	useless=calculateFirst(i,g,firstSet);
}

//-----------------------------------------------------------------------------------------------------

void firstString(int* beta,int* firstRule,int index,FirstSet firstSet)
{
	int flag=0;
	for(int k = 0; k < index; k++)
	{
		if(beta[k] < NO_OF_TERMINALS)
		{
	       		firstRule[beta[k]] = 1;
		        break;
		}
        	else{
      			int* temp = firstSet[beta[k] - NONTERMINAL_OFFSET].first;
      			if(temp[eps] == 0)
      			{
      				firstRule = add(firstRule, temp);
      				break;
      			}	
      			else{
      				firstRule = add(firstRule, temp);
      				if(k == index-1){flag = 1;}
      			}
		} 
	}
        if(flag == 1)
        firstRule[eps] = 1;
}

void add2(int* ans,int* add,int* flag)
{
	for(int i=0;i<NO_OF_TERMINALS;i++)
	{
		if(add[i] && i!=eps)
		{
			if(!ans[i])
			*flag=1;
			ans[i]=1;
		}
	}
}

void findFollow(int nonTerminalNo, int productionNo, Grammar g, FollowSet followSet, FirstSet firstSet,int* flag2)
{
	int** rules=g[nonTerminalNo].rules;
	int* rule=rules[productionNo];
	int *nextFirst,*temp,*temp2,*temp3;
	int ruleLen=rule[0];
	int flag=0,flag3=0;
	for(int i=1;i<=ruleLen;i++)
	{
		flag=0;
		if(rule[i]<NO_OF_TERMINALS)
		continue;
		int* firstRule=malloc(sizeof(int)*NO_OF_TERMINALS);
		if(i==ruleLen)
		flag=1;
		else
		{
			flag3=0;
			for(int j=0;j<NO_OF_TERMINALS;j++)
			firstRule[j]=0;
			int index=0;
			int beta[MAX_RULE_LEN];
			for(int k=i+1;k<=ruleLen;k++)
			beta[index++]=rule[k];
			firstString(beta,firstRule,index,firstSet);
			temp=followSet[rule[i]-NONTERMINAL_OFFSET].first;
			add2(temp,firstRule,&flag3);
			if(!*flag2) *flag2=flag3;
			followSet[rule[i]-NONTERMINAL_OFFSET].first=temp;
		}
		if(firstRule[eps] || flag)
		{
			for(int j=0;j<NO_OF_TERMINALS;j++)
			{
				temp3=followSet[rule[i]-NONTERMINAL_OFFSET].first;
				if(followSet[nonTerminalNo].first[j])
				{
					if(!temp3[j])
					{
						*flag2=1;
					}
					temp3[j]=1;
				}
			}
		}
	}
}
		
void getFollowSets(Grammar g, FollowSet fw, FirstSet fr){
	for(int i=0;i<NO_OF_NONTERMINALS;i++)
	{
		fw[i].first=malloc(sizeof(int)*NO_OF_TERMINALS);
		for(int j=0;j<NO_OF_TERMINALS;j++)
		fw[i].first[j]=0;
	}
	fw[0].first[TK_EOF] = 1;
	int flag;
	while(1){
		flag=0;
		for(int i = 0; i < NO_OF_NONTERMINALS; i++){
			for(int j = 0; j < g[i].numRules; j++){
				findFollow(i, j, g, fw, fr,&flag);
			}
		} 
		if(flag==0) break;
	}        
}

// void createParseTable(FirstSet firstSet,FollowSet followSet,Grammar g,Table t)
// {
// 	int beta[MAX_RULE_LEN];
// 	int** rules;
// 	int* rule;
// 	int ruleNumber=0;
// 	// intitializing the parse table
// 	for(int i = 0; i < NO_OF_NONTERMINALS; i++)
//         for(int j = 0; j < NO_OF_TERMINALS; j++)
//         {
//         	t[i][j].nonTerm = -1;
//         	t[i][j].productionNum= -1;
//         	t[i][j].syn=-1;
//         }	
// 	for(int i=0;i<NO_OF_NONTERMINALS;i++)
// 	{
// 		nonTerminal nonTerm=g[i];
// 		rules=nonTerm.rules;
// 		int* followRule=followSet[i].first;
// 		for(int j=0;j<nonTerm.numRules;j++)
// 		{
// 			rule=rules[j];
// 			int* firstRule=malloc(sizeof(int)*NO_OF_TERMINALS);
// 			int index=0,flag=0;
// 			for(int k=1;k<=rule[0];k++)
// 				beta[index++]=rule[k];
// 			firstString(beta,firstRule,index,firstSet);
			
// 			for(int k=0;k<NO_OF_TERMINALS;k++)
// 				if(firstSet[i].first[k])
// 					t[i][k].syn=0;	
					
// 			for(int k=0;k<NO_OF_TERMINALS;k++)
// 				if(followSet[i].first[k])
// 					t[i][k].syn=1;	
			
// 		        for(int k=0;k<NO_OF_TERMINALS;k++)
// 		        if(firstRule[k] && k!=eps)
// 		        {
// 		        	t[i][k].nonTerm = i;
//         			t[i][k].productionNum= j;
//         		}
// 		        if(firstRule[eps])
// 		        {
// 		        	for(int k=0;k<NO_OF_TERMINALS;k++)
// 		        	if(followRule[k] && k!=eps)
// 		        	{
// 					t[i][k].nonTerm = i;
// 					t[i][k].productionNum= j;
//         			}
// 		        }
// 		        ruleNumber++;
// 		}
// 	}
// }

void createParseTable(Grammar g, int[][] t) {
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

void parseInputSourceCode(FILE* sourceFile,Table t,Grammar g,parseTree root,int* error)
{
	root->nonTerminal=program;
	root->numChild=2;
	parseTree leaf=NULL,parent=NULL,current;
	Stack stack=newStack();
	tokenInfo token ;
	push(stack,TK_EOF,leaf);
	push(stack,program,root);
	int flag=1,terminal,nonTermID,productionNo,ruleLen;
	Key top;
	int* rule;
	do{
		if(flag)
		getNextToken(sourceFile,&token);
		if(token.tokenType==TK_ERROR)
		{
			printf("\n\nLEXICAL ERROR AT LINE NO: %lld   %s\n",token.lineNum,token.lexeme);
			//if(top1(stack)->parent->id < NO_OF_TERMINALS)
			//{
				pop(stack);
				continue;
			///}
		}
		if(token.tokenType==TK_COMMENT)
		continue;
		if(token.tokenType==TK_EOF)
		break ;
		flag=0 ;
		top=top1(stack) ;
		terminal=token.tokenType ;
		current=top->parent;
		if(top->id<NO_OF_TERMINALS)
		{
			if(top->id!=terminal && top->id!=eps)
			{
				printf("\n\nPARSER ERROR AT LINE NO: %lld The token %s for lexeme %s does not match with the expected token %s\n",token.lineNum,tokenRepr(terminal),token.lexeme,tokenRepr(top->id));
				*error=1;
				pop(stack);
			}
			else{
				pop(stack);
				current->terminal->lineNum=token.lineNum;
				current->terminal->tokenType=top->id;
				if(top->id==eps)
				{
					continue;
				}
				int lenn=0;
				while(token.lexeme[lenn]!='\0')
				{
					current->terminal->lexeme[lenn]=token.lexeme[lenn];
					lenn++;
				}
				for(int i=lenn;i<MAX_LEXEME_SIZE;i++)
				current->terminal->lexeme[i]='\0';
				flag=1;
			}
		}
		else{
			nonTermID=t[top->id-NONTERMINAL_OFFSET][terminal].nonTerm;
			productionNo=t[top->id-NONTERMINAL_OFFSET][terminal].productionNum;
			if(nonTermID==-1 || productionNo==-1)
			{
				printf("\n\nPARSING ERROR AT LINE NO: %lld\n",token.lineNum);
				*error =1;
				getNextToken(sourceFile,&token);
				if(token.tokenType==TK_ERROR)
				printf("\n\nERROR2:LINE NO: %lld\n",token.lineNum);
				while(token.tokenType!=TK_EOF && t[top->id-NONTERMINAL_OFFSET][token.tokenType].syn==-1)
				{
					getNextToken(sourceFile,&token);
					if(token.tokenType==TK_ERROR)
					printf("\n\nERROR2:LINE NO: %lld\n",token.lineNum);
				}
				
				if(token.tokenType==TK_EOF)
					return;
				
				if(t[top->id-NONTERMINAL_OFFSET][token.tokenType].syn==1)
				pop(stack);
				continue;
			}
			rule=g[nonTermID].rules[productionNo];
			ruleLen=rule[0];
			current->children = malloc(ruleLen*sizeof(parsetree));
			current->numChild = ruleLen;
			current->numChildAST = ruleLen;
			current->nonTerminal=top->id;
			
			for(int i=1;i<=ruleLen;i++)
			{
				if(rule[i]<NO_OF_TERMINALS)
				{
					current->children[i-1].terminal=malloc(sizeof(tokenInfo));
					current->children[i-1].numChild=0 ;
					current->children[i-1].numChildAST=0;
					current->children[i-1].nonTerminal=-1;
					current->children[i-1].children=NULL;
					current->children[i-1].tp = NULL;
				}
				else{
					current->children[i].nonTerminal=rule[i];
				}
			}
			pop(stack);
			for(int i=ruleLen;i>0;i--)
			{
				//if(rule[i]!=eps)
				push(stack,rule[i],&(current->children[i-1]));
			}
		}
	} while(token.tokenType!=TK_EOF);
}

void parseInputSourceCode(FILE* sourceFile, int [][] t, Grammar g, parseTree root, int* error){
	Stack stack=newStack();
	Stack tempStack = newStack();
	tokenInfo token;
	Key start;
	Key dollar;
	start.id = program;
	start.tag = 1;
	dollar.id = dollar;
	dollar.tag = 0;
	push(stack,dollar,leaf);
	push(stack,program,root);
	Key k;
	int productionIndex;
	rhsCharNode rcn;
	do {
		getNextToken(sourceFile, &token);
		k = top1(stack);
		if((k.id == token.tokenType) && (token.tokenType == dollar)){
			break;
		} else if((k.id == token.tokenType) && (token.tokenType == dollar)){
			pop(stack);
			continue;
		} else if(k.tag == 1){
			if(t[k.id][token.tokenType] > 0) {
				productionIndex = t[k.id][token.tokenType];
				pop(stack);
				rcn = g[k.id].heads[productionIndex];
				while (rcn->next != NULL) {
					push(tempStack, rcn->s, leaf);
					rcn = rcn->next;
				}
				while(top1(tempStack)){
					Key prod = top1(tempStack);
					pop(tempStack);
					push(stack, prod.id, leaf);
				}
			} else {
				error();
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
		if(current->numChildAST==0 && current->terminal->tokenType==eps)
		continue;
		if(current->numChildAST>0)
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
		if(current->numChildAST==0)
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

