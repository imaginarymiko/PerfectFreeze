CC = gcc
CFLAGS = -std=c17 -D_DEFAULT_SOURCE -Wall -Wextra -std=c11
LDFLAGS = -lcrypto -lssl -lcurl
SRC_DIR = src
OBJ_DIR = bin
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS = $(wildcard $(SRC_DIR)/*.h)

# It may be a good idea to name it something more innocuous
TARGET = PerfectFreeze

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(TARGET) $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d

$(shell mkdir -p $(OBJ_DIR))
