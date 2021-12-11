CXX = g++
CXXFLAGS = -std=c++17 -Wall -Werror -O3

SRCS = $(wildcard day*.cpp)
PROGS = $(patsubst %.cpp,obj/%.o,$(SRCS))

obj/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.cpp: generate_main.py
	./generate_main.py

main: $(PROGS)
