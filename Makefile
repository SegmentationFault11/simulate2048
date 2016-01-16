# Designate compiler
	CXX = g++

# Put compiler flags here
	FLAGS = -std=c++11 -Werror -Wall -Wextra -pedantic -O3

# Put all files here
	OBJECTS = simulate2048.cpp game.cpp 

# Name the executable
	EXECUTABLE = 2048

compilefile:
	$(CXX) $(FLAGS) $(OBJECTS) -o $(EXECUTABLE)

compileprint:
	$(CXX) $(FLAGS) $(OBJECTS) -D OUT -o $(EXECUTABLE)

clean:
	rm -vf *.out *.txt 2048