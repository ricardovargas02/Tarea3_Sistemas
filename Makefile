CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

all: simulador

simulador: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o simulador

clean:
	rm -f simulador

.PHONY: all clean