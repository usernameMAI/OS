#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <fcntl.h>
using namespace std;

int main() {
    string s;
    cout << "Enter a filename: ";
    cin >> s;
    //Преобразуем в const char*, для работы access
    const char* buf = s.c_str();

    //Проверяем, есть ли файл с именем buf
    if(access(buf, R_OK) == -1){
        cout << "File not exist" << endl;
        return -1;
    }

    //Создаём канал данных для межпроцессорного взаимодействия 
    int fd[2];
    pipe(fd);

    //Создаем процесс-потомок
    int pid = fork();
    if(pid == -1){
        perror("fork");
        return -1;
    } 
    else if(pid == 0){
        int file = open(buf, O_RDONLY);
        if (file == -1) {
            perror("no open");
            return -1;
        }
        //перенаправляет 
        if (dup2(fd[1], 1) == -1) {
            exit(1);
        }
        if (dup2(file, 0) == -1) {
            exit(1);
        }
        close(fd[0]);
        close(fd[1]);
        close(file);
        //открывается программа child
        execl("child", "child", NULL);
    }
    close(fd[1]);
    char c;
    //трубка в родительском, вывод в стдаут
    while(read(fd[0], &c, sizeof(char)) > 0){
        putchar(c);
    }
    close(fd[0]);
    return 0;
}
