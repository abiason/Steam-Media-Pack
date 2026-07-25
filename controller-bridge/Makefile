CC ?= cc
PKG_CONFIG ?= pkg-config

TARGET := controller-bridge
SRC_DIR := src
SOURCE := $(SRC_DIR)/controller-bridge.c

CFLAGS ?= -O2
CFLAGS += -Wall -Wextra -Wpedantic
CFLAGS += $(shell $(PKG_CONFIG) --cflags sdl2)

LDLIBS += $(shell $(PKG_CONFIG) --libs sdl2)
LDLIBS += -lm

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET) $(LDLIBS)

clean:
	rm -f $(TARGET)

install: $(TARGET)
	install -Dm755 $(TARGET) \
		$(DESTDIR)$(HOME)/.local/share/steam-media-pack/controller-bridge/$(TARGET)
