BIN=./bin/

all:
	$(CXX) -pthread -std=c++11 -Wall -Wextra main.cpp -o $(BIN)ScottyCPU
