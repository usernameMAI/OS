#include <iostream>

extern "C" int PrimeCount(int A, int B);
extern "C" float E(int x);

int main(){
    int command;
    while((std::cout << "Enter command: ") && (std::cin >> command)){
        if(command == 1){
            std::cout << "Enter A and B: ";
            int A, B;
            std::cin >> A >> B;
            std::cout << "PrimeCount in [A; B] " << PrimeCount(A, B) << std::endl;
        }
        else if(command == 2){
            int x;
            std::cout << "Enter precision: ";
            std::cin >> x;
            std::cout << "Approximate exponent value " << E(x) << std::endl;
        }
    }
}