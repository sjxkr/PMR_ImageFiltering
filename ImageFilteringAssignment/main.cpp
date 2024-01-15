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
	vector<vector<Pixel>> image;	// Image data

	cout << Info[0] << endl;	//only for debugging
	cout << inputFilename << endl;	//only for debugging

	openIOFiles(fin, fout, inputFilename);		//Open input and output file
	convertP6ToP3(fin, fout, image, Info);

	/*
	Testing section to understand given smooth function
	*/

	int h = image.size();	//get iamge size
	int w = image[0].size();

	cout << "The image height is " << h << endl;
	cout << "The image width is " << w << endl;

	Pixel first;
	int i = 0;
	int j = 1;

	first = image[i][j];

	cout << "The selected pixel colour is " << first << endl;

	return(0);
}