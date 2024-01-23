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
	// Declerations
	ifstream fin;	// Input file
	ofstream fout;	// Output file
	char inputFilename[MAXLEN];	
	int Info[3];
	vector<vector<Pixel>> image;	// Pixel data of input image

	// Open input and output streams and apply sharpen filter
	openIOFiles(fin, fout, inputFilename);		
	convertP6ToP3(fin, fout, image, Info);

	// Close input and output streams
	closeIOFiles(fin, fout);

	// Apply filters to image
	sharpen(image);
	smooth(image);
	
	/*
	writeHeader;
	writeP3Image;
	closeIOFiles(fin, fout);
	openIOFiles(fin, fout, inputFilename);
	edgeDetection(image);
	writeHeader;
	writeP3Image;
	closeIOFiles(fin, fout);
	*/

	return(0);
}