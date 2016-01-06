# Designate compiler
	CXX = g++

# Put compiler flags here
	FLAGS = -std=c++11 -Werror -Wall -Wextra -pedantic -O3

# Put all files here
	OBJECTS = simulate2048.cpp game.cpp table.cpp board.cpp

# Name the executable
	EXECUTABLE = 2048

compile:
	$(CXX) $(FLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -vf *.out 2048