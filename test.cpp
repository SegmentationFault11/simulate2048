#include <iostream>
#include <math.h>

using namespace std;

void decToBin(int n) {
    int remainder;
    remainder = n%2;
    if(n > 0) {
        decToBin(n/2);
        cout << remainder;
    }
}

int main(void) {
	decToBin(6);
	return 0;
}