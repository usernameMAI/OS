#include <vector>
#include <list>
#include <iostream>
#include <algorithm>

struct Degrees2{

    unsigned int bl_16 = 0;
    unsigned int bl_32 = 0;
    unsigned int bl_64 = 0;
    unsigned int bl_128 = 0;
    unsigned int bl_256 = 0;
    unsigned int bl_512 = 0;
};

class Degree2Allocator{

public:

    Degree2Allocator(const Degrees2& init_data):lists((index_to_size.size())) {

        std::vector<unsigned int> mem_sizes = {
                init_data.bl_16, init_data.bl_32, init_data.bl_64, init_data.bl_128, init_data.bl_256, init_data.bl_512
        };

        unsigned int sum = 0;
        for(int i = 0; i < mem_sizes.size(); ++i){
            sum += mem_sizes[i] * index_to_size[i];
        }

        data = (char*)malloc(sum);
        char* copy_data = data;

        for(int i = 0; i < mem_sizes.size(); ++i){
            for(int j = 0; j < mem_sizes[i]; ++j){
                lists[i].push_back(copy_data);
                *((int*)copy_data) = (int)index_to_size[i];
                copy_data += index_to_size[i];
            }
        }

        mem_size = sum;

    }

    ~Degree2Allocator(){
        free(data);
    };


    void* alloc(size_t mem_size){

        if(mem_size <= 0)
            return nullptr;

        mem_size += sizeof(int);
        int idx = -1;

        for(size_t i = 0; i < lists.size(); ++i){
            if(index_to_size[i] >= mem_size && !lists[i].empty()){
                idx = i;
                break;
            }
        }

        if(idx == -1){
            std::cout << "Error" << std::endl;
        }

        char* to_return = lists[idx].front();
        lists[idx].pop_front();
        return (void*)(to_return + sizeof(int));
    }

    void dealloc(void* ptr){
        char* c_ptr = (char*)(ptr);
        c_ptr = c_ptr - sizeof(int);
        int block_size = *((int*)c_ptr);
        int idx = std::lower_bound(index_to_size.begin(), index_to_size.end(), block_size) - index_to_size.begin();

        if(idx == index_to_size.size()){
            std::cout << "Error alloc" << std::endl;
        }

        lists[idx].push_back(c_ptr);
    }

    void PrintStatus(){
        int free_sum = 0;

        for(size_t i = 0; i < lists.size(); ++i){
            std::cout << "List with" << index_to_size[i] << " byte blocks, size: " << lists[i].size() << std::endl;
            free_sum += lists[i].size() * index_to_size[i];
        }

        int occ_sum = mem_size - free_sum;

        std::cout << "Occupied memory " << occ_sum << std::endl;
        std::cout << "Free memory " << free_sum << std::endl << std::endl;

    }

private:
    const std::vector<int> index_to_size = {16,32,64,128,256,512,1024};
    std::vector<std::list<char*>> lists;
    char* data;
    int mem_size;
};
