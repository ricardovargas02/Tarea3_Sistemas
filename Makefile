CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

all: simulador

simulador: main.cpp disco/disco.cpp disco/disco.h asignacion/asignacion.cpp asignacion/asignacion.h
	$(CXX) $(CXXFLAGS) main.cpp disco/disco.cpp asignacion/asignacion.cpp -o simulador

clean:
	rm -f simulador

.PHONY: all clean