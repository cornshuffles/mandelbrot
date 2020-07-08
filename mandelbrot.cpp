#include <iostream>
#include <fstream> // for files manipulation
#include <complex> // for complex numbers
#include <cmath> // round
using namespace std;

//Resolution of the image
int pixelHeight = 18000;
int pixelWidth = 24000;

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
		my_Image << "P3\n" << pixelHeight << " " << pixelWidth << " 255\n";


		//iterate over each pixel
		for(int xPixelCoord = 0; xPixelCoord < pixelWidth; xPixelCoord++){
			for(int yPixelCoord = 0; yPixelCoord < pixelHeight; yPixelCoord++){
				
				//Convert pixel coords to unreal plane coords (x,iy)
				long double x = map(xPixelCoord, 0, pixelWidth, minimumX, maximumX);
				long double y = map(yPixelCoord, 0, pixelHeight, minimumY, maximumY);

				//Coords of point to evaluate
				long double xCoord = x;
				long double yCoord = y;

				int loopCounter = 0;

				/*----------------------------------------------------------------------------------------------*
				 * mandelbrot iteration --- Zn+1 = Zn^2 + C --- C is the point we are evaluating (a + bi)	*
				 * If for all n, such that n < MAX_ITERATIONS, Zn < 2, C is in the mandelbrot set		*
				 * Z0 = C = x + iy										*
				 * Z1 = C^2 + C = (x + iy)^2 + C = x^2 + 2xyi - y^2 + C						*
				 *----------------------------------------------------------------------------------------------*/
				for(int i = 0; i < MAX_ITERATIONS; i++){

					long double x1 = (x * x) - (y * y);
					long double y1 = 2 * x * y;

					x = x1 + xCoord;
					y = y1 + yCoord;

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

				//Write pixel values to the file
				my_Image << r << ' ' << g << ' ' << b << "\n";

				if(xPixelCoord % 100 == 0 && yPixelCoord % 100 == 0){
					cout << "Evaluating Point X:" << xPixelCoord << " Y:" << yPixelCoord << "\r" << flush;
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
