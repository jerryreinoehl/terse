NAME = terse

CC := c++
CFLAGS := -Wall -Wextra -Werror -Weffc++ -O2
INC = -Iinclude
TARGET = trs
SRCS = $(wildcard src/*.cpp)
HDRS = $(wildcard include/**/*.h)
OBJS = $(patsubst src/%.cpp, build/%.o, $(SRCS))

DESTDIR :=
PREFIX := /usr
BIN := $(DESTDIR)/$(PREFIX)/bin

VERSION := 0.0.1
RELEASE = $(NAME)-$(VERSION).tar.gz

.PHONY: all
all: build build/$(TARGET)

build/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INC) -o $@ $^

build/%.o: src/%.cpp $(HDRS)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

build:
	mkdir -p ./build

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
