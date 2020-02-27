CC=g++-9
CXX=g++-9
#CXX= clang++
#CPPFLAGS= -std=c++11 -g -fconcepts
CPPFLAGS= -std=c++14 -g -I. -ICatch2/single_include/catch2/ -Ihippomocks/HippoMocks/
#CPPFLAGS += -stdlib=libc++
CATCH = Catch2/single_include/catch2/catch.hpp
HIPPO = hippomocks/
FILES = $(subst $(wildcard ./),, $(wildcard ./*.cpp))
EXECS = autobrake_test
OBJECTS= $(patsubst %.cpp, %.o, $(FILES))
all: $(EXECS) $(CATCH) $(HIPPO)

#%: %.o

#%.o: %.cpp

autobrake_test: $(OBJECTS)
	$(CXX) $(CPPFLAGS) $^ -o $@

clean:
	rm $(EXECS) $(OBJECTS); rm -rf *.dSYM

.PHONY: all clean
