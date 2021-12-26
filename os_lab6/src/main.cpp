#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <sys/types>
#include <unistd.h>

using namespace std;

int main() {
    int cnt = 0;
    while(true){
        ++cnt;
        fork();
        cout << cnt << endl;

    }
}
