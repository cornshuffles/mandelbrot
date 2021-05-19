#include <iostream>
#include <fstream> // for files manipulation
#include <complex> // for complex numbers
#include <cmath> // round
#include <string>
#include <pthread.h>

#define NUMBER_THREADS 16

using namespace std;

// Function Prototype
void *thread(void *arg);

//Resolution of the image
int pixelHeight = 1080;
int pixelWidth = 1920;

//Window vars
long double minimumX = -2.25;
long double maximumX = .75;

long double minimumY = -1.125;
long double maximumY = 1.125;
//auto calculate maxY to eliminate stretching
//long double maximumY = minimumY + (maximumX - minimumX) * height / width;

long double threadWidth = (maximumX - minimumX) / NUMBER_THREADS;
int threadPixelWidth = pixelWidth / NUMBER_THREADS;

int MAX_ITERATIONS = 5000;

//maps given variable from one scale to another
long double map(long double value, long double inMin, long double inMax, long double outMin, long double outMax){
	return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

int main(){

	//open file
	ofstream fullImage ("mandelbrot.ppm");
	if(fullImage.is_open ()) {

		//image size data
		fullImage << "P3\n" << pixelHeight << " " << pixelWidth << " 255\n";

		// Create threads
		pthread_t threadID[NUMBER_THREADS];
		for(int i = 0; i < NUMBER_THREADS; i++){
			long double *arg = new long double[3];
			arg[0] = minimumX + (threadWidth * i);			// Xmin
			arg[1] = minimumX + (threadWidth * (i + 1));	// Xmax
			arg[2] = i;										// threadNumber
			pthread_create(&threadID[i], NULL, thread, arg);
		}

		// Wait for threads to finish
		for(int i = 0; i < NUMBER_THREADS; i++){
			pthread_join(threadID[i], NULL);
		}

		// Combine images
		for(int i = 0; i < NUMBER_THREADS; i++){
			string fileName = "mandelbrot";
			fileName.append(to_string(i));
			ifstream partialImage(fileName);
			fullImage << partialImage.rdbuf();
			partialImage.close();
			remove(fileName.c_str());
		}

		//close the ofstream
		fullImage.close();
		cout << "Finished!" << endl;
	}
	//handle not opening file
	else{
		cout << "Could not open the file";
	}
	return 0;
}

void *thread(void *arg){
	// Read args
	long double *argz = (long double*)arg;
	long double threadMinimumX = argz[0];
	long double threadMaximumX = argz[1];
	long double threadNumber = argz[2];
	int threadInt = (int)threadNumber;

	//open file
	string fileName = "mandelbrot";
	fileName.append(to_string(threadInt));
	ofstream my_Image (fileName);
	if(my_Image.is_open ()) {

		//iterate over each pixel
		for(int xPixelCoord = 0; xPixelCoord < threadPixelWidth; xPixelCoord++){
			for(int yPixelCoord = 0; yPixelCoord < pixelHeight; yPixelCoord++){
				
				//Convert pixel coords to unreal plane coords (x,iy)
				long double x = map(xPixelCoord, 0, threadPixelWidth, threadMinimumX, threadMaximumX);
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
			}
		}

	//close the ofstream
	my_Image.close();
	cout << "Thread " << threadInt << " Finished!" << endl;
	}
	//handle not opening file
	else{
		cout << "Could not open the file";
		return (void*)1;
	}
	return (void*)0;
}