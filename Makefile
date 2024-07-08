CC := x86_64-w64-mingw32-gcc
SRC_DIR := src
OBJ_DIR := obj
BUILD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

CFLAGS := -Wall -std=c11 -Wextra
LDFLAGS :=  -lcsfml-graphics -lcsfml-window -lcsfml-system

TARGET := $(BUILD_DIR)/CCraft.exe

all: $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)

.PHONY: all clean
