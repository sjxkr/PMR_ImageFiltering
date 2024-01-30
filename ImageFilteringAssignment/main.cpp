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
	ifstream fin;	// Input filestream
	ofstream fout;	// Output filestream
	char inputFilename[MAXLEN];
	char userFilename[MAXLEN];
	int Info[3];
	vector<vector<Pixel>> image;	// Pixel data of input image

	// greeting
	cout << "Hello! Welcome to the image filtering program!\n";
	cout << "Follow the instructions and enjoy!\n\n";

	// Open input and output streams and apply sharpen filter
	openIOFiles(fin, fout, inputFilename);		
	convertP6ToP3(fin, fout, image, Info);
	
	// close input and output streams
	closeIOFiles(fin, fout);

	// prompt user for output filename
	cout << "Please enter your output filename without file extension : ";
	cin >> userFilename;

	// program status update
	cout << "\nImage conversion complete.... Applying filters.... Please wait...\n\n";

	// apply filters to image
	sharpen(image, userFilename);
	smooth(image, userFilename);
	edgeDetection(image, userFilename);

	// program status update
	cout << "\nImage processing complete!\n";

	
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