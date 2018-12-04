CC := g++
SOURCE := $(wildcard *.cpp algo/*.cpp)
TARGET := run

$(TARGET): $(SOURCE)
	$(CC) -std=c++11 $(SOURCE) -o $(TARGET)

clean:
	rm -rf $(TARGET)
