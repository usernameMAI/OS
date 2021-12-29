#include <chrono>
#include "PoolAllocator.cpp"
#include "Degree2Allocator.cpp"

using namespace std::chrono;

int main() {

    {
        steady_clock::time_point list_allocator_init_start = steady_clock::now();
        PoolAllocator list_allocator(4096);
        steady_clock::time_point list_allocator_init_end = steady_clock::now();
        std::cout << "List allocator initialization with one page of memory :"
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(
                          list_allocator_init_end - list_allocator_init_start).count()
                  << " ns" << std::endl;


        steady_clock::time_point d2_allocator_init_start = steady_clock::now();
        Degrees2 d = {64, 32, 16, 4, 2};
        Degree2Allocator d2_allocator(d); // 1 страница
        steady_clock::time_point d2_allocator_init_end = steady_clock::now();
        std::cout << "D2 allocator initialization with one page of memory :"
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(
                          d2_allocator_init_end - d2_allocator_init_start).count()
                  << " ns" << std::endl;

        std::cout << "\n";

    }

    std::cout << "First test: Allocate 10 char[256] arrays, free 5 of them, allocate 10 char[128] arrays:\n";
    {
        std::vector<char *> pointers(15, 0);
        Degrees2 d = {0, 0, 32, 20, 20, 10};
        Degree2Allocator allocator(d);
        steady_clock::time_point n2_test1_start = steady_clock::now();
        for (int i = 0; i < 10; ++i) {
            pointers[i] = (char *) allocator.alloc(256);
        }
        for (int i = 5; i < 10; ++i) {
            allocator.dealloc(pointers[i]);
        }
        for (int i = 5; i < 15; ++i) {
            pointers[i] = (char *) allocator.alloc(128);
        }
        steady_clock::time_point n2_test1_end = steady_clock::now();
        std::cout << "D2 allocator first test:"
                  << std::chrono::duration_cast<std::chrono::microseconds>(n2_test1_end - n2_test1_start).count()
                  << " microseconds" << std::endl;
        allocator.PrintStatus();
        for (int i = 0; i < 15; ++i) {
            allocator.dealloc(pointers[i]);
        }
    }
    {
        PoolAllocator allocator(4096);
        std::vector<char *> pointers(1000, 0);
        steady_clock::time_point test1_start = steady_clock::now();
        for (int i = 0; i < 10; ++i) {
            pointers[i] = (char *) allocator.alloc(256);
        }
        for (int i = 5; i < 10; ++i) {
            allocator.dealloc(pointers[i]);
        }
        for (int i = 5; i < 15; ++i) {
            pointers[i] = (char *) allocator.alloc(128);
        }
        steady_clock::time_point test1_end = steady_clock::now();
        std::cout << "List allocator first test:"
                  << std::chrono::duration_cast<std::chrono::microseconds>(test1_end - test1_start).count()
                  << " microseconds" << std::endl;
        allocator.PrintStatus();
        for (int i = 0; i < 15; ++i) {
            allocator.dealloc(pointers[i]);
        }
    }

    {
        PoolAllocator allocator(4096);
        std::vector<char *> pointers(1000, 0);
        steady_clock::time_point test1_start = steady_clock::now();
        for (int i = 0; i < 10; ++i) {
            pointers[i] = (char *) allocator.alloc(256);
        }
        for (int i = 5; i < 10; ++i) {
            allocator.dealloc(pointers[i]);
        }
        for (int i = 5; i < 15; ++i) {
            pointers[i] = (char *) allocator.alloc(128);
        }
        steady_clock::time_point test1_end = steady_clock::now();
        std::cout << "List allocator first test:"
                  << std::chrono::duration_cast<std::chrono::microseconds>(test1_end - test1_start).count()
                  << " microseconds" << std::endl;
        allocator.PrintStatus();
        for (int i = 0; i < 15; ++i) {
            allocator.dealloc(pointers[i]);
        }
    }

    std::cout << "Second test: Allocate and free 750 20 bytes arrays:\n";
    {
        Degrees2 d = {0, 400, 400};
        Degree2Allocator allocator(d);
        std::vector<char *> pointers(750, 0);
        steady_clock::time_point alloc_start = steady_clock::now();
        for (int i = 0; i < 750; ++i) {
            pointers[i] = (char *) allocator.alloc(20);
        }
        steady_clock::time_point alloc_end = steady_clock::now();
        for (int i = 0; i < 750; ++i) {
            allocator.dealloc(pointers[i]);
        }
        steady_clock::time_point test_end = steady_clock::now();
        std::cout << "D2 allocator second test:\n"
                  << "Allocation :" << duration_cast<std::chrono::microseconds>(alloc_end - alloc_start).count()
                  << " microseconds" << "\n"
                  << "Deallocation :" << duration_cast<std::chrono::microseconds>(test_end - alloc_end).count()
                  << " microseconds" << "\n";
    }
    {
        PoolAllocator allocator(16000);
        std::vector<char *> pointers(750, 0);
        steady_clock::time_point alloc_start = steady_clock::now();
        for (int i = 0; i < 750; ++i) {
            pointers[i] = (char *) allocator.alloc(20);
        }
        steady_clock::time_point alloc_end = steady_clock::now();
        for (int i = 0; i < 750; ++i) {
            allocator.dealloc(pointers[i]);
        }
        steady_clock::time_point test_end = steady_clock::now();
        std::cout << "List allocator second test:\n"
                  << "Allocation :" << duration_cast<std::chrono::microseconds>(alloc_end - alloc_start).count()
                  << " microseconds" << "\n"
                  << "Deallocation :" << duration_cast<std::chrono::microseconds>(test_end - alloc_end).count()
                  << " microseconds" << "\n";
    }

    std::cout << "Third test: Allocate 500 20 bytes arrays, deallocate every second, allocate 250 12 bytes :\n";

    {
        Degrees2 d = {400, 700};
        Degree2Allocator allocator(d);
        std::vector<char *> pointers(750, 0);
        steady_clock::time_point test_start = steady_clock::now();
        for (int i = 0; i < 500; ++i) {
            pointers[i] = (char *) allocator.alloc(20);
        }
        for (int i = 0; i < 250; ++i) {
            allocator.dealloc(pointers[i * 2]);
        }
        for (int i = 500; i < 750; ++i) {
            pointers[i] = (char *) allocator.alloc(12);
        }
        steady_clock::time_point test_end = steady_clock::now();
        std::cout << "D2 allocator third test:"
                  << std::chrono::duration_cast<std::chrono::microseconds>(test_end - test_start).count()
                  << " microseconds" << std::endl;
        allocator.PrintStatus();
        for (int i = 0; i < 250; ++i) {
            allocator.dealloc(pointers[i * 2 + 1]);
        }
        for (int i = 500; i < 750; ++i) {
            allocator.dealloc(pointers[i]);
        }
    }
    {
        PoolAllocator allocator(16000);
        std::vector<char *> pointers(750, 0);
        steady_clock::time_point test_start = steady_clock::now();
        for (int i = 0; i < 500; ++i) {
            pointers[i] = (char *) allocator.alloc(20);
        }
        for (int i = 0; i < 250; ++i) {
            allocator.dealloc(pointers[i * 2]);
        }
        for (int i = 500; i < 750; ++i) {
            pointers[i] = (char *) allocator.alloc(12);
        }
        steady_clock::time_point test_end = steady_clock::now();
        std::cout << "\nList allocator third test:"
                  << std::chrono::duration_cast<std::chrono::microseconds>(test_end - test_start).count()
                  << " microseconds" << std::endl;
        allocator.PrintStatus();
        for (int i = 0; i < 250; ++i) {
            allocator.dealloc(pointers[i * 2 + 1]);
        }
        for (int i = 500; i < 750; ++i) {
            allocator.dealloc(pointers[i]);
        }
    }

    std::cout << "Fourth test: Allocate and free 1500 20 bytes arrays:\n";

    {
        Degrees2 d = {0, 800, 800};
        Degree2Allocator allocator(d);
        std::vector<char *> pointers(1500, 0);
        steady_clock::time_point alloc_start = steady_clock::now();
        for (int i = 0; i < 1500; ++i) {
            pointers[i] = (char *) allocator.alloc(20);
        }
        steady_clock::time_point alloc_end = steady_clock::now();
        for (int i = 0; i < 1500; ++i) {
            allocator.dealloc(pointers[i]);
        }
        steady_clock::time_point test_end = steady_clock::now();
        std::cout << "D2 allocator fourth test:\n"
                  << "Allocation :" << duration_cast<std::chrono::microseconds>(alloc_end - alloc_start).count()
                  << " microseconds" << "\n"
                  << "Deallocation :" << duration_cast<std::chrono::microseconds>(test_end - alloc_end).count()
                  << " microseconds" << "\n";
    }
    {
        PoolAllocator allocator(32000);
        std::vector<char *> pointers(1500, 0);
        steady_clock::time_point alloc_start = steady_clock::now();
        for (int i = 0; i < 1500; ++i) {
            pointers[i] = (char *) allocator.alloc(20);
        }
        steady_clock::time_point alloc_end = steady_clock::now();
        for (int i = 0; i < 1500; ++i) {
            allocator.dealloc(pointers[i]);
        }
        steady_clock::time_point test_end = steady_clock::now();
        std::cout << "List allocator fourth test:\n"
                  << "Allocation :" << duration_cast<std::chrono::microseconds>(alloc_end - alloc_start).count()
                  << " microseconds" << "\n"
                  << "Deallocation :" << duration_cast<std::chrono::microseconds>(test_end - alloc_end).count()
                  << " microseconds" << "\n";
    }

}
