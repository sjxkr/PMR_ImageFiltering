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
	//Declerations
	ifstream fin;	//input file
	ofstream fout;	//output file
	char inputFilename[] = "placeholderfilename.txt";	//intialised char array
	int Info[] = { 0,0,0 }; // used in read header function
	vector<vector<Pixel>> image;	// Pixel data of input image

	cout << Info[0] << endl;	//debugging
	cout << inputFilename << endl;	//debugging

	openIOFiles(fin, fout, inputFilename);		//Open input and output file
	convertP6ToP3(fin, fout, image, Info);

	return(0);
}