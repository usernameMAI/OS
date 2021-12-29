#include <list>
#include <iostream>
#include <algorithm>

class PoolAllocator {

    struct Node {
        char *start;
        size_t capacity;
        bool availability;

        void Print() const {
            std::cout << "Node: capacity " << capacity << ", type " << (availability ? "Free" : "Busy") << std::endl;
        }

    };

public:

    PoolAllocator(size_t size) {
        data = (char *) malloc(size);
        Node node = {data, size, true};
        blocks.push_front(node);
    }

    ~PoolAllocator() {
        free(data);
    }

    void *alloc(size_t size) {

        if (size <= 0)
            return nullptr;

        size_t size_of_node = 0;
        auto needed_node = blocks.end();

        for (auto it = blocks.begin(); it != blocks.end(); ++it) {
            if (it->availability && it->capacity >= size) {
                size_of_node = it->capacity;
                needed_node = it;
                break;
            }
        }

        if (size_of_node == 0) {
            std::cout << "no alloc" << std::endl;
            exit(1);
        }

        if (size == size_of_node) {
            needed_node->availability = false;
        } else {
            Node new_node = {needed_node->start + size, needed_node->capacity - size, true};
            needed_node->capacity = size;
            needed_node->availability = false;
            blocks.insert(std::next(needed_node), new_node);
        }

        return (void *) (needed_node->start);

    }

    void dealloc(void *ptr) {

        auto it = std::find_if(blocks.begin(), blocks.end(), [ptr](const Node &node) {
            return node.start == (char *) ptr && !node.availability;
        });

        if (it == blocks.end()) {
            std::cout << "no alloc" << std::endl;
            exit(1);
        }

        it->availability = true;

        if (it != blocks.begin() && std::prev(it)->availability) {
            auto prev_it = std::prev(it);
            prev_it->capacity += it->capacity;
            blocks.erase(it);
            it = prev_it;
        }

        if (std::next(it) != blocks.end() && std::next(it)->availability) {
            auto next_it = std::next(it);
            it->capacity += next_it->capacity;
            blocks.erase(next_it);
        }

    }

    void PrintStatus() const {
        int occ_sum = 0;
        int free_sum = 0;

        for (const Node &bl: blocks) {
            bl.Print();
            if (bl.availability) {
                free_sum += bl.capacity;
            } else {
                occ_sum += bl.capacity;
            }
        }

        std::cout << "Occupied memory " << occ_sum << std::endl;
        std::cout << "Free memory " << free_sum << std::endl << std::endl;

    }

private:
    char *data;
    std::list<Node> blocks;
};
