FLAGS = -std=c++23 -march=znver2 -mtune=znver2 -fsanitize=leak,address
ECCODES_DIR = eccodes-2.33.0
FMT_DIR = fmt
INCLUDE_DIR = include
SRC_DIR = src
OBJ_DIR = obj

$(OBJ_DIR)/getKeys.o: $(SRC_DIR)/getKeys.cpp
	g++ $(FLAGS) -c -o $@ $< -I$(ECCODES_DIR)/include

getKeys: $(OBJ_DIR)/getKeys.o
	g++ $(FLAGS) -o $@ $^ -L$(ECCODES_DIR)/lib -leccodes

main: $(OBJ_DIR)/main.o
	g++ $(FLAGS) -o $@ $^ -L$(ECCODES_DIR)/lib -leccodes -L$(FMT_DIR)/lib -lfmt

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp
	g++ $(FLAGS) -c -o $@ $< -I$(ECCODES_DIR)/include -I$(FMT_DIR)/include

# utils: $(OBJ_DIR)/utils.o

# $(OBJ_DIR)/utils.o: $(SRC_DIR)/utils.cpp $(INCLUDE_DIR)/utils.hpp
# 	g++ $(FLAGS) -c -o $@ $< -I$(INCLUDE_DIR)

clean:
	rm -rf $(OBJ_DIR)/* app