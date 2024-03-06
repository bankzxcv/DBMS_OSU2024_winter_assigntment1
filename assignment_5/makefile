# generate make file to build main.cpp in program by using clang
#
CC = g++
CFLAGS = -std=c++11

all: program

program: main.cpp
	$(CC) $(CFLAGS) -o program main.cpp

run: program
	./program

rmm:
	rm ./Drun_* && rm ./Erun_* && rm ./Join.csv

clean:
	rm -f program && rm -f run_*

t:
	$(CC) $(CFLAGS) -o t test.cpp && ./t && rm -f ./t

test: program