#Default target - creates executable named mandelbrot

# run and convert image to png
img: mandelbrot
	./mandelbrot

mandelbrot: mandelbrot.o
	g++ mandelbrot.o -o mandelbrot -O3
#mandelbrot target - maintains mandelbrot file	
mandelbrot.o: mandelbrot.cpp
	g++ -c mandelbrot.cpp
#clean target -	removes all files ending in .o
clean:
	rm -i *.o
	rm -i `find ./ -type f -executable`
