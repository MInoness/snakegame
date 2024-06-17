CC=g++
TARGET = snake
OBJECT=src/SnakeControl.o src/Snake.o src/Main.o

all:$(TARGET)

snake: $(OBJECT)
	$(CC) -g -o snake src/Main.cpp src/SnakeControl.cpp src/Snake.cpp -std=c++11 -lncurses

SnakeContorl.o : src/SnakeControl.cpp src/SnakeControl.h
	$(CC) -g -c src/SnakeControl.cpp

Snake.o : src/Snake.cpp src/Snake.h
	$(CC) -g -c src/Snake.cpp

clean:
	rm -rf src/*.o src/*.exe