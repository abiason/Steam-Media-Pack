CC := gcc

TARGET := controller-bridge

SRC_DIR := src
INC_DIR := include
BUILD_DIR := build

SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
DEPENDENCIES := $(OBJECTS:.o=.d)

SDL2_CFLAGS := $(shell pkg-config --cflags sdl2)
SDL2_LIBS := $(shell pkg-config --libs sdl2)

CPPFLAGS := -I$(INC_DIR) $(SDL2_CFLAGS)
CFLAGS := -std=c11 -O2 -Wall -Wextra -Wpedantic
LDFLAGS :=
LDLIBS := $(SDL2_LIBS) -lm

.PHONY: all clean debug release install uninstall

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

debug: CFLAGS := -std=c11 -O0 -g3 -Wall -Wextra -Wpedantic
debug: clean all

release: CFLAGS := -std=c11 -O2 -DNDEBUG -Wall -Wextra -Wpedantic
release: clean all

install: $(TARGET)
	install -Dm755 $(TARGET) \
		$(DESTDIR)/usr/local/bin/$(TARGET)

uninstall:
	rm -f $(DESTDIR)/usr/local/bin/$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPENDENCIES)
