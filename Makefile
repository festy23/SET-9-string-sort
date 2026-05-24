CXX := /usr/bin/g++
CXXFLAGS := -std=c++23 -O2 -Wall -Wextra
TARGET := string_sort_test

SRC := main.cpp \
       string_generator.cpp \
       sort_algorithms.cpp \
       string_sort_tester.cpp

OBJ := $(SRC:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: $(TARGET)
	mkdir -p data
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
