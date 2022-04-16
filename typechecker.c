/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */

#include "typechecker.h"

int TypeChecker(parsetree root, symbolTable sTable){

	if(root.numChild != 0 && (root.nt == function || root.nt == mainFunction))
	{
		tp = root.th;
	} 
	int type1, type2;
	type1 = 7;
	type2 = 8;
	if(root.terminal == NULL){
		switch(root.nt){
			case assignmentStmt:
			;
				type1 = TypeChecker(root.children[0], sTable);
				
				type2 = TypeChecker(root.children[1], sTable);
				if(type1 == error || type2 == error) break;
				else if(type1==integer && type2==integer){
					return integer;
				} else if(type1==integer && type2==real){
					parsetree temp = root;
					while(temp.terminal == NULL && temp.numChild != 0){
						temp = temp.children[0];
					}
					printf("Type error in line %llu:Type mismatch", temp.terminal->lineNum);
					break;
				} else if(type1==real && type2==integer){
					return real;
				} else if(type1==real && type2==real){
					return real;
				}

			case booleanExpression:
			;
				if(root.numChild == 3){
					type1 = TypeChecker(root.children[0], sTable);
					type2 = TypeChecker(root.children[2], sTable);
					if(root.children[1].terminal->tokenType == TK_AND){
						if(type1 != boolean || type2 != boolean){
							parsetree temp = root;
							while(temp.terminal == NULL && temp.numChild != 0){
			
								temp = temp.children[0];
							}
							printf("Type error in line %llu: Logical operators are applied only to boolean types but your expression contins non-boolean types. \n", temp.terminal->lineNum);
							return error;
							break;
						}	
					} else if(root.children[1].terminal->tokenType == TK_OR){
						if(type1 != boolean || type2 != boolean){
							parsetree temp = root;
							while(temp.terminal == NULL && temp.numChild != 0){
			
								temp = temp.children[0];
							}
							printf("Type error in line %llu: Logical operators are applied only to boolean types but your expression contins non-boolean types. \n", temp.terminal->lineNum);
							return error;
							break;
						}
					}
					else{
						if((type1!= integer && type1!= real)||(type2!=integer&&type2!=real))
						{
							printf("Type error in line %llu: Relational operations are applied only on int and real but your expression contains other data types.\n", root.children[0].terminal->lineNum);
							return error;
							break;
						}
						else if(type1!=type2)
						{
							printf("Type Error in line %llu: Different types\n", root.children[0].terminal->lineNum);
							return error;
							break;
						}
						return boolean;	
					}
					break;
				}
				else if (root.numChild == 2){
					type1 = TypeChecker(root.children[1], sTable);
					if(type1 == error){
						return error;
						break;
					}
						
					else if(type1!= boolean)
					{
						printf("Type error in line %llu: NOT operator can be applied only to boolean types but you expression contains non-boolean values.\n", root.children[0].terminal->lineNum);
						return error;
					}
					return boolean;
				}
				break;

			case arithmeticExpression:
			;
				type1 = TypeChecker(root.children[0], sTable);
				type2 = TypeChecker(root.children[1], sTable);
				if(type1==error || type2==error){
					break;
				} else if(type1==integer && type2==integer){
					return integer;
				} else if(type1==integer && type2==real){
					return real;
				} else if(type1==real && type2==integer){
					return real;
				} else if(type1==real && type2==real){
					return real;
				}
				break;

			case term:
			;
				type1 = TypeChecker(root.children[0], sTable);
				type2 = TypeChecker(root.children[1], sTable);
				if(type1==error || type2==error){
					break;
				} else if(type1==integer && type2==integer){
					return integer;
				} else if(type1==integer && type2==real){
					return real;
				} else if(type1==real && type2==integer){
					return real;
				} else if(type1==real && type2==real){
					return real;
				}
				break;

			case termPrime:
			;
				if(root.numChild == 3){
					type1 = TypeChecker(root.children[1], sTable);
					type2 = TypeChecker(root.children[2], sTable);
					if(type1==error || type2==error){
						break;
					} else if(type1==integer && type2==integer){
						return integer;
					} else if(type1==integer && type2==real){
						return real;
					} else if(type1==real && type2==integer){
						return real;
					} else if(type1==real && type2==real){
						return real;
					}
				} else{
					type1 = TypeChecker(root.children[1], sTable);
					return type1;
				}
				break;

			case expPrime:
			;
				if(root.numChild == 3){
					type1 = TypeChecker(root.children[1], sTable);
					type2 = TypeChecker(root.children[2], sTable);
					if(type1==error || type2==error){
						break;
					} else if(type1==integer && type2==integer){
						return integer;
					} else if(type1==integer && type2==real){
						return real;
					} else if(type1==real && type2==integer){
						return real;
					} else if(type1==real && type2==real){
						return real;
					}	
				}
				else{
					type1 = TypeChecker(root.children[1], sTable);
					return type1;
				}
				break;	

			case singleOrRecId:
			;
				record* rectype;
				if(sTable->fTable[hashFuncLUT("global",9973)]->localTable[hashFuncLUT(root.children[0].terminal->lexeme,9973)])
					rectype  = sTable->fTable[hashFuncLUT("global",9973)]->localTable[hashFuncLUT(root.children[0].terminal->lexeme, 9973)]->ptr;
				else if(tp->localTable[hashFuncLUT(root.children[0].terminal->lexeme, 9973)])
					rectype = tp->localTable[hashFuncLUT(root.children[0].terminal->lexeme, 9973)]->ptr;
				else break;

				if(rectype == NULL) break;
				while(rectype != NULL){
					if(!strcmp(rectype->recname , root.children[1].terminal->lexeme)) {
						if(strcmp(rectype->type,"int")==0)
							return integer;
						else if(strcmp(rectype->type,"real")==0)
							return real;
						else 
							return rec;
					}
					rectype = rectype->next;
				}

			default:
				for(int i = 0; i < root.numChild; i++){
					TypeChecker(root.children[i] , sTable);
				}
				break;
		}
	}
	else{
		switch(root.terminal->tokenType){
			case TK_ID:
			;
				if(tp == NULL) break;
				
				char* ch;
				if(sTable->fTable[hashFuncLUT("global",9973)]->localTable[hashFuncLUT(root.terminal->lexeme, 9973)])
					ch = sTable->fTable[hashFuncLUT("global",9973)]->localTable[hashFuncLUT(root.terminal->lexeme, 9973)]->type;
				else if(tp->localTable[hashFuncLUT(root.terminal->lexeme, 9973)])
					ch = tp->localTable[hashFuncLUT(root.terminal->lexeme, 9973)]->type;
				else
				 {
				 	printf("Line %llu: variable %s is not declared\n",root.terminal->lineNum,root.terminal->lexeme);	
				 	break ;
				 }
				
				if(strcmp(ch,"int")==0)
					return integer;
				else if(strcmp(ch,"real")==0)
					return real;
				else 
					return rec;
				break;
			case TK_NUM:
				return integer;
				break;
			case TK_RNUM:
				return real;
				break;
			case TK_RECORD:
				return rec;
				break;
			case TK_UNION:
				return uniontype;
				break;
			default:
				break;

		}
	}
	
}

