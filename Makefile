BIN=./bin/

all:
	$(CXX) -pthread -std=c++0x -Wall -Wextra main.cpp -o $(BIN)ScottyCPU
