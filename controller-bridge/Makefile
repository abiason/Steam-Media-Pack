CC ?= cc
PKG_CONFIG ?= pkg-config

TARGET := controller-bridge

SRC := $(wildcard src/*.c)
OBJ := $(SRC:.c=.o)

CFLAGS += -O2
CFLAGS += -Wall -Wextra -Wpedantic
CFLAGS += -Iinclude
CFLAGS += $(shell $(PKG_CONFIG) --cflags sdl2)

LDLIBS += $(shell $(PKG_CONFIG) --libs sdl2)
LDLIBS += -lm

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: clean
