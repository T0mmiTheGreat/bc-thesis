# file: Makefile
# author: Tomáš Ludrovan
# version: 0.1
# date: 2024-03-12

CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic -g -MD
OBJ = test.o
BIN = a


all: test

.PHONY: all test clean clean-exe clean-o clean-d


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

test: $(BIN)
	./$(BIN)


clean: clean-exe clean-o clean-d

clean-exe:
	rm -f $(BIN)

clean-o:
	rm -f $(OBJ)

clean-d:
	rm -f $(OBJ:.o=.d)

-include $(OBJ:.o=.d)
