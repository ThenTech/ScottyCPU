##############
## Settings ##
##############
EXECUTABLE_NAME=ScottyCPU
BIN=bin

##############
##  Build   ##
##############
.PHONY: build clean

build: debug
	@echo Build successful!
	
debug: clean create_directories
	@echo Compiling debug...
	$(CXX) -pthread -std=c++0x -Wall -Wextra main.cpp -o $(BIN)/$(EXECUTABLE_NAME)
	
release: clean create_directories
	@echo Compiling release...
	$(CXX) -pthread -std=c++0x -Wall -Wextra -O2 -s main.cpp -o $(BIN)/$(EXECUTABLE_NAME)
	@echo Build successful!
	
clean:
	@echo Delete bin folder...
	@rmdir /S /Q $(BIN)
	
create_directories:
	@echo Create bin folder...
	@mkdir $(BIN)
