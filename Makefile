vpath %.cpp ./
vpath %.h ./
CFLAGS = -I/usr/local/opt/llvm/include -fopenmp -Wall -O3
CC = /usr/local/opt/llvm/bin/clang

default: integral
run: integral
	./integral 3 -1 1 100000000
integral: main.c
	$(CC) main.c $(CFLAGS) -o $@
clean:
	rm -f integral