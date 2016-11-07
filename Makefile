##############
## Settings ##
##############
EXECUTABLE_NAME=ScottyCPU
BIN=bin
LOG=build_log.txt

##############
##  Build   ##
##############
.PHONY: build clean

build: debug
	@echo Build successful!
	
debug: prepare
	@echo Compiling debug...
	$(CXX) -pthread -std=c++0x -Wall -Wextra main.cpp -o $(BIN)/$(EXECUTABLE_NAME) > $(BIN)/$(LOG)
	
release: prepare
	@echo Compiling release...
	$(CXX) -pthread -std=c++0x -Wall -Wextra -O2 -s main.cpp -o $(BIN)/$(EXECUTABLE_NAME) > $(BIN)/$(LOG)
	@echo Build successful!
	
debug_VS2013: prepare
	@echo Compiling release...
	$(CXX) -Wall main.cpp -o $(BIN)/$(EXECUTABLE_NAME) > $(BIN)/$(LOG)
	@del main.obj
	@echo Build successful!

release_VS2013: prepare
	@echo Compiling release...
	$(CXX) -Wall -O2 main.cpp -o $(BIN)/$(EXECUTABLE_NAME) > $(BIN)/$(LOG)
	@del main.obj
	@echo Build successful!
	
clean:
	@echo Delete bin folder...
	@IF EXIST "$(BIN)" @rmdir /S /Q $(BIN)
	
create_directories:
	@echo Create bin folder...
	@IF NOT EXIST "$(BIN)" @mkdir $(BIN)

prepare: clean create_directories
