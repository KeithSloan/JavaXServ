CC=gcc
CFLAGS=-c
LDFLAGS=-lX11
sources = $(wildcard *.c)
objects = $(sources:.c=.o)
executables = $(objects:.o=)

all: $(executables)

clean :
	rm -f *.o
	rm -f $(executables)

$(executables): $(objects)
	$(CC) $(LDFLAGS) $@.o -o $@
	grep -v $@ .gitignore > tmp
	echo $@ >> tmp
	mv tmp .gitignore
	

%.c.o : 
	$(CC) $(CFLAGS) $< -o$@

	




