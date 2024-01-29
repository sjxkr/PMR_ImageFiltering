#include "ImageFunctions.h"
#include <bitset>
#include <cmath>

void openIOFiles(ifstream& fin, ofstream& fout, char inputFilename[])
{
	// define filestreams
	char outputFilename[MAXLEN];

	// prompt user for input file
	cout << "enter filename ";
	cin >> inputFilename;

	// open .ppm binary file for input
	fin.open(inputFilename, ios::binary);

	if (!fin) {
		cout << "could not open file: bye" << endl;
		exit(1);
	}

	// generate meaningful output filename
	strcpy(outputFilename, inputFilename);

	// terminate output fielname at .
	char* loc = strchr(outputFilename, '.');
	*loc = '\0';

	// add extension to name
	strcat(outputFilename, "P3.ppm");

	// open .ppm ascii file for output
	fout.open(outputFilename);

	if (!fout) {
		cout << "cannot open output file: bye" << endl;
	}
}

void closeIOFiles(ifstream& fin, ofstream& fout)
{
	// close input and output files
	fin.close();
	fout.close();
}

void convertP6ToP3(ifstream& bin, ofstream& out, vector<vector<Pixel> >& image, int info[1])
{
	readHeader(bin, out, info);
	int width = info[0], height = info[1];
	readAndWriteImageData(bin, out, image, width, height);
}


void sharpen(vector<vector<Pixel> >& image)
{

	// define variables
	int h = image.size();
	int w = image[0].size();
	ofstream out;
	char outFilename[MAXLEN];
	char comment[MAXLEN] = "# Sharpen filter has been applied this image";
	int r = 0, g = 0, b = 0;
	int maxColor = 0; // change this to be read from header, imageinfo variable.
	vector<vector<Pixel>> imageOut = image;		// output image intialised as input image

	// initialise convolution kernel
	vector<vector<int>> filter{
		{-1, -1, -1},
		{-1, 12, -1},
		{-1, -1, -1}
	};

	// open file for filtered image data

	// prompt user for filename
	cout << "enter filename for filtered image:" << endl;
	cin >> outFilename;

	// add suffix and file extension
	strcat(outFilename, "P3_sh.ppm");

	// try to open the file
	try {
		if (!out) {
			throw runtime_error("Could not create file!");
		}
		out.open(outFilename);
	}
	// handle the exception
	catch (exception e) {
		cout << "Error opening out file" << endl;
	}

	// apply sharpen filter to inner pixels
	for (int i = 1; i < h - 1; i++)
		for (int j = 1; j < w - 1; j++)
		{
			// local variables
			int redSumProduct = 0, greenSumProduct = 0, blueSumProduct = 0;

			// pass kernel over pixels
			for (int k = -1; k < 2; k++)
				for (int l = -1; l < 2; l++)
				{
					// compute rgb sum product
					redSumProduct += image[i + k][j + l].getRed() * filter[k + 1][l + 1];
					greenSumProduct += image[i + k][j + l].getGreen() * filter[k + 1][l + 1];
					blueSumProduct += image[i + k][j + l].getBlue() * filter[k + 1][l + 1];
				}

			// deal with pixel values below zero
			if (redSumProduct < 0) { redSumProduct = 0; };
			if (greenSumProduct < 0) { greenSumProduct = 0; };
			if (blueSumProduct < 0) { blueSumProduct = 0; };

			// set new pixel rgb value
			imageOut[i][j].setPixel(redSumProduct, greenSumProduct, blueSumProduct);

			// deal with overflow condition
			if (imageOut[i][j].overflow()) { imageOut[i][j].reset(); }

			// get max colour value
			r = imageOut[i][j].getRed();
			g = imageOut[i][j].getGreen();
			b = imageOut[i][j].getBlue();

			if (r > maxColor) { maxColor = r; }
			if (g > maxColor) { maxColor = g; }
			if (b > maxColor) { maxColor = b; }
		}

	// write image data to file
	writeP3Image(out, imageOut, comment, maxColor);

}

void smooth(vector<vector<Pixel> >& image)
{
	// define variables
	int h = image.size();
	int w = image[0].size();
	ofstream out;
	char outFilename[MAXLEN];
	char comment[MAXLEN] = "# Smooth filter has been applied this image";
	int maxColor = 0; // change this to be read from header, imageinfo variable.
	Pixel sum;
	vector<vector<Pixel>> imageOut = image;		// output image intialised as input image

	// open file for filtered image data

	// prompt user for filename
	cout << "enter filename for filtered image:" << endl;
	cin >> outFilename;

	// add suffix and file extension
	strcat(outFilename, "P3_sm.ppm");

	// try to open the file
	try {
		if (!out) {
			throw runtime_error("Could not create file!");
		}
		out.open(outFilename);
	}
	// handle the exception
	catch (exception e) {
		cout << "Error opening out file" << endl;
	}

	// compute average value of neighbouring pixels
	for (int i = 1; i < h - 1; i++)
		for (int j = 1; j < w - 1; j++)
		{
			sum = image[i + 1][j] + image[i - 1][j] + image[i][j + 1] + image[i][j - 1];	// sum weighted pixel values
			sum = sum / 4;	// calculate average

			// Set new pixel value is overflow = false
			if (!sum.overflow()) {
				imageOut[i][j] = sum;
			}
			else
			{
				// print message
				cout << "Overflow detected at pixel index " << "[" << i << "]" << "[" << j << "]" << endl;
			}	
		}
	
	// write image data to file
	writeP3Image(out, imageOut, comment, maxColor);

}

void edgeDetection(vector<vector<Pixel> >& image)
{
	// define variables
	int h = image.size();
	int w = image[0].size();
	ofstream out, out2;
	char outFilename[MAXLEN];
	char comment[MAXLEN] = "# Edge detection has been applied this image";
	int r = 0, g = 0, b = 0;
	int maxColor = 0; // change this to be read from header, imageinfo variable.
	vector<vector<Pixel>> hDeriv = image, vDeriv = image;	// output derivatives of each kernel 
	vector<vector<Pixel>> imageOut = image;		// output image intialised as input image

	// initialise horizontal convolution kernel
	vector<vector<int>> hFilter{
		{ 1,  0, -1},
		{ 2,  0, -2},
		{ 1,  0, -1}
	};

	// initialise vertical convolution kernel
	vector<vector<int>> vFilter{
		{ 1,  2,  1},
		{ 0,  0,  0},
		{-1,  2, -1}
	};

	// open file for filtered image data

	// prompt user for filename
	cout << "enter filename for filtered image:" << endl;
	cin >> outFilename;

	// add suffix and file extension
	strcat(outFilename, "P3_ed.ppm");

	// try to open the file
	try {
		if (!out) {
			throw runtime_error("Could not create file!");
		}
		out.open(outFilename);
		out2.open("vertical.ppm");
	}
	// handle the exception
	catch (exception e) {
		cout << "Error opening out file" << endl;
	}

	// apply filter to inner pixels
	for (int i = 1; i < h - 1; i++)
		for (int j = 1; j < w - 1; j++)
		{
			// local variables
			int redSumProductH = 0, greenSumProductH = 0, blueSumProductH = 0;
			int redSumProductV = 0, greenSumProductV = 0, blueSumProductV = 0;
			int redValue = 0, grnValue = 0, bluValue = 0;

			// pass kernel over pixels
			for (int k = -1; k < 2; k++)
				for (int l = -1; l < 2; l++)
				{
					// pass horizontal kernel over image
					redSumProductH += image[i + k][j + l].getRed() * hFilter[k + 1][l + 1];
					greenSumProductH += image[i + k][j + l].getGreen() * hFilter[k + 1][l + 1];
					blueSumProductH += image[i + k][j + l].getBlue() * hFilter[k + 1][l + 1];

					// pass vertical kernel over image
					redSumProductV += image[i + k][j + l].getRed() * vFilter[k + 1][l + 1];
					greenSumProductV += image[i + k][j + l].getGreen() * vFilter[k + 1][l + 1];
					blueSumProductV += image[i + k][j + l].getBlue() * vFilter[k + 1][l + 1];

				}

			// deal with pixel values below zero
			if (redSumProductH < 0) { redSumProductH = 0; };
			if (greenSumProductH < 0) { greenSumProductH = 0; };
			if (blueSumProductH < 0) { blueSumProductH = 0; };
			if (redSumProductV < 0) { redSumProductV = 0; };
			if (greenSumProductV < 0) { greenSumProductV = 0; };
			if (blueSumProductV < 0) { blueSumProductV = 0; };


			// set new pixel rgb value
			hDeriv[i][j].setPixel(redSumProductH, greenSumProductH, blueSumProductH);

			// set new pixel rgb value
			vDeriv[i][j].setPixel(redSumProductV, greenSumProductV, blueSumProductV);


			// deal with overflow condition in horizontal derivative
			if (hDeriv[i][j].overflow()) { hDeriv[i][j].reset(); }

			// deal with overflow condition in vertical derivative
			if (vDeriv[i][j].overflow()) { vDeriv[i][j].reset(); }

			// combine the two derivatives
			redValue = lrint(sqrt(pow(hDeriv[i][j].getRed(),2) + pow(vDeriv[i][j].getRed(),2)));
			grnValue = lrint(sqrt(pow(hDeriv[i][j].getGreen(),2) + pow(vDeriv[i][j].getGreen(),2)));
			bluValue = lrint(sqrt(pow(hDeriv[i][j].getBlue(),2) + pow(vDeriv[i][j].getBlue(),2)));

			// deal with underfow
			if (redValue < 0) { redValue = 0; };
			if (grnValue < 0) { grnValue = 0; };
			if (bluValue < 0) { bluValue = 0; };

			// set combined pixel value
			imageOut[i][j].setPixel(redValue, grnValue, bluValue);

			// deal with overflow
			if (imageOut[i][j].overflow()) { imageOut[i][j].reset(); };

			// get max colour value
			r = imageOut[i][j].getRed();
			g = imageOut[i][j].getGreen();
			b = imageOut[i][j].getBlue();

			if (r > maxColor) { maxColor = r; }
			if (g > maxColor) { maxColor = g; }
			if (b > maxColor) { maxColor = b; }
		}

	// debugging print values
	int d=128, f=100;		// pixel values

	cout << "Pixel at index [-1][-1] is "<<image[d - 1][f - 1] << endl;
	cout << "Pixel at index [-1][0] is "<<image[d - 1][f - 0] << endl;
	cout << "Pixel at index [-1][1] is "<<image[d - 1][f + 1] << endl;
	cout << "Pixel at index [0][-1] is "<<image[d - 0][f - 1] << endl;
	cout << "Pixel at index [0][0] is "<<image[d - 0][f - 0] << endl;
	cout << "Pixel at index [0][1] is "<<image[d - 0][f + 1] << endl;
	cout << "Pixel at index [1][-1] is "<<image[d + 1][f - 1] << endl;
	cout << "Pixel at index [1][0] is "<<image[d + 1][f - 0] << endl;
	cout << "Pixel at index [1][1] is "<<image[d + 1][f + 1] << endl;
	cout << "Pixel value in vertical derivative = " << vDeriv[d][f] << endl;
	cout << "Pixel value in horizontal derivative = " << hDeriv[d][f] << endl;
	cout << "Combined pixel value = " << imageOut[d][f] << endl;

	// write image data to file
	writeP3Image(out, imageOut, comment, maxColor);

}

bool pixelEdgeRowColCheck(vector<vector<Pixel>>& image, int i, int j)
{
	// check if pixel is in first or last row or column

	// define input variables
	bool edge = false;
	int h = image.size();
	int w = image[0].size();

	if ((i == 0 || i == h-1) || (j == 0 || j == w-1))
	{
		edge = true;
		return(edge);
	}

	edge = false;
	return(edge);
}

void writeP3Image(ofstream& out, vector<vector<Pixel> >& image, char comment[], int maxColor)
{
	int h, w, pCount(0);
	char magicNumber[3] = "P3";
	h = (int)image.size();
	w = (int)image[0].size();

	writeHeader(out, magicNumber, comment, w, h, maxColor);

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++)
		{
			out << image[i][j];
			++pCount;
			if (pCount == w) {
				out << newline;
				pCount = 0;
			}
			else out << ' ';
		}
	}
}


void readAndWriteImageData(ifstream& fin, ofstream& fout, vector<vector<Pixel> >& image, int w, int h)
{
	// read and write image data
	// define input variables

	int charCount = 0;
	char colorByte;
	unsigned char aChar;
	unsigned int triple[3];   // red, green, blue

	// allocate memory
	image.resize(h); // allocate h rows

	for (int i = 0; i < h; i++)
	{
		image[i].resize(w);   // for each row allocate w columns
		for (int j = 0; j < w; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				// read one byte
				fin.read(&colorByte, 1);

				// convert to unsigned char
				aChar = (unsigned char)colorByte;

				// save as unsigned int
				triple[k] = (unsigned int)aChar;

				// write as int
				fout << triple[k] << ' ';
			}
			// CR printed over 32 pixels
			++charCount;
			if (charCount == 32) {
				fout << "\r\n";
				charCount = 0;
			}
			image[i][j].setPixel(triple[0], triple[1], triple[2]);
		}
	}
}

void readHeader(ifstream& fin, ofstream& fout, int imageInfo[])
{
	// define and initialise input variables
	char bData[MAXLEN] = { 0 }, magicNumber[MAXWIDTH], comment[MAXLEN] = { "#" };
	int bIndex = 0, charCount = 0, infoCount = 0;
	char ch, aNumber[MAXWIDTH];
	// input first line of text header(magic number)
	// if the magic number is not P6 exit the program
	fin.getline(magicNumber, 3);
	if (strcmp(magicNumber, "P6") != 0)
	{
		cout << "unexpected file format\n";
		exit(1);
	}
	// clear bData array and reset bIndex
	// input next line of text header
	strcpy(bData, " ");
	bIndex = 0;
	fin.getline(bData, MAXLEN);
	do {
		// is this the beginning of a comment
		ch = bData[bIndex];
		if (ch == '#') {
			// comment has been read
			// get all characters until a newline is found
			charCount = 0;
			while (ch != terminator && charCount < MAXLEN) {
				comment[charCount] = ch;
				++bIndex;
				++charCount;
				ch = bData[bIndex];
				cout << ch;
			}
			if ((charCount == MAXLEN) && (ch != newline)) {
				cout << "comment exceeded max length of " << MAXLEN << endl;
				exit(1);
			}
			// get the next line of data
			strcpy(bData, " ");
			bIndex = 0;
			fin.getline(bData, MAXLEN);
		}
		else {
			// this is not a comment
			// parse bData for image information
			charCount = 0;
			// look past whitespace
			while (bIndex < MAXLEN && isspace(bData[bIndex])) {
				++bIndex;
			}
			// may be the beginning of a decimal value
			while (bIndex < MAXLEN && isdigit(bData[bIndex]))
			{
				aNumber[charCount] = bData[bIndex];
				++bIndex;
				++charCount;
				if (charCount == MAXWIDTH)
				{
					cerr << "Maximum width of " << MAXWIDTH << " digits was exceeded.. " << endl;
					exit(1);
				}
			}

			// look at size of aNumber
			if (charCount > 0) {
				// we have image information, terminate string
				aNumber[charCount] = nullChar;
				// convert from ascii to integer
				imageInfo[infoCount] = atoi(aNumber);
				++infoCount;
				// verify input
				switch (infoCount) {
				case 1: cout << "a width of " << imageInfo[infoCount - 1] << " has been read " << endl;
					break;
				case 2: cout << " a height of " << imageInfo[infoCount - 1] << " has bene read " << endl;
					break;
				case 3: cout << "maxcolor of " << imageInfo[infoCount - 1] << " has been read " << endl;
					break;
				}
			}
			else if (infoCount < 3) {
				// aNumber has 0 digits and infoCount < 3
				// we need more image information
				// get next line of data and parse for image information
				strcpy(bData, " ");
				bIndex = 0;
				fin.getline(bData, MAXLEN);
			}
		}
	} while (infoCount < 3 && !fin.eof());
	if (infoCount < 3) {
		cerr << "image information could not be found " << endl;
		exit(1);
	}
	// we have all of the information
	// write header to ascii file
	strcpy(magicNumber, "P3");
	writeHeader(fout, magicNumber, comment, imageInfo[0], imageInfo[1], imageInfo[2]);
}

void writeHeader(ofstream& fout, char magicNumber[], char comment[], int w, int h, int maxPixelVal)
{
	// write image imformation to output file
	fout << magicNumber << newline;
	fout << comment << newline;
	fout << w << ' ' << h << ' ' << maxPixelVal << newline;
}

