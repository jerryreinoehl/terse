NAME = terse

CC := c++
#CFLAGS := -Wall -Wextra -Werror -Weffc++ -O2 -std=c++11
CFLAGS := -Wall -Wextra -O2
TARGET = trs
#SRCS = $(wildcard src/*.cpp src/**/*.cpp)
SRCS = src/terse/lexer.cpp src/main.cpp src/terse/token.cpp src/terse/parser.cpp src/terse/emission.cpp src/terse/error.cpp src/args.cpp
HDRS = $(wildcard src/*.h src/**/*.h)
OBJS = $(patsubst src/%.cpp, build/%.o, $(SRCS))
INCS = -Iinclude

DESTDIR :=
PREFIX := /usr
BIN := $(DESTDIR)/$(PREFIX)/bin

VERSION := 0.0.1
RELEASE = $(NAME)-$(VERSION).tar.gz

.PHONY: all
all: build build/$(TARGET)

build/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: src/%.cpp $(HDRS)
	$(CC) $(CFLAGS) $(INCS) -c -o $@ $<

build:
	mkdir -p ./build/terse

.PHONY: install
install: build/$(TARGET)
	mkdir -p $(BIN)
	strip build/$(TARGET)
	install -m 0755 build/$(TARGET) $(BIN)/$(TARGET)

.PHONY: uninstall
uninstall:
	rm -rf $(BIN)/$(TARGET)

.PHONY: release
release: $(RELEASE)
$(RELEASE):
	tar -czvf $@ --transform 's|^|$(NAME)-$(VERSION)/|' src/ Makefile

.PHONY: pkg
pkg: release
	makepkg -s

.PHONY: clean
clean:
	rm -rf ./build
