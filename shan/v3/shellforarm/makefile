CFLAGS = 
YACC = bison
YFLAGS = -d -v -b y
LEX = flex
LFLAGS = 
OBJS = io.o util.o y.tab.o lex.yy.o expand.o\
	kmatch.o direct.o sort.o stack.o var.o\
	iocmd.o cmdline.o builtins.o
#LIBES = -ly -lfl
HEADERS = io.h iocmd.h direct.h stack.h var.h
# CC = gcc
# This is set for AR Drone target
CC = arm-linux-gnueabi-gcc
io: $(OBJS)
	$(CC) -o io $(CFLAGS) $(OBJS) $(LIBES)

lex.yy.o: lex.yy.c $(HEADERS)
	$(CC) -c $(CFLAGS) lex.yy.c

y.tab.o: y.tab.c $(HEADERS)
	$(CC) -c $(CFLAGS) y.tab.c

y.tab.c y.tab.h: grammar.y $(HEADERS)
	$(YACC) $(YFLAGS) grammar.y

lex.yy.c: rules.l y.tab.h
	$(LEX) $(LFLAGS) rules.l

util.o: util.c $(HEADERS)
	$(CC) -c $(CFLAGS) util.c

kmatch.o: kmatch.c $(HEADERS)
	$(CC) -c $(CFLAGS) kmatch.c

expand.o: expand.c $(HEADERS)
	$(CC) -c $(CFLAGS) expand.c

direct.o: direct.c $(HEADERS)
	$(CC) -c $(CFLAGS) direct.c

stack.o: stack.c $(HEADERS)
	$(CC) -c $(CFLAGS) stack.c

sort.o: sort.c $(HEADERS)
	$(CC) -c $(CFLAGS) sort.c

var.o: var.c $(HEADERS)
	$(CC) -c $(CFLAGS) var.c

cmdline.o: cmdline.c $(HEADERS)
	$(CC) -c $(CFLAGS) cmdline.c

iocmd.o: iocmd.c $(HEADERS)
	$(CC) -c $(CFLAGS) iocmd.c

builtins.o: builtins.c $(HEADERS)
	$(CC) -c $(CFLAGS) builtins.c

clean:
	rm -f *.o *~ y.tab.[ch] lex.yy.c io y.output core a.out



