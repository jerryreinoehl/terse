CC := c++
CFLAGS := -Wall -Wextra -Werror -Weffc++
TARGET = tiny
SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp, build/%.o, $(SRCS))

.PHONY: all
all: build build/$(TARGET)

build/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

build:
	mkdir -p ./build

.PHONY: clean
clean:
	rm -rf ./build
