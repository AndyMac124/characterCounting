COMPILER = gcc
CFLAGS = -Wall -pedantic

EXES = char_count

all: ${EXES}

char_count:	char_count.c
	${COMPILER} ${CFLAGS} char_count.c -o char_count

clean:
	rm -f *~ *.o ${EXES}