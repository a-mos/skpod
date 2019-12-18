vpath %.cpp ./
vpath %.h ./
CFLAGS = -I/usr/local/opt/llvm/include -Wall
CC = /usr/local/opt/llvm/bin/clang

default: integral
run: integral
	mpirun -np 2 ./integral
integral: main.c
	mpicc -o integral main.c -O3
clean:
	rm -f integral