# Kendra Ferguson Z1933361
# Makefile for Assignment 4

CXX = g++
CXXFLAGS = -Wall -std=c++14

z1933361_project4: z1933361_project4.o 
	$(CXX) $(CXXFLAGS) -o z1933361_project4 z1933361_project4.cc -lpthread

clean:
	rm -f z1933361_project4 *.o
