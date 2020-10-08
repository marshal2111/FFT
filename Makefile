all: FFT
FFT: main.o
	g++ main.o -o main -lGL -lglfw -lGLEW  
	
main.o: main.cpp
	g++ -c main.cpp 
	
clean:
	rm -rf *o main

