#include <cstdlib>
#include <iostream>
#include <dlfcn.h>

int main(){
    std::cout << "Enter num library: ";
    int lib_num;
    std::cin >> lib_num;
    if(lib_num < 1 || lib_num > 2){
        std::cout << "error lib\n";
        exit(1);
    }
    --lib_num;
    int command;
    const char* libs[] = {"libd1.so", "libd2.so"};
    void* library_handle;
    library_handle = dlopen (libs[lib_num], RTLD_LAZY);
    if(!library_handle){
        std::cout << "Error in dlopen\n";
        exit(1);
    }

    int (*PrimeCount)(int A, int B);
    float (*E)(int x);

    PrimeCount = (int(*)(int, int))dlsym(library_handle, "PrimeCount");
    E = (float(*)(int))dlsym(library_handle, "E");

    std::cout << "Enter command 0, 1 or 2\n";
    while(std::cin >> command) {
        switch (command) {
            case 0:
                dlclose(library_handle);
                lib_num = (lib_num + 1) % 2;
                library_handle = dlopen(libs[lib_num], RTLD_LAZY);
                if(!library_handle){
                    std::cout << "Error in dlopen\n";
                    exit(1);
                }
                PrimeCount = (int(*)(int, int))dlsym(library_handle, "PrimeCount");
                E = (float(*)(int))dlsym(library_handle, "E");
                std::cout << "Change contract\n";
                break;
            case 1:
                std::cout << "Enter A and B: ";
                int A, B;
                std::cin >> A >> B;
                std::cout << "PrimeCount in [A; B] " << PrimeCount(A, B) << std::endl;
                break;
            case 2:
                int x;
                std::cout << "Enter precision: ";
                std::cin >> x;
                std::cout << "Approximate exponent value " << E(x) << std::endl;
                break;
            default:
                std::cout << "Enter 0, 1 or 2!\n";
                break;
        }
    }
    dlclose(library_handle);
}