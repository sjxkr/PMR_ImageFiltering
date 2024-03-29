/*----------------------------------------------------------------*/
/* Pixel class declaration.                                       */
/* File name: pixel.h                                             */
/* This class implements the concept of a pixel                   */
#ifndef PIXEL_H
#define PIXEL_H
#include <iostream> //Required for istream, ostream
using namespace std;

class Pixel
{
  public:
	// constants to enable undeflow/overflow detection in rgb values
	static const unsigned int MAXVAL = 255;
	static const unsigned int MINVAL = 4000000000;		// compilers interpretation of signed int32 into a unsigned = 2^32 - n. Negative pixel values are represented as very large positive integers. 
	static const unsigned short RMINMASK = 32;
	static const unsigned short GMINMASK = 16;
	static const unsigned short BMINMASK = 8;
	static const unsigned short RMASK = 4;
	static const unsigned short GMASK = 2;
	static const unsigned short BMASK = 1;
	static const unsigned short CHECK = 255;

	//Constructors 
	Pixel(); //Default
	Pixel(unsigned ); //Grey scale
	Pixel(unsigned,unsigned,unsigned); //Full color range

	// Accessor Methods
	unsigned getRed() const {return red;}
	unsigned getGreen() const {return green;}
	unsigned getBlue() const {return blue;}

	// Mutator Methods
	Pixel& setPixel(unsigned r, unsigned g, unsigned b);
	Pixel& setRed(unsigned r); 
	Pixel& setGreen(unsigned g);
	Pixel& setBlue(unsigned b);

	//Overloaded operators.
	//Addition.
	Pixel operator+(const Pixel& p) const;

	//Multiplication of a Pixel by a floating point value.
	Pixel operator*(double v) const;

	//Division of a Pixel by an integer value.
	Pixel operator/(unsigned v) const;

	//Input operator.
	friend istream& operator >>(istream& in, Pixel& p);

	//Output operator.
	friend ostream& operator <<(ostream& out, const Pixel& p);

	// assignment
	Pixel& operator=(const Pixel& rhs);

	bool overflow(); //check for overflow 
	void reset();   //reset to MAXVAL 

  private:
	void validate(); //set overflow bits
	unsigned int red, green, blue;
	unsigned short overflowFlag;
};
#endif
/*----------------------------------------------------------------*/
