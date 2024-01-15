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
#include <vector>

using namespace std;


int main()
{
	//Declerations
	ifstream fin;	//input file
	ofstream fout;	//output file
	char inputFilename[] = "placeholderfilename.txt";	//intialised char array
	int Info[] = { 0,0,0 }; // used in read header function
	vector<vector<Pixel>> image;	// Image data
	
	vector<vector<int>> filter{
		{-1,-1,-1},
		{-1,9,-1},
		{-1,-1,-1}
	};		//  3x3 filter kernel	

	vector<Pixel>temp;

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
	Pixel sum;
	Pixel product;
	int i = 1;
	int j = 1;

	first = image[i][j];

	cout << "The selected pixel is " << first << endl;
	
	/* Testing Overloaded operators multiplication and addition
	* What i want to achieve here is to apply the filter to one pixel, sum the values, validate them and write them to a file	
	*/

	// initialise filter kernel with values
	
	
	for (int k = 0; k < 3; k++)
	{
		for (int l = 0; l < 3; l++)
		{
			product = (image[k][l] * filter[k][l]);
			temp.push_back(product);
			//cout << filter[k][l] << endl;	//print filter values one by one row by row
		}
	}
	
	//print products for loop

	for (int a = 0; a < 9; a++)
	{
		cout << "value " << a + 1 << " is: " << temp[a] << endl;
	}

	sum = image[i][j] + image[i][j + 1];

	cout << "The sum of " << image[i][j] << " and " << image[i][j + 1] << " is " << sum << endl;
	

	return(0);
}