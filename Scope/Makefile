CC=g++
CFLAGS=-c 


all: scanner

scanner: plc.o administration.o scanner.o parser.o firstfollow.o symboltable.o token.o blocktable.o
	$(CC) plc.o administration.o scanner.o parser.o firstfollow.o symboltable.o token.o blocktable.o -o typenscope

plc.o: plc.cc
	$(CC) $(CFLAGS) plc.cc -g

administration.o: administration.cc
	$(CC) $(CFLAGS) administration.cc -g

scanner.o: scanner.cc
	$(CC) $(CFLAGS) scanner.cc -g

parser.o: parser.cc
	$(CC) $(CFLAGS) parser.cc -g

firstfollow.o: firstfollow.cc
	$(CC) $(CFLAGS) firstfollow.cc -g

symboltable.o: symboltable.cc
	$(CC) $(CFLAGS) symboltable.cc -g
	
token.o: token.cc
	$(CC) $(CFLAGS) token.cc -g

blockTable.o: blocktable.cc
	$(CC) $(CFLAGS) blocktable.cc -g

clean:
	rm -rf *.o *~ typenscope
	 


