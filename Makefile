all: compile

CC = g++
CFLAGS = -Wall -Wextra
TARGET=fetch

compile: src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp -o $(TARGET)

run: compile
	./$(TARGET)

clean: $(TARGET)
	rm -f $(TARGET)
