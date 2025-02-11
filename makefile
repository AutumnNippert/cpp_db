OUT_DIR = out
GCC = g++
CFLAGS = -g -Wall -std=c++23

all: main.cpp cpp_db.hpp
	mkdir -p $(OUT_DIR)
	$(GCC) $(CFLAGS) -o out/main main.cpp cpp_db.hpp