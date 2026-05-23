CXX=g++
CC=gcc
CXXFLAGS=-std=c++17 -O2 -Wall -Wextra -pedantic
CFLAGS=-O2 -Wall -Wextra
TARGET=energy_game_server

all: $(TARGET)

$(TARGET): main.o mongoose.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o mongoose.o

main.o: main.cpp mongoose.h
	$(CXX) $(CXXFLAGS) -c main.cpp

mongoose.o: mongoose.c mongoose.h
	$(CC) $(CFLAGS) -c mongoose.c

clean:
	rm -f *.o $(TARGET)
