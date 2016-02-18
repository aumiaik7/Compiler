CC=g++
CFLAGS=-c 


all: scanner

scanner: plc.o administration.o scanner.o parser.o firstfollow.o symboltable.o token.o
	$(CC) plc.o administration.o scanner.o parser.o firstfollow.o symboltable.o token.o -o parser

plc.o: plc.cc
	$(CC) $(CFLAGS) plc.cc

administration.o: administration.cc
	$(CC) $(CFLAGS) administration.cc

scanner.o: scanner.cc
	$(CC) $(CFLAGS) scanner.cc

parser.o: parser.cc
	$(CC) $(CFLAGS) parser.cc

firstfollow.o: firstfollow.cc
	$(CC) $(CFLAGS) firstfollow.cc

symboltable.o: symboltable.cc
	$(CC) $(CFLAGS) symboltable.cc
	
token.o: token.cc
	$(CC) $(CFLAGS) token.cc

clean:
	rm -rf *.o *~ scanner
	 


