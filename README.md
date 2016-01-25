## INTRODUCTION

Welcome to the 2048 simulator, in this you will find my attempt to use expectimax to achieve the 16384 block 100% of the time. Achieves the 2048 tile in 39 seconds!

No where on the internet can you find an algorithm that gets to 16384 more than 50% of the time using less than 10 look ahead steps. 

## RUNNING THE CODE

### C++ Version
This version is used primarily for code debugging and running it fast! Compile using:
	make print
to print the state of the game every time the AI makes a move. The program will write the output to a file you specify at the end. Compile using:
	make file
to output only to the file. (WARNING: if the game goes beyond the 16384 tile, the program might not end for 30 minutes)

There are 3 modes to this game: Human, Bruteforce, and SmartAI. The human version allows you to control the game yourself. The brute force version implements a very simple AI. SmartAI uses Expectimax and is the best AI ever to run this game!

Run the program using:
	./2048 {flag}
{flag} = -h or --human
{flag} = -b or --brute
{flag} = -c or --computer

### Javascript Version: 
This version is used because it is good looking. CSS and Javascript base came from https://github.com/gabrielecirulli/2048. AI came from C++ source code from the C++ version but converted to Javascript using Emscripten

#### UNDER CONSTRUCTION!!

Have fun!!
