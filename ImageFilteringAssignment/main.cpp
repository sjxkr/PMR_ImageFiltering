/*
* Course Title		-		MSc Robotics
* Module			-		Programming Methods for Robotics
* Assignment		-		Image Processing Filtering Operation
* Lecturer			-		Dr. Peter Sherar
* Author			-		Shahir Jagot
* 
* Project Desc		-		Applies sharpen, smooth and edge detection filters to ppm image files
*/


#include "ImageFunctions.h"
#include "Pixel.h"
#include <iostream>

using namespace std;


int main()
{
	//testing opening of files
	ifstream fin;
	ofstream fout;
	char inputFilename[] = "input.txt";

	openIOFiles(fin, fout, inputFilename);		//Open input and output file

	// read from input and copy to output
	vector<vector<Pixel>> image;
	int w = 320;
	int h = 200;

	readAndWriteImageData(fin, fout, image, w, h);

	return(0);
}