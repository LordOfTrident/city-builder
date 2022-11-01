BIN = bin
OUT = $(BIN)/app

SRC  = $(wildcard src/*.cc) $(wildcard src/**/*.cc)
DEPS = $(wildcard src/*.hh) $(wildcard src/**/*.hh)
OBJ  = $(addsuffix .o,$(subst src/,$(BIN)/,$(basename $(SRC))))

BIN_DIRS = $(subst src/,$(BIN)/,$(sort $(dir $(wildcard src/*/))))

CXX       = g++
CXX_VER   = c++17
CXX_FLAGS = -O3 -std=$(CXX_VER) -Wall -Wextra -Werror \
            -pedantic -Wno-deprecated-declarations
CXX_LIBS  = -lSDL2

compile: $(BIN) $(BIN_DIRS) $(OBJ) $(SRC)
	$(CXX) $(CXX_FLAGS) -o $(OUT) $(OBJ) $(CXX_LIBS)
	cp -r res $(BIN)/

$(BIN)/%/:
	mkdir -p $@

bin/%.o: src/%.cc $(DEPS)
	$(CXX) -c $< $(CXX_FLAGS) -o $@

bin:
	mkdir -p $(BIN)

clean:
	rm -r $(BIN)/*

all:
	@echo compile, clean
