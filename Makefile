CC := g++
SOURCE := $(wildcard *.cpp algo/*.cpp)
TARGET := run

$(TARGET): $(SOURCE)
	$(CC) -std=c++14 -O2 -Wall $(SOURCE) -o $(TARGET)

clean:
	rm -rf $(TARGET)
