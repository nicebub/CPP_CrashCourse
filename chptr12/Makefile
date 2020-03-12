CC=g++-9
CXX=g++-9
#CXX= clang++
#CPPFLAGS= -std=c++11 -g -fconcepts
CATCH_PATH=../chptr10/Catch2/single_include/catch2/
HIPPO_PATH=../chptr10/hippomocks/HippoMocks/
CPPFLAGS= -std=c++17 -g -I. -I$(CATCH_PATH) -I$(HIPPO_PATH)
#CPPFLAGS += -stdlib=libc++
CATCH = $(CATCH_PATH)catch.hpp
HIPPO = $(HIPPO_PATH)..
FILES = $(subst $(wildcard ./),, $(wildcard ./*.cpp))
EXECS = exercise1_test passwordGenerator
EX1 = $(wildcard ./exercise1*)
OBJECTS= $(patsubst passwordGenerator%.cpp, passwordGenerator%.o, $(FILES))
CREATED= 
all: $(EXECS) $(CATCH) $(HIPPO)

%: %.o

%.o: %.cpp

exercise1_test: $(EX1)
	$(CXX) $(CPPFLAGS) $^ -o $@

clean:
	rm $(EXECS) $(CREATED) $(EX1OBJECTS); rm -rf *.dSYM

.PHONY: all clean
