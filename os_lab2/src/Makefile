.PHONY: all clean

all:
	g++ child.cpp -o child 
	g++ parent.cpp -o main -fsanitize=address
clean:
	rm child main
