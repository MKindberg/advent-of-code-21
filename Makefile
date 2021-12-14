CXX = g++
CXXFLAGS = -std=c++2a -Wall -Werror

SRCS = $(wildcard day*.cpp)
OBJS = $(patsubst %.cpp,obj/%.o,$(SRCS))

main: $(OBJS)

day% : day%.cpp
	$(CXX) $(CXXFLAGS) -g -D MAIN $< -o $@

% : day%
	./$<

obj/%.o: %.cpp
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -O3 -c $< -o $@

main.cpp: generate_main.py $(OBJS)
	./generate_main.py


.PHONY: new run clean
new:
	./new_day.py
	make clean && bear make main

run: main
	./main

clean:
	rm -f $(OBJS) main main.cpp
