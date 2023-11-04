# Kendra Ferguson Z1933361
# Makefile for Assignment 4

CXX = g++
CXXFLAGS = -Wall -std=c++14

all: z1933361_project4 z1933361_project4_p2

z1933361_project4: z1933361_project4.o
	$(CXX) $(CXXFLAGS) -o z1933361_project4 z1933361_project4.o -lpthread

z1933361_project4_p2: z1933361_project4_p2.o
	$(CXX) $(CXXFLAGS) -o z1933361_project4_p2 z1933361_project4_p2.o -lpthread

z1933361_project4.o: z1933361_project4.cc
	$(CXX) $(CXXFLAGS) -c z1933361_project4.cc

z1933361_project4_p2.o: z1933361_project4_p2.cc
	$(CXX) $(CXXFLAGS) -c z1933361_project4_p2.cc

clean:
	rm -f z1933361_project4 z1933361_project4_p2 *.o