##############
## Settings ##
##############
EXECUTABLE_NAME=ScottyCPU
BIN=bin
BLOG=build_log.txt

##############
##  Build   ##
##############
.PHONY: build clean

build: debug
	@echo Build successful!
	
test:
	@echo Running build...
	@IF EXIST "$(BIN)\$(EXECUTABLE_NAME).exe" ( \
		$(BIN)\$(EXECUTABLE_NAME).exe -d \
	) else ( \
		@echo No build present. Run debug or release first. \
	)
	
debug: prepare
	@echo Compiling debug...
	$(CXX) -pthread -std=c++0x -Wall -Wextra main.cpp -o $(BIN)/$(EXECUTABLE_NAME) > $(BIN)/$(BLOG)
	
release: prepare
	@echo Compiling release...
	$(CXX) -pthread -std=c++0x -Wall -Wextra -O2 -s main.cpp -o $(BIN)/$(EXECUTABLE_NAME) > $(BIN)/$(BLOG)
	@echo Build successful!
	
debug_VS2013: prepare
	@echo Compiling release...
	$(CXX) -Wall main.cpp -o $(BIN)/$(EXECUTABLE_NAME) > $(BIN)/$(BLOG)
	@del main.obj
	@echo Build successful!

release_VS2013: prepare
	@echo Compiling release...
	$(CXX) -Wall -O2 main.cpp -o $(BIN)/$(EXECUTABLE_NAME) > $(BIN)/$(BLOG)
	@del main.obj
	@echo Build successful!
	
clean:
	@echo Delete bin folder...
	@IF EXIST "$(BIN)" @rmdir /S /Q $(BIN)
	
create_directories:
	@echo Create bin folder...
	@IF NOT EXIST "$(BIN)" @mkdir $(BIN)

prepare: clean create_directories
