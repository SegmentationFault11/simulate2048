#include <cmath>
#include <iostream>


#include <cstdio>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include <sstream>

using namespace std;

class Rand {
  //unsigned long x = 123456789;
  unsigned y = 362436069;
  unsigned z = 521288629;
  unsigned t = 0;
  
public:
  
  Rand(unsigned seed) {
    t = seed;
  }
  
  inline unsigned long rand() {
    t ^= t >> 16;
    t ^= t << 10;
    t ^= t >> 5;
    t ^= t << 1;
    
    z = y;
    y = t ^ z;
    
    return y;
  }
};

int main() {
  unsigned a[10];
  for (int i = 0; i < 10; ++i) {
    a[i] = 0;
  }

  Rand rand_gen(12314232);
  for (int i = 0; i < 100000000; ++i) {
    int num = rand_gen.rand()%10;
    ++a[num];
  }
  for (int i = 0; i < 10; ++i) {
    cout << i << " -> " << a[i] << endl;
  }
  
//  srand((unsigned)time(NULL));
//  for (int i = 0; i < 100000000; ++i) {
//    int num = rand()%10;
//    ++a[num];
//  }
//  
//  for (int i = 0; i < 10; ++i) {
//      cout << i << " -> " << a[i] << endl;
//  }
}
