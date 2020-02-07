CC=g++
CXX=g++
#CXX= clang++
#CPPFLAGS= -std=c++11 -g -fconcepts
CPPFLAGS= -std=c++17 -g
#-stdlib=libc++
FILES = $(subst $(wildcard ./),, $(wildcard ./*.cpp))
EXECS = $(patsubst %.cpp, %, $(FILES))
OBJECTS= $(patsubst %.cpp, %.o, $(FILES))
all: Fibonacci

#%: %.o

FibonacciIR.o: FibonacciIR.cpp FibonacciIR.h
	$(CC) $(CPPFLAGS) $< -c
	
Fibonacci: FibonacciIR.o Fibonacci.cpp
	$(CC) $(CPPFLAGS) $^ -o Fibonacci

clean:
	rm $(EXECS) $(OBJECTS); rm -rf *.dSYM

.PHONY: all clean
