CXX := clang++
CXXFLAGS := -std=c++20 -Wall -Weffc++ -Wconversion -Wsign-conversion -Wextra -pedantic-errors -g -Ofast

TARGET := aoc2024

SRC_DIR := src
SRCS := $(shell find $(SRC_DIR) -type f -name '*.cpp')
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
