all: main.cpp methods.hpp
	g++ -g -o main main.cpp -std=c++23 methods.hpp