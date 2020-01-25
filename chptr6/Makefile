CC=g++-9
CXX=g++-9
#CXX= clang++
CPPFLAGS= -std=c++11 -g -fconcepts
#-stdlib=libc++
FILES = $(subst $(wildcard ./),, $(wildcard ./*.cpp))
EXECS = $(patsubst %.cpp, %, $(FILES))
OBJECTS= $(patsubst %.cpp, %.o, $(FILES))
all: $(EXECS) $(OBJECTS)

#%: %.o

#%.o: %.cpp



clean:
	rm $(EXECS) $(OBJECTS)

.PHONY: all clean
