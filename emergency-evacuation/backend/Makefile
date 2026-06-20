CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = evacuation
SRCS = main.c grid.c bfs.c simulation.c
OBJS = $(SRCS:.c=.o)
TEST_TARGET = run_tests
TEST_SRCS = tests.c grid.c bfs.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    TEST_TARGET := $(TEST_TARGET).exe
endif

all: $(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	-del /Q $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET) 2>nul
else
	rm -f $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET)
endif

.PHONY: all test clean
