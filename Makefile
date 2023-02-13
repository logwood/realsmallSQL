source = service.cc
target = realsmallSQL

CXX = g++
CXXFLAG = -Wall -g -std=c++20 -fsanitize=address

LIB =

all:
	$(CXX) $(source) $(CXXFLAG) -o $(target) $(LIB)

clean:
	rm $(target)