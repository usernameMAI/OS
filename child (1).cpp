#include <stdio.h>
#include <unistd.h>
using namespace std;
int main(){
    double a;
    char c;
    double res = 0;
    int k = 0;
    while(scanf("%lf%c", &a, &c) != EOF) {
        res += a;
        if(c == '\n') {
            printf("%lf\n",res);
            //Сбрасывает буферы потока
            fflush(stdout);            
            res = 0.;
            continue;
        }
    }
    return 0;
}
