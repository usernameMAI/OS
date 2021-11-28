#include <cmath>

extern "C" int PrimeCount(int A, int B);
extern "C" float E(int x);

int PrimeCount(int A, int B) {
    int count = 0;
    if (B < 2)
        return 0;
    if (A < 3) {
        A = 3;
        ++count;
    }
    for (int number = A; number <= B; ++number) {
        for (int divider = 2; divider < number; ++divider) {
            if (number % divider == 0)
                break;
            if (divider == number - 1)
                ++count;
        }
    }
    return count;
}

float E(int x){
   if(x < 0)
       return -1;
   return pow(1.0 + 1.0 / x, x);
}
