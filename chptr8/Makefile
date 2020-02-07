CC=g++
CXX=g++
#CXX= clang++
#CPPFLAGS= -std=c++11 -g -fconcepts
CPPFLAGS= -std=c++17 -g
#-stdlib=libc++
FILES = $(subst $(wildcard ./),, $(wildcard ./*.cpp))
EXECS = $(patsubst %.cpp, %, $(FILES))
OBJECTS= $(patsubst %.cpp, %.o, $(FILES))
all: Fibonacci PrimeNumber fibnprime

#%: %.o

%.o: %.cpp %.h
	$(CC) $(CPPFLAGS) $< -c
	
Fibonacci: FibonacciIR.o Fibonacci.cpp
	$(CC) $(CPPFLAGS) $^ -o $@

PrimeNumber: PrimeNumberIR.o PrimeNumber.cpp
	$(CC) $(CPPFLAGS) $^ -o $@

fibnprime: FibonacciIR.o PrimeNumberIR.o fibnprime.cpp
	$(CC) $(CPPFLAGS) $^ -o $@

clean:
	rm $(EXECS) $(OBJECTS); rm -rf *.dSYM

.PHONY: all clean
