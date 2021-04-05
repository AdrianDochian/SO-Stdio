CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -L .

all: checker-lin/libso_stdio.so
	mv libso_stdio.so ../checker_lin

move_library: libso_stdio.so
	mv ./libso_stdio.so checker-lin

libso_stdio.so: so_stdio.o
	$(CC) -shared -o $@ $^

so_stdio.o: src/so_stdio.c src/so_stdio.h
	$(CC) $(CFLAGS) -c $^

run: main
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.  ./main

clean:
	rm -f *.o *.so src/*.gch
