CC = g++
CFLAGS = -Wall -Wextra
TARGET=fetch

default: help

compile: src/main.cpp ## Just compile
	$(CC) $(CFLAGS) src/main.cpp -o $(TARGET)

run: compile ## Compile and run the Program
	./$(TARGET)

clean: $(TARGET) ## Clean the binary file
	rm -f $(TARGET)

install: compile ## install fetch to the bin
	@cp -r fetch ~/.local/bin/
	@echo "installed fetch"

.PHONY: help

# https://marmelab.com/blog/2016/02/29/auto-documented-makefile.html
help:
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'
