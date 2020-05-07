#include <iostream>
#include <fstream> // for files manipulation
#include <complex> // for complex numbers
#include <cmath> // round
using namespace std;

int height = 18000;
int width = 24000;

//Window vars
long double minimumX = -2.25;
long double maximumX = .75;

long double minimumY = -1.125;
long double maximumY = 1.125;
//auto calculate maxY to eliminate stretching
//long double maximumY = minimumY + (maximumX - minimumX) * height / width;


int MAX_ITERATIONS = 5000;

//maps given variable from one scale to another
long double map(long double value, long double inMin, long double inMax, long double outMin, long double outMax){
	return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

int main(){

	//open file
	ofstream my_Image ("mandelbrot.ppm");
	if(my_Image.is_open ()) {

		//image size data
		my_Image << "P3\n" << height << " " << width << " 255\n";


		//iterate over each pixel
		for(int i = 0; i < width; i++){
			for(int j = 0; j < height; j++){

				long double x = map(i, 0, width, minimumX, maximumX);
				long double y = map(j, 0, height, minimumY, maximumY);

				long double xi = x;
				long double yi = y;

				int loopCounter = 0;

				//mandelbrot iteration
				for(int i = 0; i < MAX_ITERATIONS; i++){

					long double x1 = (x * x) - (y * y);
					long double y1 = 2 * x * y;

					x = x1 + xi;
					y = y1 + yi;

					if((x + y) > 2){
						break;
					}

					loopCounter++;
				}

				//get a brightness value
				int brightness = map(loopCounter, 0, 200, 0, 255);

				//remove wierd shadow
				if(brightness < 20){
					brightness = 0;
				}

				//mandelbrot set is black
				if(loopCounter == MAX_ITERATIONS){
					brightness = 0;
				}

				int r = map(brightness * brightness, 0, 255 * 255, 0, 255);
				int g = brightness;
				int b = map(sqrt(brightness), 0, sqrt(255), 0, 255);


				my_Image << r << ' ' << g << ' ' << b << "\n";

				if(i % 100 == 0 && j % 1000 == 0){
					cout << "X:" << i << " Y:" << j << "\n";
				}

			}
		}

	//close the ofstream
	my_Image.close();
	cout << "\nfinished\n";
	}
	//handle not opening file
	else{
		cout << "Could not open the file";
	}
	return 0;
}
