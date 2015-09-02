# OSX g++
CC=/usr/local/bin/g++
CFLAGS=-Wall -Wextra
LIB_OMP=-fopenmp
LIBS=$(LIB_OMP)


all: chess

includes = $(wildcard *.h)

chess: chess.cpp ${includes}
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

