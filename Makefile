CXX = g++
CXXFLAGS = -std=c++23 -march=znver2 -mtune=znver2 -fsanitize=leak,address
ECCODES_DIR = eccodes
FMT_DIR = fmt
LIBS = -L$(ECCODES_DIR)/lib -leccodes -L$(FMT_DIR)/lib -lfmt
INCLUDES = -I$(ECCODES_DIR)/include -I$(FMT_DIR)/include -Iinclude
SRC_DIR = src
OBJ_DIR = obj

getKeys: $(OBJ_DIR)/getKeys.o
	$(CXX) $(CXXFLAGS) -o $@ $^ -L$(ECCODES_DIR)/lib -Wl,-rpath,./eccodes/lib -leccodes
	
$(OBJ_DIR)/getKeys.o: $(SRC_DIR)/getKeys.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< -I$(ECCODES_DIR)/include

extractSingleFile: $(OBJ_DIR)/extractSingleFile.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) -Wl,-rpath,./eccodes/lib

$(OBJ_DIR)/extractSingleFile.o: $(SRC_DIR)/extractSingleFile.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCLUDES)

extractMultipleFiles: $(OBJ_DIR)/extractMultipleFiles.o  $(OBJ_DIR)/utils.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) -Wl,-rpath,./eccodes/lib
	
$(OBJ_DIR)/extractMultipleFiles.o: $(SRC_DIR)/extractMultipleFiles.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCLUDES)

utils: $(OBJ_DIR)/utils.o

$(OBJ_DIR)/utils.o: $(SRC_DIR)/utils.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCLUDES)

clean:
	rm -rf $(OBJ_DIR)/*