CC := c++
CFLAGS := -Wall -Wextra -Werror -Weffc++
TARGET = tiny
SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp, build/%.o, $(SRCS))

DESTDIR :=
PREFIX := /usr
BIN := $(DESTDIR)/$(PREFIX)/bin

VERSION := 0.0.1
RELEASE = $(TARGET)-$(VERSION).tar.gz

.PHONY: all
all: build build/$(TARGET)

build/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

build:
	mkdir -p ./build

.PHONY: install
install: build/$(TARGET)
	mkdir -p $(BIN)
	strip build/$(TARGET)
	install -m 0755 build/$(TARGET) $(BIN)/$(TARGET)

.PHONY: release
release: $(RELEASE)
$(RELEASE):
	tar -czvf $@ --transform 's|^|$(TARGET)-$(VERSION)/|' src/ Makefile

.PHONY: pkg
pkg: release
	makepkg -s

.PHONY: clean
clean:
	rm -rf ./build
