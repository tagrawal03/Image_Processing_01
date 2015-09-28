/*
***image_proc.h: Declares the class Image and its members
***Author: Taruna Agrawal
***Date: 31st August 2015
*/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

//Image class

class Image
{

private:
        int width;  //image width
        int height; //image height
        int numbytes; //number of bytes per pixel
        unsigned char* data; //image data

public:
       //constructor()
       Image();
       //overloading constructor
       Image(Image *otherImage);
       //Destructor
       ~Image();

       //operator overloading
       Image& operator= (const Image& otherImage);

       //functions
       void setWidth(int w) {width = w; };
       void setHeight(int h) {height = h; };
       void setNumbytes(int n) {numbytes = n; };

       bool readImage(char* fileIn);
       bool writeImage(char* fileOut);
       bool resizing_biInter(int newwidth, int newheight);
       bool demosaic_bilinear();
       bool demosaic_MHP();
       bool hist_eqTF();
       bool hist_eqCPH();
       bool Quantization();
       bool image_extension(int e);
       bool oil_painting();
       unsigned char* median_filtering();
       unsigned char* bilateral_filtering(double a, double b);
       unsigned char* guided_imageFiltering();
};
