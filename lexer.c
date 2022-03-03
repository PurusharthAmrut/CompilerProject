/*
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
 */

#include "lexer.h"

#define MAX_BUFFER_SIZE 4096 // max buffer size --Standard Size
#define MAX_ID_LENGTH 20

unsigned long long lineNum = 1;	   // the "current line" in the source file
char inputBuffer[MAX_BUFFER_SIZE]; // the input buffer
int bufIndex;
int bufSize = -1;

LookUpTable* createLookUpTable(int num_pos) {
	LookUpTable *lookUpTable = (LookUpTable*)malloc(sizeof(LookUpTable));
	lookUpTable->num_pos = num_pos;
	lookUpTable->positions = (List **)malloc(num_pos*sizeof(List*));
	for (int i = 0; i < num_pos; i++) {
		lookUpTable->positions[i] = (List*)malloc(sizeof(List));
		lookUpTable->positions[i]->head = NULL;
	}
	fillLookUpTable(lookUpTable);
	return lookUpTable;
}

<<<<<<< HEAD
void insertNodeElement(LookUpTable *lookUpTable, char *key, terminal tokentype) {
=======
void insertNodeElement(LookUpTable *lookUpTable, char *key, terminal tokentype)
{
>>>>>>> 95346c189b04b9527dc2d851d67d919d1adf594d
	int hashvalue = hashFuncLUT(key, lookUpTable->num_pos);
	NodeElement *entry = (NodeElement*)malloc(sizeof(NodeElement));
	entry->type = tokentype;
	entry->lexeme = (char*)malloc(sizeof(char)*(strlen(key)+1));
	strcpy(entry->lexeme, key);
	entry->next = lookUpTable->positions[hashvalue]->head;
	lookUpTable->positions[hashvalue]->head = entry;
}

int hashFuncLUT(char *key, int num_pos) {
	int sum = 0;
	for (int i=0; i<strlen(key); i++) {
        sum = (sum * 10) + (key[i]-'0');
		if(sum<0) sum = 0;
	}
	return (sum%num_pos);
}

// Filling keywords in the Lookup Table
void fillLookUpTable(LookUpTable *lookUpTable) {
	insertNodeElement(lookUpTable, "with", TK_WITH);
	insertNodeElement(lookUpTable, "parameters", TK_PARAMETERS);
	insertNodeElement(lookUpTable, "end", TK_END);
	insertNodeElement(lookUpTable, "while", TK_WHILE);
	insertNodeElement(lookUpTable, "type", TK_TYPE);
	insertNodeElement(lookUpTable, "_main", TK_MAIN);
	insertNodeElement(lookUpTable, "global", TK_GLOBAL);
	insertNodeElement(lookUpTable, "parameter", TK_PARAMETER);
	insertNodeElement(lookUpTable, "list", TK_LIST);
	insertNodeElement(lookUpTable, "input", TK_INPUT);
	insertNodeElement(lookUpTable, "output", TK_OUTPUT);
	insertNodeElement(lookUpTable, "int", TK_INT);
	insertNodeElement(lookUpTable, "real", TK_REAL);
	insertNodeElement(lookUpTable, "endwhile", TK_ENDWHILE);
	insertNodeElement(lookUpTable, "if", TK_IF);
	insertNodeElement(lookUpTable, "then", TK_THEN);
	insertNodeElement(lookUpTable, "endif", TK_ENDIF);
	insertNodeElement(lookUpTable, "read", TK_READ);
	insertNodeElement(lookUpTable, "write", TK_WRITE);
	insertNodeElement(lookUpTable, "return", TK_RETURN);
	insertNodeElement(lookUpTable, "call", TK_CALL);
	insertNodeElement(lookUpTable, "record", TK_RECORD);
	insertNodeElement(lookUpTable, "endrecord", TK_ENDRECORD);
	insertNodeElement(lookUpTable, "union", TK_UNION);
	insertNodeElement(lookUpTable, "definetype", TK_DEFINETYPE);
	insertNodeElement(lookUpTable, "as", TK_AS);
	insertNodeElement(lookUpTable, "endunion", TK_ENDUNION);
	insertNodeElement(lookUpTable, "else", TK_ELSE);
}

terminal getTokenType(char *key, LookUpTable *lookUpTable) {
	int hashvalue = hashFuncLUT(key, lookUpTable->num_pos);
	NodeElement *temp = lookUpTable->positions[hashvalue]->head;
	while(temp != NULL) {
		if(strcmp(temp->lexeme, key)==0) return temp->type;
		temp = temp->next;
	}
	return -1;
}

void removeComments(char *testcaseFile, char *cleanFile) {
	FILE *src, *dst, *read;
	src = fopen(testcaseFile, "r");
	if(src==NULL) {
		fprintf(stderr, "Cannot open %s file \n", testcaseFile);
		exit(0);
	}
	dst = fopen(cleanFile, "w");
	if(dst==NULL) {
		fprintf(stderr, "Cannot open %s file \n", cleanFile);
		exit(0);
	}
	char c;
	int m = 0, size = 1000;
	char *inputBuffer = (char*)malloc(1000*sizeof(char));
	while(!feof(src)) {
		c = getc(src);
		if(c=='%') {
			while(!feof(src)) {
				if((c=getc(src))=='\n') {
					inputBuffer[m++] = c;
					if(m==size) {
                        size *= 2;
						inputBuffer = (char*)realloc(inputBuffer, size);
					}
					break;
				}
			}
		}else {
			if(c!=EOF) inputBuffer[m++] = c;
			if(m==size) {
                size *= 2;
				inputBuffer = (char*)realloc(inputBuffer, size);
			}
		}
	}
	fprintf(dst, "%s", inputBuffer);
	free(inputBuffer);
	fclose(src);
	fclose(dst);

	// printing the comment free code on console from file
	read = fopen(cleanFile, "r");
	if (read == NULL) {
		fprintf(stderr, "Cannot open %s file\n", cleanFile);
		exit(0);
	}
	char ch;
	while((ch=fgetc(read))!=EOF) printf("%c", ch);
	fclose(read);
}

FILE *getStream(FILE *fp, char *c) {
	if(bufSize==bufIndex) {
		bufSize = fread(inputBuffer, sizeof(char), (size_t)MAX_BUFFER_SIZE, fp);
		bufIndex = 1;
		switch(bufSize) {
            case 0:
                *c = 26;
                return fp;
            default:
                *c = inputBuffer[0];
                return fp;
		}
	}
	switch(bufSize) {
        case -1:
            bufSize = fread(inputBuffer, sizeof(char), (size_t)MAX_BUFFER_SIZE, fp);
            bufIndex = 1;
            switch(bufSize) {
                case 0:
                    *c = 26;
                    return fp;
                default:
                    *c = inputBuffer[0];
                    return fp;
            }
        case 0:
            *c = 26;
            return fp;
        default:
            *c = inputBuffer[bufIndex++];
            return fp;
	}
}

// function to print the tokens on console
void printToken(tokenInfo *t) {
	printf("Line %llu--> |%s|\t\t|%s|", t->lineNum, getTermString(t->tokenType), t->lexeme);
	printf("\n");
}

void printTokenizedCode(char *filepath) {
    FILE *fp = fopen(filepath, "r");
    tokenInfo *token = (tokenInfo*)malloc(sizeof(tokenInfo));
    token->tokenType = 0;
    while(1) {
        getNextToken(fp, token);
        printToken(token);
        if(token->tokenType==TK_EOF) break;
    }
    free(token);
    fclose(fp);
}

void getNextToken(FILE *fp, tokenInfo *t) {
    char c;
    int count = 0, state = 1;
    LookUpTable *lookUpTable = createLookUpTable(50);
    while(1) {
        fp = getStream(fp, &c);
        t->lineNum = lineNum;
        t->lexeme[count++] = c;

        if(c==26) {
            t->tokenType = TK_EOF;
            sprintf(t->lexeme, "End of File");
            return;
        }

        switch(state) {
            case 1:
                switch(c) {
                    case '%':
                        state = 55;
                        break;
                    case '<':
                        state = 20;
                        break;
                    case '&':
                        state = 45;
                        break;
                    case '@':
                        state = 48;
                        break;
                    case '[': // TK_SQL found
                        t->tokenType = TK_SQL;
                        t->lexeme[count] = '\0';
                        return;
                    case ']': // TK_SQR found
                        t->tokenType = TK_SQR;
                        t->lexeme[count] = '\0';
                        return;
                    case ',': // TK_COMMA found
                        t->tokenType = TK_COMMA;
                        t->lexeme[count] = '\0';
                        return;
                    case ';': // TK_SEM found
                        t->tokenType = TK_SEM;
                        t->lexeme[count] = '\0';
                        return;
                    case ':': // TK_COLON found
                        t->tokenType = TK_COLON;
                        t->lexeme[count] = '\0';
                        return;
                    case '.': // TK_DOT found
                        t->tokenType = TK_DOT;
                        t->lexeme[count] = '\0';
                        return;
                    case '(': // TK_OP found
                        t->tokenType = TK_OP;
                        t->lexeme[count] = '\0';
                        return;
                    case ')': // TK_CL found
                        t->tokenType = TK_CL;
                        t->lexeme[count] = '\0';
                        return;
                    case '+': // TK_PLUS found
                        t->tokenType = TK_PLUS;
                        t->lexeme[count] = '\0';
                        return;
                    case '-': // TK_MINUS found
                        t->tokenType = TK_MINUS;
                        t->lexeme[count] = '\0';
                        return;
                    case '*': // TK_MUL found
                        t->tokenType = TK_MUL;
                        t->lexeme[count] = '\0';
                        return;
                    case '/': // TK_DIV found
                        t->tokenType = TK_DIV;
                        t->lexeme[count] = '\0';
                        return;
                    case '~': // TK_NOT found
                        t->tokenType = TK_NOT;
                        t->lexeme[count] = '\0';
                        return;
                    case '=':
                        state = 29;
                        break;
                    case '>':
                        state = 26;
                        break;
                    case '!':
                        state = 31;
                        break;
                    case 'b':
                        state = 6;
                        break;
                    case 'c':
                    case 'd':
                        state = 5;
                        break;
                    case 'a':
                    case 'e':
                    case 'f':
                    case 'g':
                    case 'h':
                    case 'i':
                    case 'j':
                    case 'k':
                    case 'l':
                    case 'm':
                    case 'n':
                    case 'o':
                    case 'p':
                    case 'q':
                    case 'r':
                    case 's':
                    case 't':
                    case 'u':
                    case 'v':
                    case 'w':
                    case 'x':
                    case 'y':
                    case 'z':
                        state = 2;
                        break;
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        state = 10;
                        break;
                    case '_':
                        state = 33;
                        break;
                    case '#':
                        state = 42;
                        break;
                    case '\n':
                        // adding a line and retaining state
                        lineNum++;
                        // overwriting '\n' so to not store that in lexeme
                        count--;
                        break;
                    case ' ':
                    case '\t':
                    case '\r':
                        // ignoring whitespace at start state
                        // by retaining state
                        // overwriting ' ' or '\t' or '\r' so to not store that in lexeme
                        count--;
                        break;
                    default:										// invalid starting character
                        sprintf(t->lexeme, "Unknown Symbol %c", c); // storing the error message in t->lexeme
                        t->tokenType = TK_ERROR;
                        return;
                }
                break;
            case 2:
                switch(c) {
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':
                    case 'e':
                    case 'f':
                    case 'g':
                    case 'h':
                    case 'i':
                    case 'j':
                    case 'k':
                    case 'l':
                    case 'm':
                    case 'n':
                    case 'o':
                    case 'p':
                    case 'q':
                    case 'r':
                    case 's':
                    case 't':
                    case 'u':
                    case 'v':
                    case 'w':
                    case 'x':
                    case 'y':
                    case 'z':
                        if(count==(MAX_ID_LENGTH+1)) {
                            while(c>='a' && c<='z') fp = getStream(fp, &c);
                            bufIndex--;
                            t->tokenType = TK_ERROR;
                            // storing the error message in t->lexeme
                            sprintf(t->lexeme, "Identifier is longer than the prescribed length of 20 characters"); 
                            return;
                        }
                        break;
                    default:
                        t->lexeme[count-1] = '\0';
                        int temp = getTokenType(t->lexeme, lookUpTable);
                        t->tokenType = (temp > 0) ? temp : TK_FIELDID;
                        bufIndex--;
                        return;
                }
                break;
            case 5:
                switch(c) {
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                        state = 7;
                        break;
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':
                    case 'e':
                    case 'f':
                    case 'g':
                    case 'h':
                    case 'i':
                    case 'j':
                    case 'k':
                    case 'l':
                    case 'm':
                    case 'n':
                    case 'o':
                    case 'p':
                    case 'q':
                    case 'r':
                    case 's':
                    case 't':
                    case 'u':
                    case 'v':
                    case 'w':
                    case 'x':
                    case 'y':
                    case 'z':
                        state = 2;
                        break;
                    default:
                        t->tokenType = TK_ERROR;
                        strcpy(t->lexeme, "expecting [2-7] or [a-z]");
                        if(c=='\n' || c=='\t' || c=='\r' || c==' ') return;
                        bufIndex--;
                        return;
                }
                break;
            case 6:
                switch(c) {
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                        state = 7;
                        break;
                    default:
                        t->tokenType = TK_ERROR;
                        strcpy(t->lexeme, "expecting [2-7]");
                        if(c=='\n' || c=='\t' || c=='\r' || c==' ') return;
                        bufIndex--;
                        return;
                }
                break;
            case 7:
                switch(c) {
                    case 'b':
                    case 'c':
                    case 'd':
                        break; // retain state
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                        state = 8;
                        break;
                    default: // TK_ID found
                        t->lexeme[count-1] = '\0';
                        t->tokenType = TK_ID;
                        bufIndex--;
                        return;
                }
                break;
            case 8:
                switch(c) {
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                        break; // retain state
                    default:   // TK_ID found
                        t->lexeme[count-1] = '\0';
                        t->tokenType = TK_ID;
                        bufIndex--;
                        return;
                }
                break;
            case 10:
                switch(c) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        if(count==21) {
                            while(c>='0' && c<='9') fp = getStream(fp, &c);
                            if(c=='.') {
                                while(c>='0' && c<='9') fp = getStream(fp, &c);
                                if(c=='E') {
                                    while(c=='+' || c=='-') fp = getStream(fp, &c);
                                    while(c>='0' && c<='9') fp = getStream(fp, &c);
                                }
                            }
                            t->tokenType = TK_ERROR;
                            t->lexeme[count - 1] = '\0';
                            sprintf(t->lexeme, "%s: number longer than prescribed length", t->lexeme);
                            bufIndex--;
                            return;
                        }
                        break;
                    case '.':
                        state = 11;
                        break;
                    default: // TK_NUM found
                        t->lexeme[count - 1] = '\0';
                        t->tokenType = TK_NUM;
                        bufIndex--;
                        return;
                }
                break;
            case 11:
                switch(c) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        state = 12;
                        break;
                    default:
                        t->lexeme[count - 1] = '\0';
                        sprintf(t->lexeme, "unknown pattern %s", t->lexeme);
                        t->tokenType = TK_ERROR;
                        bufIndex--;
                        return;
                }
                break;
            case 12:
                switch(c) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        state = 13;
                        break;
                    default:
                        t->lexeme[count - 1] = '\0';
                        sprintf(t->lexeme, "unknown pattern %s", t->lexeme);
                        t->tokenType = TK_ERROR;
                        bufIndex--;
                        return;
                }
                break;
            case 13:
                switch(c) {
                    case 'E':
                        state = 14;
                        break;
                    default: // TK_RNUM found
                        t->lexeme[count - 1] = '\0';
                        t->tokenType = TK_RNUM;
                        bufIndex--;
                        return;
                }
                break;
            case 14:
                switch(c) {
                    case '+':
                    case '-':
                        state = 15;
                        break;
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        state = 16;
                        break;
                    default:
                        t->lexeme[count - 1] = '\0';
                        sprintf(t->lexeme, "unknown pattern %s", t->lexeme);
                        t->tokenType = TK_ERROR;
                        bufIndex--;
                        return;
                }
                break;
            case 15:
                switch(c) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        state = 16;
                        break;
                    default:
                        t->lexeme[count - 1] = '\0';
                        sprintf(t->lexeme, "unknown pattern %s", t->lexeme);
                        t->tokenType = TK_ERROR;
                        bufIndex--;
                        return;
                }
                break;
            case 16:
                switch(c) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        t->lexeme[count] = '\0';
                        t->tokenType = TK_RNUM;
                        return;
                    default:
                        t->lexeme[count - 1] = '\0';
                        sprintf(t->lexeme, "unknown pattern %s", t->lexeme);
                        t->tokenType = TK_ERROR;
                        bufIndex--;
                        return;
                }
                break;
            case 20:
                switch(c) {
                    case '-':
                        state = 21;
                        break;
                    case '=': // token TK_LE found
                        t->tokenType = TK_LE;
                        t->lexeme[count] = '\0';
                        return;
                    default:
                        t->tokenType = TK_LT;
                        t->lexeme[count - 1] = '\0';
                        bufIndex--;
                        return;
                }
                break;
            case 21:
                switch(c) {
                    case '-':
                        state = 22;
                        break;
                    default:
                        t->tokenType = TK_ERROR;
                        strcpy(t->lexeme, "Unknown pattern <-");
                        bufIndex--;
                        return;
                }
                break;
            case 22:
                switch(c) {
                    case '-': // TK_ASSIGNOP found
                        t->tokenType = TK_ASSIGNOP;
                        t->lexeme[count] = '\0';
                        return;
                    default:
                        t->tokenType = TK_ERROR;
                        strcpy(t->lexeme, "Unknown pattern <--");
                        bufIndex--;
                        return;
                }
                break;
            case 26:
                switch(c) {
                    case '=': // TK_GE found
                        t->tokenType = TK_GE;
                        t->lexeme[count] = '\0';
                        return;
                    default: // TK_GT found
                        t->lexeme[count - 1] = '\0';
                        t->tokenType = TK_GT;
                        bufIndex--;
                        return;
                }
                break;
            case 29:
                switch(c) {
                    case '=': // TK_EQ found
                        t->tokenType = TK_EQ;
                        t->lexeme[count] = '\0';
                        return;
                    default:
                        sprintf(t->lexeme, "Unknown pattern =");
                        t->tokenType = TK_ERROR;
                        bufIndex--;
                        return;
                }
                break;
            case 31:
                switch(c) {
                    case '=': // TK_NE found
                        t->tokenType = TK_NE;
                        t->lexeme[count] = '\0';
                        return;
                    default:
                        sprintf(t->lexeme, "Unknown pattern !"); // storing the error message in t->lexeme
                        t->tokenType = TK_ERROR;
                        bufIndex--;
                        return;
                }
                break;
            case 33:
                switch(c) {
                    // all alphabets except 'm'
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':
                    case 'e':
                    case 'f':
                    case 'g':
                    case 'h':
                    case 'i':
                    case 'j':
                    case 'k':
                    case 'l':
                    case 'n':
                    case 'o':
                    case 'p':
                    case 'q':
                    case 'r':
                    case 's':
                    case 't':
                    case 'u':
                    case 'v':
                    case 'w':
                    case 'x':
                    case 'y':
                    case 'z':
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'E':
                    case 'F':
                    case 'G':
                    case 'H':
                    case 'I':
                    case 'J':
                    case 'K':
                    case 'L':
                    case 'M':
                    case 'N':
                    case 'O':
                    case 'P':
                    case 'Q':
                    case 'R':
                    case 'S':
                    case 'T':
                    case 'U':
                    case 'V':
                    case 'W':
                    case 'X':
                    case 'Y':
                    case 'Z':
                        state = 34;
                        break;
                    case 'm':
                        state = 37;
                        break;
                    default:
                        t->tokenType = TK_ERROR;
                        strcpy(t->lexeme, "expecting alphabet after underscore for function name");
                        bufIndex--;
                        return;
                }
                break;
            case 34:
                switch(c) {
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':
                    case 'e':
                    case 'f':
                    case 'g':
                    case 'h':
                    case 'i':
                    case 'j':
                    case 'k':
                    case 'l':
                    case 'm':
                    case 'n':
                    case 'o':
                    case 'p':
                    case 'q':
                    case 'r':
                    case 's':
                    case 't':
                    case 'u':
                    case 'v':
                    case 'w':
                    case 'x':
                    case 'y':
                    case 'z':
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'E':
                    case 'F':
                    case 'G':
                    case 'H':
                    case 'I':
                    case 'J':
                    case 'K':
                    case 'L':
                    case 'M':
                    case 'N':
                    case 'O':
                    case 'P':
                    case 'Q':
                    case 'R':
                    case 'S':
                    case 'T':
                    case 'U':
                    case 'V':
                    case 'W':
                    case 'X':
                    case 'Y':
                    case 'Z':
                        // retain state
                        break;
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        state = 35;
                        break;
                    default: // TK_FUNID
                        t->tokenType = TK_FUNID;
                        t->lexeme[count - 1] = '\0';
                        bufIndex--;
                        return;
                }
                break;
            case 35:
                switch(c) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        // retain state
                        break;
                    default:   // TK_FUNID
                        t->tokenType = TK_FUNID;
                        t->lexeme[count - 1] = '\0';
                        bufIndex--;
                        return;
                }
                break;
            case 37:
                switch(c) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        state = 35;
                        break;
                    case 'b':
                    case 'c':
                    case 'd':
                    case 'e':
                    case 'f':
                    case 'g':
                    case 'h':
                    case 'i':
                    case 'j':
                    case 'k':
                    case 'l':
                    case 'm':
                    case 'n':
                    case 'o':
                    case 'p':
                    case 'q':
                    case 'r':
                    case 's':
                    case 't':
                    case 'u':
                    case 'v':
                    case 'w':
                    case 'x':
                    case 'y':
                    case 'z':
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'E':
                    case 'F':
                    case 'G':
                    case 'H':
                    case 'I':
                    case 'J':
                    case 'K':
                    case 'L':
                    case 'M':
                    case 'N':
                    case 'O':
                    case 'P':
                    case 'Q':
                    case 'R':
                    case 'S':
                    case 'T':
                    case 'U':
                    case 'V':
                    case 'W':
                    case 'X':
                    case 'Y':
                    case 'Z':
                        state = 34;
                        break;
                    case 'a':
                        state = 38;
                        break;
                    default:
                        t->tokenType = TK_ERROR;
                        strcpy(t->lexeme, "expecting alphanumeral for function name");
                        bufIndex--;
                        return;
                }
                break;
            case 38:
                switch(c) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        state = 35;
                        break;
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':
                    case 'e':
                    case 'f':
                    case 'g':
                    case 'h':
                    case 'j':
                    case 'k':
                    case 'l':
                    case 'm':
                    case 'n':
                    case 'o':
                    case 'p':
                    case 'q':
                    case 'r':
                    case 's':
                    case 't':
                    case 'u':
                    case 'v':
                    case 'w':
                    case 'x':
                    case 'y':
                    case 'z':
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'E':
                    case 'F':
                    case 'G':
                    case 'H':
                    case 'I':
                    case 'J':
                    case 'K':
                    case 'L':
                    case 'M':
                    case 'N':
                    case 'O':
                    case 'P':
                    case 'Q':
                    case 'R':
                    case 'S':
                    case 'T':
                    case 'U':
                    case 'V':
                    case 'W':
                    case 'X':
                    case 'Y':
                    case 'Z':
                        state = 34;
                        break;
                    case 'i':
                        state = 39;
                        break;
                    default:
                        t->tokenType = TK_ERROR;
                        strcpy(t->lexeme, "expecting alphanumeral for function name");
                        bufIndex--;
                        return;
                }
                break;
            case 39:
                switch(c) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        state = 35;
                        break;
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':
                    case 'e':
                    case 'f':
                    case 'g':
                    case 'h':
                    case 'i':
                    case 'j':
                    case 'k':
                    case 'l':
                    case 'm':
                    case 'o':
                    case 'p':
                    case 'q':
                    case 'r':
                    case 's':
                    case 't':
                    case 'u':
                    case 'v':
                    case 'w':
                    case 'x':
                    case 'y':
                    case 'z':
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'E':
                    case 'F':
                    case 'G':
                    case 'H':
                    case 'I':
                    case 'J':
                    case 'K':
                    case 'L':
                    case 'M':
                    case 'N':
                    case 'O':
                    case 'P':
                    case 'Q':
                    case 'R':
                    case 'S':
                    case 'T':
                    case 'U':
                    case 'V':
                    case 'W':
                    case 'X':
                    case 'Y':
                    case 'Z':
                        state = 34;
                        break;
                    case 'n':
                        state = 40;
                        break;
                    default:
                        t->tokenType = TK_ERROR;
                        strcpy(t->lexeme, "expecting alphanumeral for function name");
                        bufIndex--;
                        return;
                }
                break;
            case 40:
                switch(c) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        state = 35;
                        break;
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':
                    case 'e':
                    case 'f':
                    case 'g':
                    case 'h':
                    case 'i':
                    case 'j':
                    case 'k':
                    case 'l':
                    case 'm':
                    case 'n':
                    case 'o':
                    case 'p':
                    case 'q':
                    case 'r':
                    case 's':
                    case 't':
                    case 'u':
                    case 'v':
                    case 'w':
                    case 'x':
                    case 'y':
                    case 'z':
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'E':
                    case 'F':
                    case 'G':
                    case 'H':
                    case 'I':
                    case 'J':
                    case 'K':
                    case 'L':
                    case 'M':
                    case 'N':
                    case 'O':
                    case 'P':
                    case 'Q':
                    case 'R':
                    case 'S':
                    case 'T':
                    case 'U':
                    case 'V':
                    case 'W':
                    case 'X':
                    case 'Y':
                    case 'Z':
                        state = 34;
                        break;
                    default: // TK_MAIN found
                        t->tokenType = TK_MAIN;
                        t->lexeme[count - 1] = '\0';
                        bufIndex--;
                        return;
                }
                break;
            case 42:
                switch(c) {
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':
                    case 'e':
                    case 'f':
                    case 'g':
                    case 'h':
                    case 'i':
                    case 'j':
                    case 'k':
                    case 'l':
                    case 'm':
                    case 'n':
                    case 'o':
                    case 'p':
                    case 'q':
                    case 'r':
                    case 's':
                    case 't':
                    case 'u':
                    case 'v':
                    case 'w':
                    case 'x':
                    case 'y':
                    case 'z':
                        state = 43;
                        break;
                    default:
                        t->tokenType = TK_ERROR;
                        strcpy(t->lexeme, "expecting [a-z] for RUID");
                        bufIndex--;
                        return;
                }
                break;
            case 43:
                switch(c) {
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':
                    case 'e':
                    case 'f':
                    case 'g':
                    case 'h':
                    case 'i':
                    case 'j':
                    case 'k':
                    case 'l':
                    case 'm':
                    case 'n':
                    case 'o':
                    case 'p':
                    case 'q':
                    case 'r':
                    case 's':
                    case 't':
                    case 'u':
                    case 'v':
                    case 'w':
                    case 'x':
                    case 'y':
                    case 'z':
                        // retain state
                        break;
                    default:
                        t->tokenType = TK_RUID;
                        t->lexeme[count - 1] = '\0';
                        bufIndex--;
                        return;
                }
                break;
            case 45:
                switch(c) {
                    case '&':
                        state = 46;
                        break;
                    default:
                        t->tokenType = TK_ERROR;
                        sprintf(t->lexeme, "Unknown pattern &"); // storing the error message in t->lexeme
                        bufIndex--;
                        return;
                }
                break;
            case 46:
                switch(c) {
                    case '&': // TK_AND found
                        t->tokenType = TK_AND;
                        t->lexeme[count] = '\0';
                        return;
                    default:
                        sprintf(t->lexeme, "Unknown pattern &&"); // storing the error message in t->lexeme
                        t->tokenType = TK_ERROR;
                        bufIndex--;
                        return;
                }
                break;
            case 48:
                switch(c) {
                    case '@':
                        state = 49;
                        break;
                    default:
                        // storing the error message in t->lexeme
                        sprintf(t->lexeme, "Unknown pattern @");
                        t->tokenType = TK_ERROR;
                        bufIndex--;
                        return;
                }
                break;
            case 49:
                switch(c) {
                    case '@': // TK_OR found
                        t->tokenType = TK_OR;
                        t->lexeme[count] = '\0';
                        return;
                    default: // error
                        sprintf(t->lexeme, "Unknown pattern @@");
                        t->tokenType = TK_ERROR;
                        bufIndex--;
                        return;
                }
                break;
            case 55:
                switch(c) {
                    // comment starts and read until we encounter a new line
                    case '\n':
                        t->lexeme[count - 1] = '\0';
                        state = 0; // after end of comment return to state 0
                        count = 0;
                        // comment, though ignored is still counted as a line
                        lineNum++;
                        t->tokenType = TK_COMMENT;
                        return;
                    default:
                        // retain state
                        break;
                }
                break;
            default:
                fprintf(stderr, "Found Some Error while getting the next token\n");
        }
    }
}