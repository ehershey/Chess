all: chess

includes = $(wildcard *.h)

chess: chess.cpp ${includes}
	g++ $< -o $@ -Wall -Wextra

