#Default target - creates executable named mandelbrot
mandelbrot: mandelbrot.o
	g++ mandelbrot.o -o mandelbrot
#mandelbrot target - maintains mandelbrot file	
mandelbrot.o: mandelbrot.cpp
	g++ -c mandelbrot.cpp
#clean target -	removes all files ending in .o
clean:
	rm -i *.o
	rm -i `find ./ -type f -executable`
