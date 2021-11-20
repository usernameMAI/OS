#include <iostream>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include<sys/mman.h>

using namespace std;

int main() {
    const int map_size = 1024;
    string filename;
    string s = "";
    const char *input_sem_name = "input_sem";
    const char *output_sem_name = "output_sem";
    sem_unlink(input_sem_name);
    sem_unlink(output_sem_name);

    sem_t *input_semaphore = sem_open(input_sem_name, O_CREAT, S_IWUSR | S_IRUSR, 1);
    sem_t *output_semaphore = sem_open(output_sem_name, O_CREAT, S_IWUSR | S_IRUSR, 0);
    if(input_semaphore == SEM_FAILED || output_semaphore == SEM_FAILED){
        cout << "Error semaphore create" << endl;
        exit(1);
    }

    int map_fd1 = shm_open("map_fd1.txt", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
    int map_fd2 = shm_open("map_fd2.txt", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
    if (map_fd1 == -1 || map_fd2 == -1) {
        cout << "Error creating file for file mapping" << endl;
        exit(1);
    }

    char *memptr1 = (char *) mmap(nullptr, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, map_fd1, 0);
    char *memptr2 = (char *) mmap(nullptr, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, map_fd2, 0);
    if (memptr1 == MAP_FAILED || memptr2 == MAP_FAILED) {
        cout << "Error in file mapping" << endl;
        exit(1);
    }

    cout << "Enter file name: ";
    cin >> filename;
    ifstream file1;
    file1.open(filename);
    if (!file1.is_open()) {
        cout << "File open error" << endl;
        exit(1);
    }

    int id = fork();
    if (id == -1) {
        cout << "Error fork" << endl;
        exit(1);
    } else if (id == 0) {
        sem_wait(output_semaphore);
        sem_wait(input_semaphore);
        sem_post(output_semaphore);

        struct stat st;
        if (fstat(map_fd1, &st)) {
            cout << "Error fstat" << endl;
            exit(1);
        }

        int idx = 0;
        int length_1 = 0;
        for(int i = 0; i <= st.st_size; ++i){
            if (memptr1[i] != '\n') {
                s += memptr1[i];
            } else {
                long double res = 0;
                string result = "";
                double number;
                s += " ";

                for (unsigned j = 0; j < s.length(); ++j) {
                    if (s[j] != ' ') {
                        result += s[j];
                    } else {
                        number = stof(result);
                        res += number;
                        result = "";
                    }
                }

                s = to_string(res) + "\n";
                length_1 += s.length() * sizeof(char);
                if (ftruncate(map_fd2, length_1)) {
                    cout << "Error ftruncate" << endl;
                    exit(1);
                }

                for (unsigned j = 0; j < s.length(); ++j) {
                    memptr2[idx] = s[j];
                    idx += 1;
                }

                s = "";
            }
        }
        sem_post(input_semaphore);
    } else {
        sem_wait(input_semaphore);
        sem_post(output_semaphore);
        int idx = 0;
        int length = 0;

        while (!file1.eof()) {
            getline(file1, s);
            if (s != "") {
                s += "\n";

                length += s.length() * sizeof(char);
                if (ftruncate(map_fd1, length)) {
                    cout << "Error during ftruncate" << endl;
                    exit(1);
                }

                for (unsigned i = 0; i < s.length(); i++) {
                    memptr1[idx] = s[i];
                    idx += 1;
                }
            }
        }
        s = "";
        sem_post(input_semaphore);
        sem_wait(output_semaphore);
        sem_wait(input_semaphore);

        struct stat st;
        if (fstat(map_fd2, &st)) {
            cout << "Error fstat" << endl;
            exit(1);
        }

        for (int i = 0; i <= st.st_size; ++i) {
            if (memptr2[i] != '\n') {
                s += memptr2[i];
            } else {
                s += "\n";
                cout << s;
                s = "";
            }
        }
    }

    munmap(memptr1, map_size);
    munmap(memptr2, map_size);
    shm_unlink("map_fd1.txt");
    shm_unlink("map_fd2.txt");
    remove("map_fd1.txt");
    remove("map_fd2.txt");
    sem_destroy(input_semaphore);
    sem_destroy(output_semaphore);
    close(map_fd1);
    close(map_fd2);
}