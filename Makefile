##############
## Settings ##
##############
EXECUTABLE_NAME=ScottyCPU
BIN=bin
BLOG=build_log.txt
RESOURCES=$(BIN)\ScottyResources_res.o

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
	
debug: prepare prepare-resources
	@echo Compiling debug...
	$(CXX) -pthread -std=c++0x -Wall -Wextra main.cpp -o $(BIN)/$(EXECUTABLE_NAME) $(RESOURCES) > $(BIN)/$(BLOG)
	$(MAKE) cleanup
	@echo Build successful!
	
debug-extra: prepare prepare-resources
	@echo Compiling debug with extra's...
	$(CXX) -m32 -g -fno-inline -fno-omit-frame-pointer -pthread -std=c++0x -Wall -Wextra main.cpp -o $(BIN)/$(EXECUTABLE_NAME) $(RESOURCES) > $(BIN)/$(BLOG)
	$(MAKE) cleanup
	@echo Build successful!
	
release: prepare prepare-resources
	@echo Compiling release...
	$(CXX) -pthread -std=c++0x -Wall -Wextra -O2 -s main.cpp -o $(BIN)/$(EXECUTABLE_NAME) $(RESOURCES) > $(BIN)/$(BLOG)
	$(MAKE) cleanup
	@echo Build successful!
	
debug_VS2013: prepare
	@RC /nologo ScottyResources.rc
	@echo Compiling release...
	$(CXX) -Wall main.cpp -o $(BIN)/$(EXECUTABLE_NAME) ScottyResources.res > $(BIN)/$(BLOG)
	$(MAKE) cleanup
	@echo Build successful!

release_VS2013: prepare
	@RC /nologo ScottyResources.rc
	@echo Compiling release...
	$(CXX) -Wall -O2 main.cpp -o $(BIN)/$(EXECUTABLE_NAME) ScottyResources.res > $(BIN)/$(BLOG)
	$(MAKE) cleanup
	@echo Build successful!
	
clean:
	@echo Delete bin folder...
	@IF EXIST "$(BIN)" @del /f /s /q $(BIN) 1>nul
	@IF EXIST "$(BIN)" @rmdir /S /Q $(BIN)
	@IF EXIST "$(BIN)" @rmdir /S /Q $(BIN)

cleanup:
	@IF EXIST "$(RESOURCES)" @del $(RESOURCES)
	@IF EXIST "main.obj" @del main.obj
	@IF EXIST "ScottyResources.res" @del ScottyResources.res
	
create_directories:
	@echo Create bin folder...
	@IF NOT EXIST "$(BIN)" @mkdir $(BIN)

prepare: clean create_directories

prepare-resources:
	@echo Compile $(RESOURCES)...
	@windres -i ScottyResources.rc -o $(RESOURCES) --include-dir=.
