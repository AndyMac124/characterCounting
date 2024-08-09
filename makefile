COMPILER = gcc
CFLAGS = -Wall -pedantic
COBJS = char_count_utils.o file_handling.o ring_process.o

EXES = char_count

all: ${EXES}

char_count:	char_count.c ${COBJS}
	${COMPILER} ${CFLAGS} char_count.c ${COBJS} -o char_count -lm

char_count_utils.o: char_count_utils.c char_count_utils.h
	${COMPILER} ${CFLAGS} -c char_count_utils.c

file_handling.o: file_handling.c file_handling.h
	${COMPILER} ${CFLAGS} -c file_handling.c

ring_process.o: ring_process.c ring_process.h file_handling.h
	${COMPILER} ${CFLAGS} -c ring_process.c

run: char_count
	./char_count ${ARGS}

clean:
	rm -f *~ *.o ${EXES}