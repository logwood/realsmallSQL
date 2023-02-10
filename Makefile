source = insert.cpp CreateTable.cpp
target = dlist

CXX = g++-12
CXXFLAG = -Wall -g -std=c++20 -fsanitize=address

LIB =

all:
	$(CXX) $(source) $(CXXFLAG) -o $(target) $(LIB)

clean:
	rm $(target)