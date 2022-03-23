edit: driver.o utils.o lexer.o Stack.o parser.o
	gcc -o getGram2 utils.o lexer.o Stack.o parser.o

utils.o : utils.c utils.h 
	gcc -c utils.c
lexer.o : lexer.c lexer.h lexerDef.h
	gcc -c lexer.c
Stack.o : Stack.c Stack.h
	gcc -c Stack.c
parser.o : parser.c parser.h parserDef.h
	gcc -c parser.c
getGram2.o: getGram2.c getGram2.h
	gcc -c getGram2.c

clean :
	rm utils.o lexer.o Stack.o parser.o driver.o
