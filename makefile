# edit: utils.o lexer.o lexerTest.o
#   gcc -o lexerTest utils.o lexer.o lexerTest.o 

edit: utils.o lexer.o Stack.o parser.o parserTest.o
    gcc -o parserTest utils.o lexer.o Stack.o parser.o parserTest.o

utils.o : utils.c utils.h 
    gcc -c utils.c
lexer.o : lexer.c lexer.h lexerDef.h
    gcc -c lexer.c
lexerTest.o: lexerTest.c
    gcc -c lexerTest.c
Stack.o : Stack.c Stack.h
    gcc -c Stack.c
parser.o : parser.c parser.h parserDef.h
    gcc -c parser.c
parserTest.o: parserTest.c
    gcc -c parserTest.c
driver.o : driver.c 
    gcc -c driver.c

clean :
    rm lexerTest parserTest utils.o lexer.o lexerTest.o Stack.o parser.o parserTest.o