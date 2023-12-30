NAME = terse

CC := c++
CFLAGS := -Wall -Wextra -Werror -Weffc++ -O2
TARGET = trs
SRCS = $(wildcard src/*.cpp)
HDRS = $(wildcard src/*.h)
OBJS = $(patsubst src/%.cpp, build/%.o, $(SRCS))

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
	$(CC) $(CFLAGS) -c -o $@ $<

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
