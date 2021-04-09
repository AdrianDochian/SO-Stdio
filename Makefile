CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -L .

all: build

build:libso_stdio.so

libso_stdio.so: so_stdio.o
	$(CC) -shared -o $@ $^

so_stdio.o: so_stdio.c so_stdio.h
	$(CC) $(CFLAGS) -c -fPIC $^

clean:
	rm -f *.o *.so

# Personal use
move_library: libso_stdio.so
	mv ./libso_stdio.so checker-lin

pack:
	zip -r DochianAlexandruAdrian335CC so_stdio.c so_stdio.h Makefile README

run: main
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.  ./main
