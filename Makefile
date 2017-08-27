CC=gcc
CFLAGS=-I.
DEPS = prompt.h input.h tokenize.h
OBJ = prompt.o shell.o input.o tokenize.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

a.out: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o *~ core
