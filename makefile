COMPILER = gcc
CFLAGS = -Wall -pedantic

EXES = char_count

all: ${EXES}

char_count:	char_count.c files.o ring_process.o
	${COMPILER} ${CFLAGS} char_count.c files.o ring_process.o -o char_count

files.o: files.c
	${COMPILER} ${CFLAGS} -c files.c

ring_process.o: ring_process.c
	${COMPILER} ${CFLAGS} -c ring_process.c

clean:
	rm -f *~ *.o ${EXES}