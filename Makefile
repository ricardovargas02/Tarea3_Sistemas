CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

all: simulador

simulador: main.cpp disco/disco.cpp disco/disco.h
	$(CXX) $(CXXFLAGS) main.cpp disco/disco.cpp -o simulador

clean:
	rm -f simulador

.PHONY: all clean