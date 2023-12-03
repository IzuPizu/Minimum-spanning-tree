
EXECUTABLE = tema3

SOURCE_FILES = tema3.c

.PHONY: all build run clean

all: build

build:
	touch tema3.out
	gcc -std=gnu11 tema3.c  -o $(EXECUTABLE)

run:
	./$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
