#include <vector>

extern "C" int PrimeCount(int A, int B);
extern "C" float E(int x);

int PrimeCount(int A, int B){
    if (B < 2)
        return 0;
    if(A < 2)
        A = 2;
    int n = B;
    std::vector<char> prime(n + 1, true);
    prime[0] = prime[1] = false;
    for(int i = 2; i <= n; ++i){
        if(prime[i]){
            if(i * i <= n){
                for(int j = i * i; j <= n; j += i){
                    prime[j] = false;
                }
            }
        }
    }
    int count = 0;
    for(int i = A; i <= B; ++i)
        count += prime[i];
    return count;
}

float E(int x) {
    float ans = 1;
    if (x < 0)
        return -1;
    for(int i = 1; i <= x; ++i){
        float fact = 1;
        for(int j = 1; j <= i; ++j)
            fact *= j;
        ans += 1.0 / fact;
    }
    return ans;
}